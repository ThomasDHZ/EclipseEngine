#include "Renderer2DPipeline.h"
#include "JsonGraphicsPipeline.h"

Renderer2DPipeline::Renderer2DPipeline()
{
}

Renderer2DPipeline::~Renderer2DPipeline()
{
}

void Renderer2DPipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct)
{
   // std::vector<VkDescriptorBufferInfo> MeshPropertiesmBufferList = MeshRendererManager::GetMeshPropertiesBuffer();
   // std::vector<VkDescriptorBufferInfo> MaterialBufferList = MaterialManager::GetMaterialBufferList();
   // std::vector<VkDescriptorImageInfo> RenderedTextureBufferInfo = TextureManager::GetTexturemBufferList();

    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    auto a = CreateShader(BaseShaderFilePath + "Renderer2DVert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    auto b = CreateShader(BaseShaderFilePath + "Renderer2DFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

    nlohmann::json json;
    JsonGraphicsPipeline jsonPipeline{};
    JsonConverter::to_json(json["CubeMapShader"], true);
    jsonPipeline.SavePipelineShaderStageCreateInfo(json["Shader"][0], a, BaseShaderFilePath + "Renderer2DVert.spv");
    jsonPipeline.SavePipelineShaderStageCreateInfo(json["Shader"][1], b, BaseShaderFilePath + "Renderer2DFrag.spv");

    for (int x = 0; x < json["Shader"].size(); x++)
    {
        PipelineShaderStageList.emplace_back(jsonPipeline.LoadPipelineShaderStageCreateInfo(json["Shader"][x]));
    }

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
 //   AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, MeshPropertiesmBufferList);
 ////   AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 1, MaterialBufferList);
 //   AddTextureDescriptorSetBinding(DescriptorBindingList, 2, RenderedTextureBufferInfo, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR);
    jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][0], VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, DescriptorBindingPropertiesEnum::kModelTransformDescriptor);
    jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][1], VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, DescriptorBindingPropertiesEnum::kMeshPropertiesDescriptor);
    //jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][2], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, DescriptorBindingPropertiesEnum::kAlbedoMapDescriptor);
    //jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][3], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, DescriptorBindingPropertiesEnum::kNormalMapDescriptor);
    //jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][4], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, DescriptorBindingPropertiesEnum::kMetallicRoughnessMapDescriptor);
    //jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][5], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, DescriptorBindingPropertiesEnum::kAmbientOcclusionMapDescriptor);
    //jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][6], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, DescriptorBindingPropertiesEnum::kAlphaMapDescriptor);
    //jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][7], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, DescriptorBindingPropertiesEnum::kDepthMapDescriptor);
    jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][8], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, DescriptorBindingPropertiesEnum::kBRDFMapDescriptor);
    jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][9], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, DescriptorBindingPropertiesEnum::kIrradianceMapDescriptor);
    jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][10], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, DescriptorBindingPropertiesEnum::kPrefilterMapDescriptor);
    jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][11], VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, DescriptorBindingPropertiesEnum::kSunLightDescriptor);
    jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][12], VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, DescriptorBindingPropertiesEnum::kDirectionalLightDescriptor);
    jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][13], VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, DescriptorBindingPropertiesEnum::kPointLightDescriptor);
    jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][14], VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, DescriptorBindingPropertiesEnum::kSpotLightDescriptor);


    VkPipelineDepthStencilStateCreateInfo DepthStencilStateCreateInfo{};
    DepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    DepthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
    DepthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
    DepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
    DepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
    DepthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    jsonPipeline.SavePipelineDepthStencilStateCreateInfo(json["PipelineDepthStencilStateCreateInfo"], DepthStencilStateCreateInfo);

    BuildVertexDescription VertexDescriptionInfo{};
    VertexDescriptionInfo.VertexBindingDescriptions = Vertex2D::getBindingDescriptions();
    VertexDescriptionInfo.VertexAttributeDescriptions = Vertex2D::getAttributeDescriptions();
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
        jsonPipeline.BuildAndSaveShaderPipeLine(json, buildGraphicsPipelineInfo, DescriptorSetLayout);
        jsonPipeline.SaveGraphicsPipeline("Renderer2DPipeline.txt", json);
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

//void Renderer2DPipeline::Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh)
//{
//    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
//    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
//    vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SceneProperties), &GLTFSceneManager::sceneProperites);
//    mesh->Draw(commandBuffer);
//}
