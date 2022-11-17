#include "DeferredRendererPipeline.h"
DeferredRendererPipeline::DeferredRendererPipeline()
{
}

DeferredRendererPipeline::~DeferredRendererPipeline()
{
}

void DeferredRendererPipeline::InitializePipeline(std::shared_ptr<RenderedColorTexture> PositionTexture,
    std::shared_ptr<RenderedColorTexture> TangentTexture,
    std::shared_ptr<RenderedColorTexture> BiTangentTexture,
    std::shared_ptr<RenderedColorTexture> TBNormalTexture,
    std::shared_ptr<RenderedColorTexture> NormalTexture,
    std::shared_ptr<RenderedColorTexture> AlbedoTexture,
    std::shared_ptr<RenderedColorTexture> SpecularTexture,
    std::shared_ptr<RenderedColorTexture> BloomTexture,
    std::shared_ptr<RenderedColorTexture> ShadowTexture,
    PipelineInfoStruct& pipelineInfoStruct)
{
    std::vector<VkDescriptorBufferInfo> MeshPropertiesmBufferList = MeshRendererManager::GetMeshPropertiesBuffer();
    std::vector<VkDescriptorBufferInfo> DirectionalLightBufferInfoList = LightManager::GetDirectionalLightBuffer();
    std::vector<VkDescriptorBufferInfo> PointLightBufferInfoList = LightManager::GetPointLightBuffer();
    std::vector<VkDescriptorBufferInfo> SpotLightBufferInfoList = LightManager::GetSpotLightBuffer();
    std::vector<VkDescriptorImageInfo> RenderedTextureBufferInfo = TextureManager::GetTexturemBufferList();
    VkDescriptorImageInfo PositionTextureBuffer = AddTextureDescriptor(PositionTexture->View, PositionTexture->Sampler);
    VkDescriptorImageInfo TangentTextureeBuffer = AddTextureDescriptor(TangentTexture->View, TangentTexture->Sampler);
    VkDescriptorImageInfo BiTangentTextureBuffer = AddTextureDescriptor(BiTangentTexture->View, BiTangentTexture->Sampler);
    VkDescriptorImageInfo TBNormalTextureBuffer = AddTextureDescriptor(TBNormalTexture->View, TBNormalTexture->Sampler);
    VkDescriptorImageInfo NormalTextureBuffer = AddTextureDescriptor(NormalTexture->View, NormalTexture->Sampler);
    VkDescriptorImageInfo AlbedoTextureBuffer = AddTextureDescriptor(AlbedoTexture->View, AlbedoTexture->Sampler);
    VkDescriptorImageInfo SpecularTextureBuffer = AddTextureDescriptor(SpecularTexture->View, SpecularTexture->Sampler);
    VkDescriptorImageInfo BloomTextureBuffer = AddTextureDescriptor(BloomTexture->View, BloomTexture->Sampler);
    VkDescriptorImageInfo ShadowTextureBuffer = AddTextureDescriptor(ShadowTexture->View, ShadowTexture->Sampler);

    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "DeferredRendererVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "DeferredRendererFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, MeshPropertiesmBufferList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 1, DirectionalLightBufferInfoList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 2, PointLightBufferInfoList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 3, SpotLightBufferInfoList);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 4, RenderedTextureBufferInfo, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 5, PositionTextureBuffer);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 6, TangentTextureeBuffer);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 7, BiTangentTextureBuffer);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 8, TBNormalTextureBuffer);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 9, NormalTextureBuffer);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 10, AlbedoTextureBuffer);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 11, SpecularTextureBuffer);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 12, BloomTextureBuffer);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 13, ShadowTextureBuffer);

    VkPipelineDepthStencilStateCreateInfo DepthStencilStateCreateInfo{};
    DepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    DepthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
    DepthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
    DepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
    DepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
    DepthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;

    BuildVertexDescription VertexDescriptionInfo{};
    VertexDescriptionInfo.VertexTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    VertexDescriptionInfo.PolygonMode = VK_POLYGON_MODE_FILL;
    VertexDescriptionInfo.CullMode = VK_CULL_MODE_NONE;

    BuildRenderPassDescription RenderPassInfo{};
    RenderPassInfo.PipelineShaderStageList = PipelineShaderStageList;
    RenderPassInfo.DescriptorBindingList = DescriptorBindingList;
    RenderPassInfo.ColorAttachments = pipelineInfoStruct.ColorAttachments;
    RenderPassInfo.DepthStencilInfo = DepthStencilStateCreateInfo;
    RenderPassInfo.renderPass = pipelineInfoStruct.renderPass;
    RenderPassInfo.sampleCount = pipelineInfoStruct.SampleCount;
    RenderPassInfo.ConstBufferSize = sizeof(SceneProperties);

    BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
    buildGraphicsPipelineInfo.VertexDescription = VertexDescriptionInfo;
    buildGraphicsPipelineInfo.RenderPassDescription = RenderPassInfo;

    if (ShaderPipeline == nullptr)
    {
        CreateGraphicsPipeline(buildGraphicsPipelineInfo);
    }
    else
    {
        Destroy();
        UpdateGraphicsPipeLine(buildGraphicsPipelineInfo);
    }

    for (auto& shader : PipelineShaderStageList)
    {
        vkDestroyShaderModule(VulkanRenderer::GetDevice(), shader.module, nullptr);
    }
}

void DeferredRendererPipeline::Draw(VkCommandBuffer& commandBuffer)
{
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
    vkCmdDraw(commandBuffer, 4, 1, 0, 0);
}
