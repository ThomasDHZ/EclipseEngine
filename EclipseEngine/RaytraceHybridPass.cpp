#include "RaytraceHybridPass.h"
#include "Math.h"
#include "MeshRendererManager.h"

RaytraceHybridPass::RaytraceHybridPass() : RenderPass()
{
}

RaytraceHybridPass::~RaytraceHybridPass()
{
}

void RaytraceHybridPass::BuildRenderPass()
{
    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();
    if (renderPass == nullptr)
    {
        RenderedShadowTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R8G8B8A8_UNORM));

        BuildRenderPassPipelines();
        SetUpCommandBuffers();
    }
    else
    {
        RenderedShadowTexture->RecreateRendererTexture(RenderPassResolution);
        BuildRenderPassPipelines();
    }
}

void RaytraceHybridPass::SetUpCommandBuffers()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = VulkanRenderer::GetCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(VulkanRenderer::GetDevice(), &allocInfo, &RayTraceCommandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void RaytraceHybridPass::BuildRenderPassPipelines()
{
    std::vector<VkPipelineShaderStageCreateInfo> ShaderList;
    std::vector<VkRayTracingShaderGroupCreateInfoKHR> RayTraceShaderList;
    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    {
        ShaderList.emplace_back(CreateShader("../raygen.rgen.spv", VK_SHADER_STAGE_RAYGEN_BIT_KHR));
        VkRayTracingShaderGroupCreateInfoKHR RayGeneratorShaderInfo = {};
        RayGeneratorShaderInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
        RayGeneratorShaderInfo.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
        RayGeneratorShaderInfo.generalShader = static_cast<uint32_t>(ShaderList.size()) - 1;
        RayGeneratorShaderInfo.closestHitShader = VK_SHADER_UNUSED_KHR;
        RayGeneratorShaderInfo.anyHitShader = VK_SHADER_UNUSED_KHR;
        RayGeneratorShaderInfo.intersectionShader = VK_SHADER_UNUSED_KHR;
        RayTraceShaderList.emplace_back(RayGeneratorShaderInfo);

        ShaderList.emplace_back(CreateShader("../miss.rmiss.spv", VK_SHADER_STAGE_MISS_BIT_KHR));
        VkRayTracingShaderGroupCreateInfoKHR MissShaderInfo = {};
        MissShaderInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
        MissShaderInfo.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
        MissShaderInfo.generalShader = static_cast<uint32_t>(ShaderList.size()) - 1;
        MissShaderInfo.closestHitShader = VK_SHADER_UNUSED_KHR;
        MissShaderInfo.anyHitShader = VK_SHADER_UNUSED_KHR;
        MissShaderInfo.intersectionShader = VK_SHADER_UNUSED_KHR;
        RayTraceShaderList.emplace_back(MissShaderInfo);

        ShaderList.emplace_back(CreateShader("../shadow.rmiss.spv", VK_SHADER_STAGE_MISS_BIT_KHR));
        VkRayTracingShaderGroupCreateInfoKHR ShadowShaderInfo = {};
        ShadowShaderInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
        ShadowShaderInfo.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
        ShadowShaderInfo.generalShader = static_cast<uint32_t>(ShaderList.size()) - 1;
        ShadowShaderInfo.closestHitShader = VK_SHADER_UNUSED_KHR;
        ShadowShaderInfo.anyHitShader = VK_SHADER_UNUSED_KHR;
        ShadowShaderInfo.intersectionShader = VK_SHADER_UNUSED_KHR;
        RayTraceShaderList.emplace_back(ShadowShaderInfo);

        ShaderList.emplace_back(CreateShader("../hybridclosesthit.rchit.spv", VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR));
        VkRayTracingShaderGroupCreateInfoKHR ClosestHitShaderInfo = {};
        ClosestHitShaderInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
        ClosestHitShaderInfo.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR;
        ClosestHitShaderInfo.generalShader = VK_SHADER_UNUSED_KHR;
        ClosestHitShaderInfo.closestHitShader = static_cast<uint32_t>(ShaderList.size()) - 1;

        ShaderList.emplace_back(CreateShader("../anyhit.rahit.spv", VK_SHADER_STAGE_ANY_HIT_BIT_KHR));
        ClosestHitShaderInfo.anyHitShader = static_cast<uint32_t>(ShaderList.size()) - 1;
        ClosestHitShaderInfo.intersectionShader = VK_SHADER_UNUSED_KHR;
        RayTraceShaderList.emplace_back(ClosestHitShaderInfo);

        ShaderList.emplace_back(CreateShader("../anyhit1.rahit.spv", VK_SHADER_STAGE_ANY_HIT_BIT_KHR));
        VkRayTracingShaderGroupCreateInfoKHR ShadwoHitShaderInfo = {};
        ShadwoHitShaderInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
        ShadwoHitShaderInfo.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR;
        ShadwoHitShaderInfo.generalShader = VK_SHADER_UNUSED_KHR;
        ShadwoHitShaderInfo.closestHitShader = VK_SHADER_UNUSED_KHR;
        ShadwoHitShaderInfo.anyHitShader = static_cast<uint32_t>(ShaderList.size()) - 1;
        ShadwoHitShaderInfo.intersectionShader = VK_SHADER_UNUSED_KHR;
        RayTraceShaderList.emplace_back(ShadwoHitShaderInfo);
    }
    {
        VkWriteDescriptorSetAccelerationStructureKHR AccelerationDescriptorStructure = AddAcclerationStructureBinding(DescriptorBindingList, TopLevelAccelerationStructureManager::GetAccelerationStructureHandlePtr());
        VkDescriptorImageInfo RayTracedTextureMaskDescriptor = AddRayTraceStorageImageDescriptor(DescriptorBindingList, VK_IMAGE_LAYOUT_GENERAL, RenderedShadowTexture->View);
        std::vector<VkDescriptorImageInfo> RenderedTextureBufferInfo = TextureManager::GetTexturemBufferList();
        std::vector<VkDescriptorBufferInfo> MeshVertexBufferList = MeshRendererManager::GetMeshVertexBuffer();
        std::vector<VkDescriptorBufferInfo> MeshIndexBufferList = MeshRendererManager::GetMeshIndexBuffer();
        std::vector<VkDescriptorBufferInfo> MeshPropertiesBufferList = MeshRendererManager::GetMeshPropertiesBuffer();
        std::vector<VkDescriptorBufferInfo> DirectionalLightBufferInfoList = LightManager::GetDirectionalLightBuffer();
        std::vector<VkDescriptorBufferInfo> PointLightBufferInfoList = LightManager::GetPointLightBuffer();
        std::vector<VkDescriptorBufferInfo> SpotLightBufferInfoList = LightManager::GetSpotLightBuffer();

        std::vector<VkDescriptorImageInfo> CubeMapBufferInfoList;
        const auto cubeMap = TextureManager::GetCubeMapTextureList()[0];
        VkDescriptorImageInfo cubeMapBuffer;
        cubeMapBuffer.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        cubeMapBuffer.imageView = cubeMap->View;
        cubeMapBuffer.sampler = cubeMap->Sampler;
        CubeMapBufferInfoList.emplace_back(cubeMapBuffer);

        AddAccelerationDescriptorSetBinding(DescriptorBindingList, 0, AccelerationDescriptorStructure, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR);
        AddStorageTextureSetBinding(DescriptorBindingList, 1, RayTracedTextureMaskDescriptor, VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR);
        AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 2, MeshVertexBufferList);
        AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 3, MeshIndexBufferList);
        AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 4, MeshPropertiesBufferList);
        AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 5, DirectionalLightBufferInfoList);
        AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 6, PointLightBufferInfoList);
        AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 7, SpotLightBufferInfoList);
        AddTextureDescriptorSetBinding(DescriptorBindingList, 8, RenderedTextureBufferInfo, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR);
        AddTextureDescriptorSetBinding(DescriptorBindingList, 9, CubeMapBufferInfoList); 
    }

    if (RayTracePipeline == nullptr)
    {
        RayTracePipeline = std::make_shared<RayTracingPipeline>();
        RayTracePipeline->SetUp(ShaderList, RayTraceShaderList, DescriptorBindingList);
    }
    else
    {
        RayTracePipeline->UpdateGraphicsPipeLine(ShaderList, RayTraceShaderList, DescriptorBindingList);
    }
}

