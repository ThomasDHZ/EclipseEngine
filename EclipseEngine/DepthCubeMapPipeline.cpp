#include "DepthCubeMapPipeline.h"

DepthCubeMapPipeline::DepthCubeMapPipeline()
{
}

DepthCubeMapPipeline::~DepthCubeMapPipeline()
{
}

void DepthCubeMapPipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct)
{
    std::vector<VkDescriptorBufferInfo> CubeMapSamplerBufferInfoList;

    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "DepthCubeShaderVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "DepthCubeShaderFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

    auto a = GLTFSceneManager::GetCubeMapSamplerBuffer();
    auto b = GLTFSceneManager::GetGameObjectTransformBuffer();
    auto c = GLTFSceneManager::GetGameObjectPropertiesBuffer();
    auto d = GLTFSceneManager::GetMaterialPropertiesBuffer();
    auto e = GLTFSceneManager::GetTexturePropertiesBuffer();

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    AddUniformBufferDescriptorSetBinding(DescriptorBindingList, 0, a);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 1, b);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 2, c);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 3, d);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 4, e, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR);

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

void DepthCubeMapPipeline::Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> mesh, DepthSceneData& depthSceneData)
{
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
    mesh->DrawMesh(commandBuffer, DescriptorSet, ShaderPipelineLayout, depthSceneData);
}
void DepthCubeMapPipeline::Destroy()
{
    GraphicsPipeline::Destroy();
}