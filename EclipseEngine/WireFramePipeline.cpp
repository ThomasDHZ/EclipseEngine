#include "WireFramePipeline.h"

WireFramePipeline::WireFramePipeline()
{
}

WireFramePipeline::~WireFramePipeline()
{
}

void WireFramePipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct)
{
    std::vector<VkDescriptorBufferInfo> MeshPropertiesmBufferList = MeshRendererManager::GetMeshPropertiesBuffer();

    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "WireFrameShaderVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "WireFrameShaderFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, MeshPropertiesmBufferList, VK_SHADER_STAGE_VERTEX_BIT);

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
    VertexDescriptionInfo.PolygonMode = VK_POLYGON_MODE_LINE;
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

void WireFramePipeline::Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh)
{
    SceneManager::sceneProperites.MeshIndex = mesh->GetMeshBufferIndex();
    SceneManager::sceneProperites.MeshColorID = Converter::PixelToVec3(mesh->GetMeshColorID());

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
    vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SceneProperties), &SceneManager::sceneProperites);
    mesh->Draw(commandBuffer);
}
