#include "PBRReflectionPipeline.h"

PBRReflectionPipeline::PBRReflectionPipeline()
{
}

PBRReflectionPipeline::~PBRReflectionPipeline()
{
}

void PBRReflectionPipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, PBRRenderPassTextureSubmitList& textures)
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
    IrradianceMapBuffer.imageView = textures.IrradianceTexture->View;
    IrradianceMapBuffer.sampler = textures.IrradianceTexture->Sampler;

    VkDescriptorImageInfo PrefilterBuffer;
    PrefilterBuffer.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    PrefilterBuffer.imageView = textures.PrefilterTexture->View;
    PrefilterBuffer.sampler = textures.PrefilterTexture->Sampler;

    VkDescriptorImageInfo BRDFBuffer;
    BRDFBuffer.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    BRDFBuffer.imageView = SceneManager::BRDFTexture->View;
    BRDFBuffer.sampler = SceneManager::BRDFTexture->Sampler;

    std::vector<VkDescriptorImageInfo> DirectionalLightShadowMaps;
    if (textures.DirectionalLightTextureShadowMaps.size() == 0)
    {
        VkDescriptorImageInfo nullBuffer;
        nullBuffer.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        nullBuffer.imageView = VK_NULL_HANDLE;
        nullBuffer.sampler = NullSampler;
        DirectionalLightShadowMaps.emplace_back(nullBuffer);
    }
    else
    {
        for (auto texture : textures.DirectionalLightTextureShadowMaps)
        {
            VkDescriptorImageInfo DescriptorImage{};
            DescriptorImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            DescriptorImage.imageView = texture->GetView();
            DescriptorImage.sampler = texture->GetSampler();
            DirectionalLightShadowMaps.emplace_back(DescriptorImage);
        }
    }

    std::vector<VkDescriptorImageInfo> PointLightShadowMaps;
    if (textures.PointLightShadowMaps.size() == 0)
    {
        VkDescriptorImageInfo nullBuffer;
        nullBuffer.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        nullBuffer.imageView = VK_NULL_HANDLE;
        nullBuffer.sampler = NullSampler;
        PointLightShadowMaps.emplace_back(nullBuffer);
    }
    else
    {
        for (auto texture : textures.PointLightShadowMaps)
        {
            VkDescriptorImageInfo DescriptorImage{};
            DescriptorImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            DescriptorImage.imageView = texture->GetView();
            DescriptorImage.sampler = texture->GetSampler();
            PointLightShadowMaps.emplace_back(DescriptorImage);
        }
    }

    std::vector<VkDescriptorImageInfo> SpotLightShadowMaps;
    if (textures.SpotLightTextureShadowMaps.size() == 0)
    {
        VkDescriptorImageInfo nullBuffer;
        nullBuffer.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        nullBuffer.imageView = VK_NULL_HANDLE;
        nullBuffer.sampler = NullSampler;
        SpotLightShadowMaps.emplace_back(nullBuffer);
    }
    else
    {
        for (auto texture : textures.SpotLightTextureShadowMaps)
        {
            VkDescriptorImageInfo DescriptorImage{};
            DescriptorImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            DescriptorImage.imageView = texture->GetView();
            DescriptorImage.sampler = texture->GetSampler();
            SpotLightShadowMaps.emplace_back(DescriptorImage);
        }
    }

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
    AddTextureDescriptorSetBinding(DescriptorBindingList, 9, DirectionalLightShadowMaps, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);

    BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
    buildGraphicsPipelineInfo.ColorAttachments = pipelineInfoStruct.ColorAttachments;
    buildGraphicsPipelineInfo.DescriptorBindingList = DescriptorBindingList;
    buildGraphicsPipelineInfo.renderPass = pipelineInfoStruct.renderPass;
    buildGraphicsPipelineInfo.PipelineShaderStageList = PipelineShaderStageList;
    buildGraphicsPipelineInfo.sampleCount = pipelineInfoStruct.SampleCount;
    buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderMesh;
    buildGraphicsPipelineInfo.ConstBufferSize = sizeof(SceneProperties);
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

void PBRReflectionPipeline::Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh, glm::vec3 CubeMapSamplerPos)
{
    SceneManager::sceneProperites.MeshIndex = mesh->GetMeshBufferIndex();
    SceneManager::sceneProperites.MeshColorID = Converter::PixelToVec3(mesh->GetMeshColorID());

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
    vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SceneProperties), &SceneManager::sceneProperites);
    mesh->Draw(commandBuffer);
}

void PBRReflectionPipeline::Destroy()
{
    cubeMapSampler.Destroy();
    GraphicsPipeline::Destroy();
}