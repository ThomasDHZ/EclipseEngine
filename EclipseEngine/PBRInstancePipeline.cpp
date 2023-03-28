#include "PBRInstancePipeline.h"

PBRInstancePipeline::PBRInstancePipeline()
{
}

PBRInstancePipeline::~PBRInstancePipeline()
{
}

void PBRInstancePipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, PBRRenderPassTextureSubmitList& textures)
{
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
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "PBRInstanceRendererVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "PBRInstanceRendererFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, MeshPropertiesBufferList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 1, MaterialBufferList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 2, DirectionalLightBufferInfoList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 3, PointLightBufferInfoList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 4, SpotLightBufferInfoList);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 5, RenderedTextureBufferInfo, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 6, IrradianceMapBufferList, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 7, PrefilterMapBufferList, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 8, BRDFBuffer, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 9, DirectionalLightShadowMaps, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
    if (SpotLightShadowMaps.size() > 0)
    {
        AddTextureDescriptorSetBinding(DescriptorBindingList, 9, PointLightShadowMaps, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
    }

    VkPipelineDepthStencilStateCreateInfo DepthStencilStateCreateInfo{};
    DepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    DepthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
    DepthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
    DepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
    DepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
    DepthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;

    std::vector<VkVertexInputBindingDescription> bindingDescriptions;
    bindingDescriptions = Vertex3D::getBindingDescriptions();
    bindingDescriptions.emplace_back(InstancedData3D::getBindingDescriptions()[0]);

    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
    attributeDescriptions = Vertex3D::getAttributeDescriptions();
    attributeDescriptions = InstancedData3D::AddInstnacingAttributeDescription(attributeDescriptions);

    BuildVertexDescription VertexDescriptionInfo{};
    VertexDescriptionInfo.VertexBindingDescriptions = bindingDescriptions;
    VertexDescriptionInfo.VertexAttributeDescriptions = attributeDescriptions;
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
    RenderPassInfo.ConstBufferSize = sizeof(SceneProperties);

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

void PBRInstancePipeline::Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh)
{
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
    vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SceneProperties), &SceneManager::sceneProperites);
    mesh->InstanceDraw(commandBuffer);
}