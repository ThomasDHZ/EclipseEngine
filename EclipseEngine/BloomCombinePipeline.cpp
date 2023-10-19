#include "BloomCombinePipeline.h"
#include "File.h"

BloomCombinePipeline::BloomCombinePipeline()
{
}

BloomCombinePipeline::~BloomCombinePipeline()
{
}

void BloomCombinePipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::vector<std::shared_ptr<RenderedColorTexture>> textureList)
{
    //std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;

    //VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    //vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    //vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    //vertShaderStageInfo.module = VulkanRenderer::CompileHLSLShader(File::OpenFile("EclipseEngine/AddP"), VK_SHADER_STAGE_VERTEX_BIT);
    //vertShaderStageInfo.pName = "main";

    //VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    //fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    //fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    //fragShaderStageInfo.module = VulkanRenderer::CompileHLSLShader(File::OpenFile("EclipseEngine/AddV"), VK_SHADER_STAGE_FRAGMENT_BIT);
    //fragShaderStageInfo.pName = "main";

    //PipelineShaderStageList.emplace_back(vertShaderStageInfo);
    //PipelineShaderStageList.emplace_back(fragShaderStageInfo);
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

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    AddTextureDescriptorSetBinding(DescriptorBindingList, 0, blurTextureBufferList);

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
    RenderPassInfo.ConstBufferSize = sizeof(BloomIndexSettings);

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

void BloomCombinePipeline::Draw(VkCommandBuffer& commandBuffer)
{
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
    vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(BloomIndexSettings), &bloomTextureCount);
    vkCmdDraw(commandBuffer, 6, 1, 0, 0);
}
