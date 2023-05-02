#include "WireFramePipeline.h"
#include "JsonGraphicsPipeline.h"

WireFramePipeline::WireFramePipeline()
{
}

WireFramePipeline::~WireFramePipeline()
{
}

void WireFramePipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct)
{
  //  std::vector<VkDescriptorBufferInfo> MeshPropertiesmBufferList = MeshRendererManager::GetMeshPropertiesBuffer();

    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    auto a = CreateShader(BaseShaderFilePath + "WireFrameShaderVert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    auto b = CreateShader(BaseShaderFilePath + "WireFrameShaderFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

    nlohmann::json json;
    nlohmann::json json2;
    JsonGraphicsPipeline jsonPipeline{};
    jsonPipeline.SavePipelineShaderStageCreateInfo(json["Shader"][0], a, BaseShaderFilePath + "WireFrameShaderVert.spv");
    jsonPipeline.SavePipelineShaderStageCreateInfo(json["Shader"][1], b, BaseShaderFilePath + "WireFrameShaderFrag.spv");

    for (int x = 0; x < json["Shader"].size(); x++)
    {
        PipelineShaderStageList.emplace_back(jsonPipeline.LoadPipelineShaderStageCreateInfo(json["Shader"][x]));
    }

    JsonGraphicsPipeline jsonPipeline2{};
    jsonPipeline2.SavePipelineShaderStageCreateInfo(json2["Shader"][0], a, BaseShaderFilePath + "InstanceWireFrameVert.spv");
    jsonPipeline2.SavePipelineShaderStageCreateInfo(json2["Shader"][1], b, BaseShaderFilePath + "InstanceWireFrameFrag.spv");

    for (int x = 0; x < json["Shader"].size(); x++)
    {
        PipelineShaderStageList.emplace_back(jsonPipeline2.LoadPipelineShaderStageCreateInfo(json2["Shader"][x]));
    }

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
   // AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, MeshPropertiesmBufferList, VK_SHADER_STAGE_VERTEX_BIT);

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

    VkPipelineDepthStencilStateCreateInfo DepthStencilStateCreateInfo{};
    DepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    DepthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
    DepthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
    DepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
    DepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
    DepthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    jsonPipeline.SavePipelineDepthStencilStateCreateInfo(json["PipelineDepthStencilStateCreateInfo"], DepthStencilStateCreateInfo);
    jsonPipeline.SavePipelineDepthStencilStateCreateInfo(json2["PipelineDepthStencilStateCreateInfo"], DepthStencilStateCreateInfo);

    BuildVertexDescription VertexDescriptionInfo{};
    VertexDescriptionInfo.VertexBindingDescriptions = Vertex3D::getBindingDescriptions();
    VertexDescriptionInfo.VertexAttributeDescriptions = Vertex3D::getAttributeDescriptions();
    VertexDescriptionInfo.VertexTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    VertexDescriptionInfo.PolygonMode = VK_POLYGON_MODE_LINE;
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
       // CreateGraphicsPipeline(buildGraphicsPipelineInfo);
        jsonPipeline.BuildAndSaveShaderPipeLine(json, buildGraphicsPipelineInfo, DescriptorSetLayout);
        jsonPipeline.BuildAndSaveShaderPipeLine(json2, buildGraphicsPipelineInfo, DescriptorSetLayout);
        jsonPipeline.SaveGraphicsPipeline("WireframePipeline.txt", json);
        jsonPipeline.SaveGraphicsPipeline("WireframeInstancePipeline.txt", json2);
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

//void WireFramePipeline::Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh)
//{
//    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
//    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
//    vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SceneProperties), &GLTFSceneManager::sceneProperites);
//    mesh->Draw(commandBuffer);
//}
