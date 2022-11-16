#include "IrradiancePipeline.h"
IrradiancePipeline::IrradiancePipeline()
{
}

IrradiancePipeline::~IrradiancePipeline()
{
}

void IrradiancePipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::vector<std::shared_ptr<RenderedCubeMapTexture>> cubeMapList)
{


    std::vector<VkDescriptorImageInfo> SkyboxBufferInfoList;
    if (cubeMapList.size() == 0)
    {
        VkDescriptorImageInfo nullBuffer;
        nullBuffer.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        nullBuffer.imageView = VK_NULL_HANDLE;
        nullBuffer.sampler = SceneManager::NullSampler;
        SkyboxBufferInfoList.emplace_back(nullBuffer);
    }
    else
    {
        for (auto cubeMap : cubeMapList)
        {
            VkDescriptorImageInfo SkyboxBufferInfo;
            SkyboxBufferInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            SkyboxBufferInfo.imageView = cubeMap->View;
            SkyboxBufferInfo.sampler = cubeMap->Sampler;
            SkyboxBufferInfoList.emplace_back(SkyboxBufferInfo);
        }
    }

    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "IrradianceShaderVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "IrradianceShaderFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    AddTextureDescriptorSetBinding(DescriptorBindingList, 0, SkyboxBufferInfoList, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);

    //BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
    //buildGraphicsPipelineInfo.ColorAttachments = pipelineInfoStruct.ColorAttachments;
    //buildGraphicsPipelineInfo.DescriptorBindingList = DescriptorBindingList;
    //buildGraphicsPipelineInfo.renderPass = pipelineInfoStruct.renderPass;
    //buildGraphicsPipelineInfo.PipelineShaderStageList = PipelineShaderStageList;
    //buildGraphicsPipelineInfo.sampleCount = pipelineInfoStruct.SampleCount;
    //buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderPBRSkyBox;
    //buildGraphicsPipelineInfo.ConstBufferSize = sizeof(IrradianceSkyboxSettings);
    //buildGraphicsPipelineInfo.VertexDescriptorType = VertexDescriptorTypeEnum::kVertex3D;


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

void IrradiancePipeline::Draw(VkCommandBuffer& commandBuffer, IrradianceSkyboxSettings& irradiance)
{
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
    vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(IrradianceSkyboxSettings), &irradiance);
    SceneManager::GetSkyboxMesh()->Draw(commandBuffer);
}