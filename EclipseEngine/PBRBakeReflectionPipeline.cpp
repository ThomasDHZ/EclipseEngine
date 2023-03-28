#include "PBRBakeReflectionPipeline.h"
#include "ConsoleLogger.h"

PBRBakeReflectionPipeline::PBRBakeReflectionPipeline()
{
}

PBRBakeReflectionPipeline::~PBRBakeReflectionPipeline()
{
}

void PBRBakeReflectionPipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, PBRRenderPassTextureSubmitList& textures)
{
    const auto& MeshList = MeshRendererManager::GetMeshList();
    std::vector<VkDescriptorBufferInfo> ReflectionBufferInfoList;
    VkDescriptorBufferInfo ReflectionBufferInfo = {};


    if (MeshList.size() == 0)
    {
        VkDescriptorBufferInfo nullBuffer;
        nullBuffer.buffer = VK_NULL_HANDLE;
        nullBuffer.offset = 0;
        nullBuffer.range = VK_WHOLE_SIZE;
        ReflectionBufferInfoList.emplace_back(nullBuffer);
    }
    else
    {
        for (int x = 0; x < MeshList.size(); x++)
        {
            cubeMapSamplerList.emplace_back(CubeMapSamplerBuffer());

            VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
            MeshPropertiesmBufferBufferInfo.buffer = cubeMapSamplerList[x].GetVulkanBufferData().Buffer;
            MeshPropertiesmBufferBufferInfo.offset = 0;
            MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
            ReflectionBufferInfoList.emplace_back(MeshPropertiesmBufferBufferInfo);
        }
    }

    std::vector<VkDescriptorBufferInfo> MeshPropertiesBufferList = MeshRendererManager::GetMeshPropertiesBuffer();
    std::vector<VkDescriptorBufferInfo> MaterialBufferList = MaterialManager::GetMaterialBufferList();
    std::vector<VkDescriptorBufferInfo> DirectionalLightBufferInfoList = GLTFSceneManager::GetDirectionalLightPropertiesBuffer();
    std::vector<VkDescriptorBufferInfo> PointLightBufferInfoList = GLTFSceneManager::GetPointLightPropertiesBuffer();
    std::vector<VkDescriptorBufferInfo> SpotLightBufferInfoList = GLTFSceneManager::GetSpotLightPropertiesBuffer();
    std::vector<VkDescriptorImageInfo> RenderedTextureBufferInfo = TextureManager::GetTexturemBufferList();

    std::vector<VkDescriptorImageInfo> IrradianceMapBufferList;
    if (textures.IrradianceTextureList.size() == 0)
    {
        VkDescriptorImageInfo nullBuffer;
        nullBuffer.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        nullBuffer.imageView = VK_NULL_HANDLE;
        nullBuffer.sampler = SceneManager::NullSampler;
        IrradianceMapBufferList.emplace_back(nullBuffer);
    }
    else
    {
        for (auto IrradianceMap : textures.IrradianceTextureList)
        {
            VkDescriptorImageInfo SkyboxBufferInfo;
            SkyboxBufferInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            SkyboxBufferInfo.imageView = IrradianceMap->View;
            SkyboxBufferInfo.sampler = IrradianceMap->Sampler;
            IrradianceMapBufferList.emplace_back(SkyboxBufferInfo);
        }
    }

    std::vector<VkDescriptorImageInfo> PrefilterMapBufferList;
    if (textures.IrradianceTextureList.size() == 0)
    {
        VkDescriptorImageInfo nullBuffer;
        nullBuffer.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        nullBuffer.imageView = VK_NULL_HANDLE;
        nullBuffer.sampler = SceneManager::NullSampler;
        PrefilterMapBufferList.emplace_back(nullBuffer);
    }
    else
    {
        for (auto PrefilterMap : textures.PrefilterTextureList)
        {
            VkDescriptorImageInfo SkyboxBufferInfo;
            SkyboxBufferInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            SkyboxBufferInfo.imageView = PrefilterMap->View;
            SkyboxBufferInfo.sampler = PrefilterMap->Sampler;
            PrefilterMapBufferList.emplace_back(SkyboxBufferInfo);
        }
    }

    VkDescriptorImageInfo BRDFBuffer;
    BRDFBuffer.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    BRDFBuffer.imageView = GLTFSceneManager::BRDFTexture->View;
    BRDFBuffer.sampler = GLTFSceneManager::BRDFTexture->Sampler;

    std::vector<VkDescriptorImageInfo> DirectionalLightShadowMaps;
    if (textures.DirectionalLightTextureShadowMaps.size() == 0)
    {
        VkDescriptorImageInfo nullBuffer;
        nullBuffer.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        nullBuffer.imageView = VK_NULL_HANDLE;
        nullBuffer.sampler = SceneManager::NullSampler;
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
        nullBuffer.sampler = SceneManager::NullSampler;
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
    //if (textures.SpotLightTextureShadowMaps.size() == 0)
    //{
    //    VkDescriptorImageInfo nullBuffer;
    //    nullBuffer.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    //    nullBuffer.imageView = VK_NULL_HANDLE;
    //    nullBuffer.sampler = NullSampler;
    //    SpotLightShadowMaps.emplace_back(nullBuffer);
    //}
    //else
    //{
    for (auto texture : textures.SpotLightTextureShadowMaps)
    {
        VkDescriptorImageInfo DescriptorImage{};
        DescriptorImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        DescriptorImage.imageView = texture->GetView();
        DescriptorImage.sampler = texture->GetSampler();
        SpotLightShadowMaps.emplace_back(DescriptorImage);
    }
    //}


    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "BakeReflectionPBRShaderVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "BakeReflectionPBRShaderFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    AddUniformBufferDescriptorSetBinding(DescriptorBindingList, 0, ReflectionBufferInfoList, VK_SHADER_STAGE_VERTEX_BIT);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 1, MeshPropertiesBufferList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 2, MaterialBufferList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 3, DirectionalLightBufferInfoList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 4, PointLightBufferInfoList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 5, SpotLightBufferInfoList);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 6, RenderedTextureBufferInfo, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 7, IrradianceMapBufferList, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 8, PrefilterMapBufferList, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 9, BRDFBuffer, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 10, DirectionalLightShadowMaps, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
    if (SpotLightShadowMaps.size() > 0)
    {
        AddTextureDescriptorSetBinding(DescriptorBindingList, 11, PointLightShadowMaps, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
    }

    VkPipelineDepthStencilStateCreateInfo DepthStencilStateCreateInfo{};
    DepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    DepthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
    DepthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
    DepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
    DepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
    DepthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;

    BuildVertexDescription VertexDescriptionInfo{};
    VertexDescriptionInfo.VertexBindingDescriptions = Vertex3D::getBindingDescriptions();
    VertexDescriptionInfo.VertexAttributeDescriptions = Vertex3D::getAttributeDescriptions();
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
    RenderPassInfo.ConstBufferSize = sizeof(BakeSceneProperties);

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

void PBRBakeReflectionPipeline::Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> drawMesh, std::shared_ptr<Mesh> reflectingMesh, uint32_t counter)
{
    if (reflectingMesh != nullptr &&
        drawMesh->GetMeshID() != reflectingMesh->GetMeshID())
    {
        SceneManager::sceneProperites.MeshIndex = drawMesh->GetMeshBufferIndex();
        SceneManager::sceneProperites.MeshColorID = Converter::PixelToVec3(drawMesh->GetMeshColorID());

        auto BakeProperties = BakeSceneProperties();
        BakeProperties.Counter = counter;
        BakeProperties.AmbientLight = SceneManager::sceneProperites.AmbientLight;
        BakeProperties.CameraPos = SceneManager::sceneProperites.CameraPos;
        BakeProperties.DirectionalLightCount = SceneManager::sceneProperites.DirectionalLightCount;
        BakeProperties.MeshColorID = SceneManager::sceneProperites.MeshColorID;
        BakeProperties.MeshIndex = SceneManager::sceneProperites.MeshIndex;
        BakeProperties.PBRMaxMipLevel = SceneManager::sceneProperites.PBRMaxMipLevel;
        BakeProperties.PointLightCount = SceneManager::sceneProperites.PointLightCount;
        BakeProperties.proj = SceneManager::sceneProperites.proj;
        BakeProperties.SpotLightCount = SceneManager::sceneProperites.SpotLightCount;
        BakeProperties.Timer = SceneManager::sceneProperites.Timer;
        BakeProperties.view = SceneManager::sceneProperites.view;


        const glm::vec3 reflectPos = reflectingMesh->reflectionPoint;
        glm::mat4 reflectionProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10000.0f);

        cubeMapSamplerList[counter].UniformDataInfo.CubeMapFaceMatrix[0] = reflectionProj * glm::lookAt(reflectPos, reflectPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        cubeMapSamplerList[counter].UniformDataInfo.CubeMapFaceMatrix[1] = reflectionProj * glm::lookAt(reflectPos, reflectPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        cubeMapSamplerList[counter].UniformDataInfo.CubeMapFaceMatrix[2] = reflectionProj * glm::lookAt(reflectPos, reflectPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        cubeMapSamplerList[counter].UniformDataInfo.CubeMapFaceMatrix[3] = reflectionProj * glm::lookAt(reflectPos, reflectPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
        cubeMapSamplerList[counter].UniformDataInfo.CubeMapFaceMatrix[4] = reflectionProj * glm::lookAt(reflectPos, reflectPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        cubeMapSamplerList[counter].UniformDataInfo.CubeMapFaceMatrix[5] = reflectionProj * glm::lookAt(reflectPos, reflectPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        cubeMapSamplerList[counter].Update();
        
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
        vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(BakeSceneProperties), &BakeProperties);
        drawMesh->Draw(commandBuffer);
    }
}

void PBRBakeReflectionPipeline::Destroy()
{
    for (auto& cubeMapSampler : cubeMapSamplerList)
    {
        cubeMapSampler.Destroy();
    }
    GraphicsPipeline::Destroy();
}