#include "Renderer3DPipeline.h"


Renderer3DPipeline::Renderer3DPipeline()
{
}

Renderer3DPipeline::~Renderer3DPipeline()
{
}

void Renderer3DPipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<RenderedDepthTexture> depthTexture)
{
    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    std::vector<VkDescriptorBufferInfo> MeshPropertiesmBufferList = MeshRendererManager::GetMeshPropertiesBuffer();
    std::vector<VkDescriptorBufferInfo> MaterialBufferList = MaterialManager::GetMaterialBufferList();
    std::vector<VkDescriptorBufferInfo> DirectionalLightBufferInfoList = GLTFSceneManager::GetDirectionalLightPropertiesBuffer();
    std::vector<VkDescriptorBufferInfo> PointLightBufferInfoList = GLTFSceneManager::GetPointLightPropertiesBuffer();
    std::vector<VkDescriptorBufferInfo> SpotLightBufferInfoList = GLTFSceneManager::GetSpotLightPropertiesBuffer();
    std::vector<VkDescriptorImageInfo> RenderedTextureBufferInfo = TextureManager::GetTexturemBufferList();

    VkDescriptorImageInfo ShadowMapBufferInfo;
    ShadowMapBufferInfo.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    ShadowMapBufferInfo.imageView = depthTexture->View;
    ShadowMapBufferInfo.sampler = depthTexture->Sampler;

    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "Renderer3DVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "Renderer3DFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, MeshPropertiesmBufferList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 1, MaterialBufferList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 2, DirectionalLightBufferInfoList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 3, PointLightBufferInfoList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 4, SpotLightBufferInfoList);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 5, RenderedTextureBufferInfo, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR);
   // AddTextureDescriptorSetBinding(DescriptorBindingList, 6, ShadowMapBufferInfo, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR);

    VkPipelineDepthStencilStateCreateInfo DepthStencilStateCreateInfo{};
    DepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    DepthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
    DepthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
    DepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
    DepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
    DepthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;

    BuildVertexDescription VertexDescriptionInfo{};
    VertexDescriptionInfo.VertexBindingDescriptions = Vertex3D::getBindingDescriptions();
    VertexDescriptionInfo.VertexAttributeDescriptions = Vertex3D::getAttributeDescriptions();
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

void Renderer3DPipeline::Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh)
{
    SceneManager::sceneProperites.MeshIndex = mesh->GetMeshBufferIndex();
    SceneManager::sceneProperites.MeshColorID = Converter::PixelToVec3(mesh->GetMeshColorID());

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
    vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SceneProperties), &SceneManager::sceneProperites);
    mesh->Draw(commandBuffer);
}
