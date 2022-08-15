#include "PrefilterPipeline.h"

PrefilterPipeline::PrefilterPipeline()
{
}

PrefilterPipeline::~PrefilterPipeline()
{
}

void PrefilterPipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<RenderedCubeMapTexture> cubeMap)
{
    VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
    MeshPropertiesmBufferBufferInfo.buffer = cubeMapSampler.GetVulkanBufferData().Buffer;
    MeshPropertiesmBufferBufferInfo.offset = 0;
    MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;

    VkDescriptorImageInfo SkyboxBufferInfo;
    SkyboxBufferInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    SkyboxBufferInfo.imageView = cubeMap->View;
    SkyboxBufferInfo.sampler = cubeMap->Sampler;


    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "PrefilterShaderVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "PrefilterShaderFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    AddTextureDescriptorSetBinding(DescriptorBindingList, 0, SkyboxBufferInfo, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
    AddUniformBufferDescriptorSetBinding(DescriptorBindingList, 1, MeshPropertiesmBufferBufferInfo, VK_SHADER_STAGE_VERTEX_BIT);

    BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
    buildGraphicsPipelineInfo.ColorAttachments = pipelineInfoStruct.ColorAttachments;
    buildGraphicsPipelineInfo.DescriptorBindingList = DescriptorBindingList;
    buildGraphicsPipelineInfo.renderPass = pipelineInfoStruct.renderPass;
    buildGraphicsPipelineInfo.PipelineShaderStageList = PipelineShaderStageList;
    buildGraphicsPipelineInfo.sampleCount = pipelineInfoStruct.SampleCount;
    buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderPBRSkyBox;
    buildGraphicsPipelineInfo.ConstBufferSize = sizeof(PrefilterSkyboxSettings);
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

void PrefilterPipeline::Draw(VkCommandBuffer& commandBuffer, PrefilterSkyboxSettings& prefiliter, glm::vec3 CubeMapSamplerPos)
{
    glm::mat4 reflectionProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10000.0f);

    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[0] = reflectionProj * glm::lookAt(CubeMapSamplerPos, CubeMapSamplerPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[1] = reflectionProj * glm::lookAt(CubeMapSamplerPos, CubeMapSamplerPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[2] = reflectionProj * glm::lookAt(CubeMapSamplerPos, CubeMapSamplerPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[3] = reflectionProj * glm::lookAt(CubeMapSamplerPos, CubeMapSamplerPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[4] = reflectionProj * glm::lookAt(CubeMapSamplerPos, CubeMapSamplerPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[5] = reflectionProj * glm::lookAt(CubeMapSamplerPos, CubeMapSamplerPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    cubeMapSampler.Update();

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
    vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(prefiliter), &prefiliter);
    SceneManager::GetSkyboxMesh()->Draw(commandBuffer);
}

void PrefilterPipeline::Destroy()
{
    cubeMapSampler.Destroy();
    GraphicsPipeline::Destroy();
}
