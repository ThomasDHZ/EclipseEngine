#include "PBRInstancePipeline.h"
#include "JsonGraphicsPipeline.h"

PBRInstancePipeline::PBRInstancePipeline()
{
}

PBRInstancePipeline::~PBRInstancePipeline()
{
}

void PBRInstancePipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct)
{
    //std::vector<VkDescriptorBufferInfo> MeshPropertiesBufferList = MeshRendererManager::GetMeshPropertiesBuffer();
    //std::vector<VkDescriptorBufferInfo> MaterialBufferList = MaterialManager::GetMaterialBufferList();
    std::vector<VkDescriptorBufferInfo> DirectionalLightBufferInfoList = GLTFSceneManager::GetDirectionalLightPropertiesBuffer();
    std::vector<VkDescriptorBufferInfo> PointLightBufferInfoList = GLTFSceneManager::GetPointLightPropertiesBuffer();
    std::vector<VkDescriptorBufferInfo> SpotLightBufferInfoList = GLTFSceneManager::GetSpotLightPropertiesBuffer();
   // std::vector<VkDescriptorImageInfo> RenderedTextureBufferInfo = TextureManager::GetTexturemBufferList();

    //std::vector<VkDescriptorImageInfo> IrradianceMapBufferList;
    //if (textures.IrradianceTextureList.size() == 0)
    //{
    //    VkDescriptorImageInfo nullBuffer;
    //    nullBuffer.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    //    nullBuffer.imageView = VK_NULL_HANDLE;
    //    nullBuffer.sampler = SceneManager::NullSampler;
    //    IrradianceMapBufferList.emplace_back(nullBuffer);
    //}
    //else
    //{
    //    for (auto IrradianceMap : textures.IrradianceTextureList)
    //    {
    //        VkDescriptorImageInfo SkyboxBufferInfo;
    //        SkyboxBufferInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //        SkyboxBufferInfo.imageView = IrradianceMap->View;
    //        SkyboxBufferInfo.sampler = IrradianceMap->Sampler;
    //        IrradianceMapBufferList.emplace_back(SkyboxBufferInfo);
    //    }
    //}

    //std::vector<VkDescriptorImageInfo> PrefilterMapBufferList;
    //if (textures.IrradianceTextureList.size() == 0)
    //{
    //    VkDescriptorImageInfo nullBuffer;
    //    nullBuffer.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    //    nullBuffer.imageView = VK_NULL_HANDLE;
    //    nullBuffer.sampler = SceneManager::NullSampler;
    //    PrefilterMapBufferList.emplace_back(nullBuffer);
    //}
    //else
    //{
    //    for (auto PrefilterMap : textures.PrefilterTextureList)
    //    {
    //        VkDescriptorImageInfo SkyboxBufferInfo;
    //        SkyboxBufferInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //        SkyboxBufferInfo.imageView = PrefilterMap->View;
    //        SkyboxBufferInfo.sampler = PrefilterMap->Sampler;
    //        PrefilterMapBufferList.emplace_back(SkyboxBufferInfo);
    //    }
    //}

    //VkDescriptorImageInfo BRDFBuffer;
    //BRDFBuffer.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //BRDFBuffer.imageView = GLTFSceneManager::BRDFTexture->View;
    //BRDFBuffer.sampler = GLTFSceneManager::BRDFTexture->Sampler;

    //std::vector<VkDescriptorImageInfo> DirectionalLightShadowMaps;
    //if (textures.DirectionalLightTextureShadowMaps.size() == 0)
    //{
    //    VkDescriptorImageInfo nullBuffer;
    //    nullBuffer.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    //    nullBuffer.imageView = VK_NULL_HANDLE;
    //    nullBuffer.sampler = SceneManager::NullSampler;
    //    DirectionalLightShadowMaps.emplace_back(nullBuffer);
    //}
    //else
    //{
    //    for (auto texture : textures.DirectionalLightTextureShadowMaps)
    //    {
    //        VkDescriptorImageInfo DescriptorImage{};
    //        DescriptorImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //        DescriptorImage.imageView = texture->GetView();
    //        DescriptorImage.sampler = texture->GetSampler();
    //        DirectionalLightShadowMaps.emplace_back(DescriptorImage);
    //    }
    //}


    //std::vector<VkDescriptorImageInfo> PointLightShadowMaps;
    //if (textures.PointLightShadowMaps.size() == 0)
    //{
    //    VkDescriptorImageInfo nullBuffer;
    //    nullBuffer.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    //    nullBuffer.imageView = VK_NULL_HANDLE;
    //    nullBuffer.sampler = SceneManager::NullSampler;
    //    PointLightShadowMaps.emplace_back(nullBuffer);
    //}
    //else
    //{
    //    for (auto texture : textures.PointLightShadowMaps)
    //    {
    //        VkDescriptorImageInfo DescriptorImage{};
    //        DescriptorImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //        DescriptorImage.imageView = texture->GetView();
    //        DescriptorImage.sampler = texture->GetSampler();
    //        PointLightShadowMaps.emplace_back(DescriptorImage);
    //    }
    //}

    //std::vector<VkDescriptorImageInfo> SpotLightShadowMaps;
    ////if (textures.SpotLightTextureShadowMaps.size() == 0)
    ////{
    ////    VkDescriptorImageInfo nullBuffer;
    ////    nullBuffer.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    ////    nullBuffer.imageView = VK_NULL_HANDLE;
    ////    nullBuffer.sampler = NullSampler;
    ////    SpotLightShadowMaps.emplace_back(nullBuffer);
    ////}
    ////else
    ////{
    //for (auto texture : textures.SpotLightTextureShadowMaps)
    //{
    //    VkDescriptorImageInfo DescriptorImage{};
    //    DescriptorImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //    DescriptorImage.imageView = texture->GetView();
    //    DescriptorImage.sampler = texture->GetSampler();
    //    SpotLightShadowMaps.emplace_back(DescriptorImage);
    //}
    //}

    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    auto a = CreateShader(BaseShaderFilePath + "PBRInstanceRendererVert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    auto b = CreateShader(BaseShaderFilePath + "PBRInstanceRendererFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

    nlohmann::json json;
    JsonGraphicsPipeline jsonPipeline{};

    jsonPipeline.SavePipelineShaderStageCreateInfo(json["Shader"][0], a, BaseShaderFilePath + "PBRInstanceRendererVert.spv");
    jsonPipeline.SavePipelineShaderStageCreateInfo(json["Shader"][1], b, BaseShaderFilePath + "PBRInstanceRendererFrag.spv");

    for (int x = 0; x < json["Shader"].size(); x++)
    {
        PipelineShaderStageList.emplace_back(jsonPipeline.LoadPipelineShaderStageCreateInfo(json["Shader"][x]));
    }

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    //AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, MeshPropertiesBufferList);
   // AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 1, MaterialBufferList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 2, DirectionalLightBufferInfoList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 3, PointLightBufferInfoList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 4, SpotLightBufferInfoList);
  //  AddTextureDescriptorSetBinding(DescriptorBindingList, 5, RenderedTextureBufferInfo, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR);
    //AddTextureDescriptorSetBinding(DescriptorBindingList, 6, IrradianceMapBufferList, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
    //AddTextureDescriptorSetBinding(DescriptorBindingList, 7, PrefilterMapBufferList, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
    //AddTextureDescriptorSetBinding(DescriptorBindingList, 8, BRDFBuffer, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
    //AddTextureDescriptorSetBinding(DescriptorBindingList, 9, DirectionalLightShadowMaps, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);

    jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][0], VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, DescriptorBindingPropertiesEnum::kMeshPropertiesDescriptor);
    jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][1], VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, DescriptorBindingPropertiesEnum::kModelTransformDescriptor);
    jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][2], VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, DescriptorBindingPropertiesEnum::kMaterialDescriptor);
    jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][3], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, DescriptorBindingPropertiesEnum::kTextureDescriptor);
    jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][4], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, DescriptorBindingPropertiesEnum::kBRDFMapDescriptor);
    jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][5], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, DescriptorBindingPropertiesEnum::kIrradianceMapDescriptor);
    jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][6], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, DescriptorBindingPropertiesEnum::kPrefilterMapDescriptor);
    jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][7], VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, DescriptorBindingPropertiesEnum::kSunLightDescriptor);
    jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][8], VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, DescriptorBindingPropertiesEnum::kDirectionalLightDescriptor);
    jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][9], VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, DescriptorBindingPropertiesEnum::kPointLightDescriptor);
    jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][10], VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, DescriptorBindingPropertiesEnum::kSpotLightDescriptor);

    const std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions = Vertex3D::getBindingDescriptions();
    const std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions = Vertex3D::getAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(VertexBindingDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = VertexBindingDescriptions.data();
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(VertexAttributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = VertexAttributeDescriptions.data();

    VkPipelineDepthStencilStateCreateInfo saveDepthStencilStateCreateInfo{};
    saveDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    saveDepthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
    saveDepthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
    saveDepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
    saveDepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
    saveDepthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    jsonPipeline.SavePipelineDepthStencilStateCreateInfo(json["PipelineDepthStencilStateCreateInfo"], saveDepthStencilStateCreateInfo);

    VkPipelineInputAssemblyStateCreateInfo saveinputAssembly = {};
    saveinputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    saveinputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    saveinputAssembly.primitiveRestartEnable = VK_FALSE;
    jsonPipeline.SavePipelineInputAssemblyStateCreateInfo(json["PipelineInputAssemblyStateCreateInfo"], saveinputAssembly);

    VkPipelineViewportStateCreateInfo saveviewportState{};
    saveviewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    saveviewportState.scissorCount = 0;
    saveviewportState.viewportCount = 0;
    jsonPipeline.SavePipelineViewportStateCreateInfo(json["PipelineViewportStateCreateInfo"], saveviewportState);

    VkPipelineRasterizationStateCreateInfo saverasterizer{};
    saverasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    saverasterizer.depthClampEnable = VK_FALSE;
    saverasterizer.rasterizerDiscardEnable = VK_FALSE;
    saverasterizer.lineWidth = 1.0f;
    saverasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    saverasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    saverasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    saverasterizer.depthBiasEnable = VK_FALSE;
    jsonPipeline.SavePipelineRasterizationStateCreateInfo(json["PipelineRasterizationStateCreateInfo"], saverasterizer);

    VkPipelineMultisampleStateCreateInfo savemultisampling{};
    savemultisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    savemultisampling.sampleShadingEnable = VK_TRUE;
    savemultisampling.rasterizationSamples = pipelineInfoStruct.SampleCount;
    jsonPipeline.SavePipelineMultisampleStateCreateInfo(json["PipelineMultisampleStateCreateInfo"], savemultisampling);

    VkPipelineColorBlendStateCreateInfo savecolorBlending{};
    savecolorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    savecolorBlending.attachmentCount = static_cast<uint32_t>(pipelineInfoStruct.ColorAttachments.size());
    savecolorBlending.pAttachments = pipelineInfoStruct.ColorAttachments.data();
    jsonPipeline.SavePipelineColorBlendStateCreateInfo(json["PipelineColorBlendStateCreateInfo"], savecolorBlending);

    std::cout << json << std::endl;
    jsonPipeline.SaveGraphicsPipeline("PBRInstancePipeline.txt", json);
}

//void PBRInstancePipeline::Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh)
//{
//    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
//    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
//    vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SceneProperties), &GLTFSceneManager::sceneProperites);
//    mesh->InstanceDraw(commandBuffer);
//}