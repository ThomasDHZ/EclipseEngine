#include "RayTracePBRRenderPass.h"
#include "EngineMath.h"

RayTracePBRRenderPass::RayTracePBRRenderPass() : RenderPass()
{
}

RayTracePBRRenderPass::~RayTracePBRRenderPass()
{
}

void RayTracePBRRenderPass::StartUp()
{
    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();
    RayTracedTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R8G8B8A8_UNORM));

    BuildRenderPassPipelines();
    SetUpCommandBuffers();
}

void RayTracePBRRenderPass::SetUpCommandBuffers()
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

void RayTracePBRRenderPass::BuildRenderPassPipelines()
{
    std::vector<VkPipelineShaderStageCreateInfo> ShaderList;
    std::vector<VkRayTracingShaderGroupCreateInfoKHR> RayTraceShaderList;
    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    {
        ShaderList.emplace_back(CreateShader("../Shaders/PBRRaygen.rgen.spv", VK_SHADER_STAGE_RAYGEN_BIT_KHR));
        VkRayTracingShaderGroupCreateInfoKHR RayGeneratorShaderInfo = {};
        RayGeneratorShaderInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
        RayGeneratorShaderInfo.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
        RayGeneratorShaderInfo.generalShader = static_cast<uint32_t>(ShaderList.size()) - 1;
        RayGeneratorShaderInfo.closestHitShader = VK_SHADER_UNUSED_KHR;
        RayGeneratorShaderInfo.anyHitShader = VK_SHADER_UNUSED_KHR;
        RayGeneratorShaderInfo.intersectionShader = VK_SHADER_UNUSED_KHR;
        RayTraceShaderList.emplace_back(RayGeneratorShaderInfo);

        ShaderList.emplace_back(CreateShader("../Shaders/PBRMiss.rmiss.spv", VK_SHADER_STAGE_MISS_BIT_KHR));
        VkRayTracingShaderGroupCreateInfoKHR MissShaderInfo = {};
        MissShaderInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
        MissShaderInfo.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
        MissShaderInfo.generalShader = static_cast<uint32_t>(ShaderList.size()) - 1;
        MissShaderInfo.closestHitShader = VK_SHADER_UNUSED_KHR;
        MissShaderInfo.anyHitShader = VK_SHADER_UNUSED_KHR;
        MissShaderInfo.intersectionShader = VK_SHADER_UNUSED_KHR;
        RayTraceShaderList.emplace_back(MissShaderInfo);

        ShaderList.emplace_back(CreateShader("../Shaders/PBRShadow.rmiss.spv", VK_SHADER_STAGE_MISS_BIT_KHR));
        VkRayTracingShaderGroupCreateInfoKHR ShadowShaderInfo = {};
        ShadowShaderInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
        ShadowShaderInfo.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
        ShadowShaderInfo.generalShader = static_cast<uint32_t>(ShaderList.size()) - 1;
        ShadowShaderInfo.closestHitShader = VK_SHADER_UNUSED_KHR;
        ShadowShaderInfo.anyHitShader = VK_SHADER_UNUSED_KHR;
        ShadowShaderInfo.intersectionShader = VK_SHADER_UNUSED_KHR;
        RayTraceShaderList.emplace_back(ShadowShaderInfo);

        ShaderList.emplace_back(CreateShader("../Shaders/PBRClosesthit.rchit.spv", VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR));
        VkRayTracingShaderGroupCreateInfoKHR ClosestHitShaderInfo = {};
        ClosestHitShaderInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
        ClosestHitShaderInfo.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR;
        ClosestHitShaderInfo.generalShader = VK_SHADER_UNUSED_KHR;
        ClosestHitShaderInfo.closestHitShader = static_cast<uint32_t>(ShaderList.size()) - 1;

        ShaderList.emplace_back(CreateShader("../Shaders/PBRAnyhit.rahit.spv", VK_SHADER_STAGE_ANY_HIT_BIT_KHR));
        ClosestHitShaderInfo.anyHitShader = static_cast<uint32_t>(ShaderList.size()) - 1;
        ClosestHitShaderInfo.intersectionShader = VK_SHADER_UNUSED_KHR;
        RayTraceShaderList.emplace_back(ClosestHitShaderInfo);

        ShaderList.emplace_back(CreateShader("../Shaders/PBRAnyhit1.rahit.spv", VK_SHADER_STAGE_ANY_HIT_BIT_KHR));
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
        //layout(binding = 0, set = 0) uniform accelerationStructureEXT topLevelAS;
        //layout(binding = 1, set = 0, rgba8) uniform image2D RayTracedTexture;
        //layout(binding = 2, scalar) buffer Vertices { Vertex v[]; } vertices[];
        //layout(binding = 3) buffer Indices { uint i[]; } indices[];
        //layout(binding = 4) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
        //layout(binding = 5) buffer TransformBuffer { mat4 transform; } transformBuffer[];
        //layout(binding = 6) buffer MaterialPropertiesBuffer { MaterialProperties materialProperties; } materialBuffer[];
        //layout(binding = 7) uniform sampler2D TextureMap[];
        //layout(binding = 8) uniform sampler2D BRDFMap;
        //layout(binding = 9) uniform samplerCube IrradianceMap;
        //layout(binding = 10) uniform samplerCube PrefilterMap;
        //layout(binding = 11) buffer SunLightBuffer { SunLight sunLight; } SULight[];
        //layout(binding = 12) buffer DirectionalLightBuffer { DirectionalLight directionalLight; } DLight[];
        //layout(binding = 13) buffer PointLightBuffer { PointLight pointLight; } PLight[];
        //layout(binding = 14) buffer SpotLightBuffer { SpotLight spotLight; } SLight[];


        std::vector<VkDescriptorPoolSize> descriptorPoolSizeList;
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetGameObjectPropertiesBuffer().size() });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetGameObjectTransformBuffer().size() });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetMaterialPropertiesBuffer().size() });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, (uint32_t)GLTFSceneManager::GetTexturePropertiesBuffer().size() });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetSunLightPropertiesBuffer().size() });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetDirectionalLightPropertiesBuffer().size() });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetPointLightPropertiesBuffer().size() });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetSpotLightPropertiesBuffer().size() });
        DescriptorPool = GLTF_GraphicsDescriptors::CreateDescriptorPool(descriptorPoolSizeList);

        std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBinding;
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetGameObjectPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetGameObjectTransformBuffer().size(), VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetMaterialPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, (uint32_t)GLTFSceneManager::GetTexturePropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 5, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 6, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 7, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetSunLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 8, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetDirectionalLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 9, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetPointLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 10, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetSpotLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
        DescriptorSetLayoutList.emplace_back(GLTF_GraphicsDescriptors::CreateDescriptorSetLayout(descriptorSetLayoutBinding));


        std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
        GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, GLTFSceneManager::GetGameObjectPropertiesBuffer());
        GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 1, GLTFSceneManager::GetGameObjectTransformBuffer());
        GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 2, GLTFSceneManager::GetMaterialPropertiesBuffer());
        GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 3, GLTFSceneManager::GetTexturePropertiesBuffer());
        GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 4, GLTFSceneManager::GetBRDFMapDescriptor());
        GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 5, GLTFSceneManager::GetIrradianceMapDescriptor());
        GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 6, GLTFSceneManager::GetPrefilterMapDescriptor());
        GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 7, GLTFSceneManager::GetSunLightPropertiesBuffer());
        GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 8, GLTFSceneManager::GetDirectionalLightPropertiesBuffer());
        GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 9, GLTFSceneManager::GetPointLightPropertiesBuffer());
        GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 10, GLTFSceneManager::GetSpotLightPropertiesBuffer());
        DescriptorSet = GLTF_GraphicsDescriptors::CreateDescriptorSets(DescriptorPool, DescriptorSetLayout);

        std::vector<VkWriteDescriptorSet> writeDescriptorSet;
        for (auto& DescriptorBinding : DescriptorBindingList)
        {
            if (DescriptorBinding.BufferDescriptor.size() > 0)
            {
                writeDescriptorSet.emplace_back(GLTF_GraphicsDescriptors::AddBufferDescriptorSet(DescriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.BufferDescriptor, DescriptorBinding.DescriptorType));
            }
            else if (DescriptorBinding.TextureDescriptor.size() > 0)
            {
                writeDescriptorSet.emplace_back(GLTF_GraphicsDescriptors::AddTextureDescriptorSet(DescriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.TextureDescriptor, DescriptorBinding.DescriptorType));
            }
            else
            {
                writeDescriptorSet.emplace_back(GLTF_GraphicsDescriptors::AddAccelerationBuffer(DescriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.AccelerationStructureDescriptor));
            }
        }
        vkUpdateDescriptorSets(VulkanRenderer::GetDevice(), static_cast<uint32_t>(writeDescriptorSet.size()), writeDescriptorSet.data(), 0, nullptr);
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

