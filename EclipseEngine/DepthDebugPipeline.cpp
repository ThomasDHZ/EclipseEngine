#include "DepthDebugPipeline.h"

DepthDebugPipeline::DepthDebugPipeline()
{
}

DepthDebugPipeline::~DepthDebugPipeline()
{
}

void DepthDebugPipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<RenderedDepthTexture> depthTexture)
{
    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;

    VkDescriptorImageInfo RenderedTextureBufferInfo = AddTextureDescriptor(depthTexture->View, depthTexture->Sampler);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 0, RenderedTextureBufferInfo, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR);

    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "DepthDebugShaderVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "DepthDebugShaderFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

    BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
    buildGraphicsPipelineInfo.ColorAttachments = pipelineInfoStruct.ColorAttachments;
    buildGraphicsPipelineInfo.DescriptorBindingList = DescriptorBindingList;
    buildGraphicsPipelineInfo.renderPass = pipelineInfoStruct.renderPass;
    buildGraphicsPipelineInfo.PipelineShaderStageList = PipelineShaderStageList;
    buildGraphicsPipelineInfo.sampleCount = pipelineInfoStruct.SampleCount;
    buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderMesh;
    buildGraphicsPipelineInfo.ConstBufferSize = 0;
    buildGraphicsPipelineInfo.VertexDescriptorType = VertexDescriptorTypeEnum::kVertexNone;

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

void DepthDebugPipeline::Draw(VkCommandBuffer& commandBuffer)
{
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
    vkCmdDraw(commandBuffer, 6, 1, 0, 0);
}