VkCommandBuffer RaytraceHybridPass::Draw()
{
    VkCommandBufferBeginInfo cmdBufInfo{};
    cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    VkImageSubresourceRange subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

    vkBeginCommandBuffer(RayTraceCommandBuffer, &cmdBufInfo);
    const uint32_t handleSizeAligned = GraphicsDevice::GetAlignedSize(GraphicsDevice::GetRayTracingPipelineProperties().shaderGroupHandleSize, GraphicsDevice::GetRayTracingPipelineProperties().shaderGroupHandleAlignment);

    VkStridedDeviceAddressRegionKHR raygenShaderSbtEntry{};
    raygenShaderSbtEntry.deviceAddress = VulkanRenderer::GetBufferDeviceAddress(RayTracePipeline->GetRaygenShaderBindingTable());
    raygenShaderSbtEntry.stride = handleSizeAligned;
    raygenShaderSbtEntry.size = handleSizeAligned;

    VkStridedDeviceAddressRegionKHR missShaderSbtEntry{};
    missShaderSbtEntry.deviceAddress = VulkanRenderer::GetBufferDeviceAddress(RayTracePipeline->GetMissShaderBindingTable());
    missShaderSbtEntry.stride = handleSizeAligned;
    missShaderSbtEntry.size = handleSizeAligned;

    VkStridedDeviceAddressRegionKHR hitShaderSbtEntry{};
    hitShaderSbtEntry.deviceAddress = VulkanRenderer::GetBufferDeviceAddress(RayTracePipeline->GetHitShaderBindingTable());
    hitShaderSbtEntry.stride = handleSizeAligned;
    hitShaderSbtEntry.size = handleSizeAligned;

    SceneManager::sceneProperites.proj = glm::inverse(SceneManager::sceneProperites.proj);
    SceneManager::sceneProperites.view = glm::inverse(SceneManager::sceneProperites.view);

    vkCmdPushConstants(RayTraceCommandBuffer, RayTracePipeline->GetShaderPipelineLayout(), VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_MISS_BIT_KHR | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR, 0, sizeof(SceneProperties), &SceneManager::sceneProperites);
    vkCmdBindPipeline(RayTraceCommandBuffer, VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, RayTracePipeline->GetShaderPipeline());
    vkCmdBindDescriptorSets(RayTraceCommandBuffer, VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, RayTracePipeline->GetShaderPipelineLayout(), 0, 1, RayTracePipeline->GetDescriptorSetPtr(), 0, 0);

    VkStridedDeviceAddressRegionKHR CallableShaderSbtEntry{};
    RenderedShadowTexture->UpdateImageLayout(RayTraceCommandBuffer, VK_IMAGE_LAYOUT_GENERAL);
    VulkanRenderer::vkCmdTraceRaysKHR(RayTraceCommandBuffer, &raygenShaderSbtEntry, &missShaderSbtEntry, &hitShaderSbtEntry, &CallableShaderSbtEntry, VulkanRenderer::GetSwapChainResolution().width, VulkanRenderer::GetSwapChainResolution().height, 1);
    RenderedShadowTexture->UpdateImageLayout(RayTraceCommandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkEndCommandBuffer(RayTraceCommandBuffer);

    return RayTraceCommandBuffer;
}

void RaytraceHybridPass::Destroy()
{
    RenderedShadowTexture->Destroy();
    RayTracePipeline->Destroy();

    RenderPass::Destroy();
}
