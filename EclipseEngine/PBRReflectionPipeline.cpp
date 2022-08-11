#include "PBRReflectionPipeline.h"

PBRReflectionPipeline::PBRReflectionPipeline()
{
}

PBRReflectionPipeline::~PBRReflectionPipeline()
{
}

void PBRReflectionPipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<RenderedCubeMapTexture> reflectionIrradianceMap, std::shared_ptr<RenderedCubeMapTexture> reflectionPrefilterMap)
{
    VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
    MeshPropertiesmBufferBufferInfo.buffer = cubeMapSampler.GetVulkanBufferData().Buffer;
    MeshPropertiesmBufferBufferInfo.offset = 0;
    MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;

    std::vector<VkDescriptorBufferInfo> MeshPropertiesBufferList = MeshRendererManager::GetMeshPropertiesBuffer();
    std::vector<VkDescriptorBufferInfo> DirectionalLightBufferInfoList = LightManager::GetDirectionalLightBuffer();
    std::vector<VkDescriptorBufferInfo> PointLightBufferInfoList = LightManager::GetPointLightBuffer();
    std::vector<VkDescriptorBufferInfo> SpotLightBufferInfoList = LightManager::GetSpotLightBuffer();
    std::vector<VkDescriptorImageInfo> RenderedTextureBufferInfo = TextureManager::GetTexturemBufferList();

    VkDescriptorImageInfo IrradianceMapBuffer;
    IrradianceMapBuffer.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    IrradianceMapBuffer.imageView = reflectionIrradianceMap->View;
    IrradianceMapBuffer.sampler = reflectionIrradianceMap->Sampler;

    VkDescriptorImageInfo PrefilterBuffer;
    PrefilterBuffer.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    PrefilterBuffer.imageView = reflectionPrefilterMap->View;
    PrefilterBuffer.sampler = reflectionPrefilterMap->Sampler;

    VkDescriptorImageInfo BRDFBuffer;
    BRDFBuffer.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    BRDFBuffer.imageView = SceneManager::BRDFTexture->View;
    BRDFBuffer.sampler = SceneManager::BRDFTexture->Sampler;


    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "ReflectionPBRShaderVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "ReflectionPBRShaderFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    AddUniformBufferDescriptorSetBinding(DescriptorBindingList, 0, MeshPropertiesmBufferBufferInfo, VK_SHADER_STAGE_VERTEX_BIT);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 1, MeshPropertiesBufferList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 2, DirectionalLightBufferInfoList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 3, PointLightBufferInfoList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 4, SpotLightBufferInfoList);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 5, RenderedTextureBufferInfo, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 6, IrradianceMapBuffer, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 7, PrefilterBuffer, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 8, BRDFBuffer, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);

    BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
    buildGraphicsPipelineInfo.ColorAttachments = pipelineInfoStruct.ColorAttachments;
    buildGraphicsPipelineInfo.DescriptorBindingList = DescriptorBindingList;
    buildGraphicsPipelineInfo.renderPass = pipelineInfoStruct.renderPass;
    buildGraphicsPipelineInfo.PipelineShaderStageList = PipelineShaderStageList;
    buildGraphicsPipelineInfo.sampleCount = pipelineInfoStruct.SampleCount;
    buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderMesh;
    buildGraphicsPipelineInfo.ConstBufferSize = sizeof(SceneProperties);

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

void PBRReflectionPipeline::Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh, glm::vec3 CubeMapSamplerPos)
{
    SceneManager::sceneProperites.MeshIndex = mesh->GetMeshBufferIndex();
    SceneManager::sceneProperites.MeshColorID = Converter::PixelToVec3(mesh->GetMeshColorID());

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
    vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SceneProperties), &SceneManager::sceneProperites);
    mesh->Draw(commandBuffer);
}
