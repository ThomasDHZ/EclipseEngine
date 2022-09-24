#include "BloomBlurPipeline.h"

BloomBlurPipeline::BloomBlurPipeline()
{
}

BloomBlurPipeline::~BloomBlurPipeline()
{
}

void BloomBlurPipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<Texture> texture)
{
    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "BloomBlurVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "BloomBlurFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

    VkDescriptorImageInfo BlurTextureBuffer;
    BlurTextureBuffer.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    BlurTextureBuffer.imageView = texture->View;
    BlurTextureBuffer.sampler = texture->Sampler;

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    AddTextureDescriptorSetBinding(DescriptorBindingList, 0, BlurTextureBuffer);

    BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
    buildGraphicsPipelineInfo.ColorAttachments = pipelineInfoStruct.ColorAttachments;
    buildGraphicsPipelineInfo.DescriptorBindingList = DescriptorBindingList;
    buildGraphicsPipelineInfo.renderPass = pipelineInfoStruct.renderPass;
    buildGraphicsPipelineInfo.PipelineShaderStageList = PipelineShaderStageList;
    buildGraphicsPipelineInfo.sampleCount = pipelineInfoStruct.SampleCount;
    buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderMesh;
    buildGraphicsPipelineInfo.ConstBufferSize = sizeof(BloomSettings);
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

void BloomBlurPipeline::Draw(VkCommandBuffer& commandBuffer, BloomSettings& bloomSettings)
{
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
    vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(BloomSettings), &bloomSettings);
    vkCmdDraw(commandBuffer, 6, 1, 0, 0);
}