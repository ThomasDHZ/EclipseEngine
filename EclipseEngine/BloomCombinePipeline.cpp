#include "BloomCombinePipeline.h"

BloomCombinePipeline::BloomCombinePipeline()
{
}

BloomCombinePipeline::~BloomCombinePipeline()
{
}

void BloomCombinePipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::vector<std::shared_ptr<RenderedColorTexture>> textureList)
{
    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "AdditionShaderBlurVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "AdditionShaderBlurFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));


    std::vector<VkDescriptorImageInfo> blurTextureBufferList;
    for (auto& texture : textureList)
    {
        VkDescriptorImageInfo BlurTextureBuffer;
        BlurTextureBuffer.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        BlurTextureBuffer.imageView = texture->View;
        BlurTextureBuffer.sampler = texture->Sampler;

        blurTextureBufferList.emplace_back(BlurTextureBuffer);
        bloomTextureCount.TextureCount += 1;
    }


    //std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    //AddTextureDescriptorSetBinding(DescriptorBindingList, 0, blurTextureBufferList);

    //BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
    //buildGraphicsPipelineInfo.ColorAttachments = pipelineInfoStruct.ColorAttachments;
    //buildGraphicsPipelineInfo.DescriptorBindingList = DescriptorBindingList;
    //buildGraphicsPipelineInfo.renderPass = pipelineInfoStruct.renderPass;
    //buildGraphicsPipelineInfo.PipelineShaderStageList = PipelineShaderStageList;
    //buildGraphicsPipelineInfo.sampleCount = pipelineInfoStruct.SampleCount;
    //buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderMesh;
    //buildGraphicsPipelineInfo.ConstBufferSize = sizeof(BloomIndexSettings);
    //buildGraphicsPipelineInfo.VertexDescriptorType = VertexDescriptorTypeEnum::kVertexNone;

    //if (ShaderPipeline == nullptr)
    //{
    //    CreateGraphicsPipeline(buildGraphicsPipelineInfo);
    //}
    //else
    //{
    //    Destroy();
    //    UpdateGraphicsPipeLine(buildGraphicsPipelineInfo);
    //}

    for (auto& shader : PipelineShaderStageList)
    {
        vkDestroyShaderModule(VulkanRenderer::GetDevice(), shader.module, nullptr);
    }
}

void BloomCombinePipeline::Draw(VkCommandBuffer& commandBuffer)
{
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
    vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(BloomIndexSettings), &bloomTextureCount);
    vkCmdDraw(commandBuffer, 6, 1, 0, 0);
}
