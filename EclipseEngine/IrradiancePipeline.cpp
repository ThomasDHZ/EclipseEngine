#include "IrradiancePipeline.h"
IrradiancePipeline::IrradiancePipeline()
{
}

IrradiancePipeline::~IrradiancePipeline()
{
}

void IrradiancePipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<RenderedCubeMapTexture> cubeMap)
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
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "IrradianceShaderVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "IrradianceShaderFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

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
    buildGraphicsPipelineInfo.ConstBufferSize = sizeof(IrradianceSkyboxSettings);
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

void IrradiancePipeline::Draw(VkCommandBuffer& commandBuffer, IrradianceSkyboxSettings& irradiance, glm::vec3 CubeMapSamplerPos)
{
    glm::mat4 MVP[6] = { {{0.000000, 0.000000, 1.010101, 1.000000},
                   {0.000000, -1.000000, 0.000000, 0.000000},
                   {-1.000000, 0.000000, 0.000000, 0.000000},
                   {0.000000, 0.000000, -0.101010, 0.000000}},
                  {{0.000000, 0.000000, -1.010101, -1.000000},
                   {0.000000, -1.000000, 0.000000, 0.000000},
                   {1.000000, 0.000000, 0.000000, 0.000000},
                   {0.000000, 0.000000, -0.101010, 0.000000}},
                  {{1.000000, 0.000000, 0.000000, 0.000000},
                   {0.000000, 0.000000, 1.010101, 1.000000},
                   {0.000000, 1.000000, 0.000000, 0.000000},
                   {0.000000, 0.000000, -0.101010, 0.000000}},
                  {{1.000000, 0.000000, 0.000000, 0.000000},
                   {0.000000, 0.000000, -1.010101, -1.000000},
                   {0.000000, -1.000000, 0.000000, 0.000000},
                   {0.000000, 0.000000, -0.101010, 0.000000}},
                  {{1.000000, 0.000000, 0.000000, 0.000000},
                   {0.000000, -1.000000, 0.000000, 0.000000},
                   {0.000000, 0.000000, 1.010101, 1.000000},
                   {0.000000, 0.000000, -0.101010, 0.000000}},
                  {{-1.000000, 0.000000, 0.000000, 0.000000},
                   {0.000000, -1.000000, 0.000000, 0.000000},
                   {0.000000, 0.000000, -1.010101, -1.000000},
                   {0.000000, 0.000000, -0.101010, 0.000000}} };


    glm::mat4 reflectionProj = glm::ortho(-10000.0f, 10000.0f, -10000.0f, 10000.0f, -10000.0f, 10000.0f);
    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[0] = MVP[0];
    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[1] = MVP[1];
    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[2] = MVP[2];
    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[3] = MVP[3];
    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[4] = MVP[4];
    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[5] = MVP[5];
    cubeMapSampler.Update();

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
    vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(IrradianceSkyboxSettings), &irradiance);
    SceneManager::GetSkyboxMesh()->Draw(commandBuffer);
}