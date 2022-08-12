#include "CubeMapSamplerPipeline.h"

CubeMapSamplerPipeline::CubeMapSamplerPipeline()
{
}

CubeMapSamplerPipeline::~CubeMapSamplerPipeline()
{
}

void CubeMapSamplerPipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<CubeMapTexture> cubemap)
{
    VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
    MeshPropertiesmBufferBufferInfo.buffer = cubeMapSampler.GetVulkanBufferData().Buffer;
    MeshPropertiesmBufferBufferInfo.offset = 0;
    MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;

    VkDescriptorImageInfo SkyboxBufferInfo;
    SkyboxBufferInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    SkyboxBufferInfo.imageView = cubemap->View;
    SkyboxBufferInfo.sampler = cubemap->Sampler;

    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "CubeMapSamplerVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "CubeMapSamplerFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

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
    buildGraphicsPipelineInfo.ConstBufferSize = 0;
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

void CubeMapSamplerPipeline::Draw(VkCommandBuffer& commandBuffer)
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

    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[0] = MVP[0];
    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[1] = MVP[1];
    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[2] = MVP[2];
    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[3] = MVP[3];
    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[4] = MVP[4];
    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[5] = MVP[5];
    cubeMapSampler.Update();

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
    SceneManager::GetSkyboxMesh()->Draw(commandBuffer);
}