VkCommandBuffer RayTracePBRRenderPass::Draw()
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

    GLTFSceneManager::sceneProperites.proj = glm::inverse(GLTFSceneManager::sceneProperites.proj);
    GLTFSceneManager::sceneProperites.view = glm::inverse(GLTFSceneManager::sceneProperites.view);

    vkCmdPushConstants(RayTraceCommandBuffer, RayTracePipeline->GetShaderPipelineLayout(), VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_MISS_BIT_KHR | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR, 0, sizeof(SceneProperties), &GLTFSceneManager::sceneProperites);
    vkCmdBindPipeline(RayTraceCommandBuffer, VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, RayTracePipeline->GetShaderPipeline());
    vkCmdBindDescriptorSets(RayTraceCommandBuffer, VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, RayTracePipeline->GetShaderPipelineLayout(), 0, 1, RayTracePipeline->GetDescriptorSetPtr(), 0, 0);

    VkStridedDeviceAddressRegionKHR CallableShaderSbtEntry{};
    RayTracedTexture->UpdateImageLayout(RayTraceCommandBuffer, VK_IMAGE_LAYOUT_GENERAL);
    VulkanRenderer::vkCmdTraceRaysKHR(RayTraceCommandBuffer, &raygenShaderSbtEntry, &missShaderSbtEntry, &hitShaderSbtEntry, &CallableShaderSbtEntry, VulkanRenderer::GetSwapChainResolution().width, VulkanRenderer::GetSwapChainResolution().height, 1);
    RayTracedTexture->UpdateImageLayout(RayTraceCommandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkEndCommandBuffer(RayTraceCommandBuffer);
    return RayTraceCommandBuffer;
}

void RayTracePBRRenderPass::RebuildSwapChain()
{
    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();

    RayTracedTexture->RecreateRendererTexture(RenderPassResolution);
  //  BuildRenderPassPipelines();
}

void RayTracePBRRenderPass::Destroy()
{
    RayTracedTexture->Destroy();
    RayTracePipeline->Destroy();

    RenderPass::Destroy();
}
