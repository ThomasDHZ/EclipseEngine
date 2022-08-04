#include "SkyboxPipeline.h"

SkyboxPipeline::SkyboxPipeline()
{
}

SkyboxPipeline::~SkyboxPipeline()
{
}

void SkyboxPipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<CubeMapTexture> cubemap)
{
    VkDescriptorImageInfo SkyboxBufferInfo;
    SkyboxBufferInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    SkyboxBufferInfo.imageView = cubemap->View;
    SkyboxBufferInfo.sampler = cubemap->Sampler;

    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "CubeMapVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "CubeMapFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    AddTextureDescriptorSetBinding(DescriptorBindingList, 0, SkyboxBufferInfo, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);

    BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
    buildGraphicsPipelineInfo.ColorAttachments = pipelineInfoStruct.ColorAttachments;
    buildGraphicsPipelineInfo.DescriptorBindingList = DescriptorBindingList;
    buildGraphicsPipelineInfo.renderPass = pipelineInfoStruct.renderPass;
    buildGraphicsPipelineInfo.PipelineShaderStageList = PipelineShaderStageList;
    buildGraphicsPipelineInfo.sampleCount = pipelineInfoStruct.SampleCount;
    buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderSkybox;
    buildGraphicsPipelineInfo.ConstBufferSize = sizeof(ConstSkyBoxView);
    buildGraphicsPipelineInfo.VertexDescriptorType = VertexDescriptorTypeEnum::kVertex3D;

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

void SkyboxPipeline::Draw(VkCommandBuffer& commandBuffer)
{
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
    vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(ConstSkyBoxView), &SceneManager::cubeMapInfo);
    SceneManager::GetSkyboxMesh()->Draw(commandBuffer);
}
