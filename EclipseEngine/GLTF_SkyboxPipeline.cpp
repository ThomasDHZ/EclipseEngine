#include "GLTF_SkyboxPipeline.h"
#include "JsonGraphicsPipeline.h"

GLTF_SkyboxPipeline::GLTF_SkyboxPipeline()
{
}

GLTF_SkyboxPipeline::~GLTF_SkyboxPipeline()
{
}

void GLTF_SkyboxPipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<CubeMapTexture> cubemap)
{
    VkDescriptorImageInfo SkyboxBufferInfo;
    SkyboxBufferInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    SkyboxBufferInfo.imageView = cubemap->View;
    SkyboxBufferInfo.sampler = cubemap->Sampler;


    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    auto a = CreateShader(BaseShaderFilePath + "CubeMapVert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    auto b = CreateShader(BaseShaderFilePath + "CubeMapFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

    nlohmann::json json;
    JsonGraphicsPipeline jsonPipeline{};
    JsonConverter::to_json(json["CubeMapShader"], true);
    jsonPipeline.SavePipelineShaderStageCreateInfo(json["Shader"][0], a, BaseShaderFilePath + "CubeMapVert.spv");
    jsonPipeline.SavePipelineShaderStageCreateInfo(json["Shader"][1], b, BaseShaderFilePath + "CubeMapFrag.spv");

    for (int x = 0; x < json["Shader"].size(); x++)
    {
        PipelineShaderStageList.emplace_back(jsonPipeline.LoadPipelineShaderStageCreateInfo(json["Shader"][x]));
    }

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 0, SkyboxBufferInfo, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
    DescriptorSet = GLTF_GraphicsDescriptors::SubmitDescriptorSet(DescriptorBindingList);
    DescriptorSetLayout = GLTF_GraphicsDescriptors::SubmitDescriptorSetLayout(DescriptorBindingList);

    jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][0], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, DescriptorBindingPropertiesEnum::kCubeMapDescriptor);


    VkPipelineDepthStencilStateCreateInfo DepthStencilStateCreateInfo{};
    DepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    DepthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
    DepthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
    DepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
    DepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
    DepthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    jsonPipeline.SavePipelineDepthStencilStateCreateInfo(json["PipelineDepthStencilStateCreateInfo"], DepthStencilStateCreateInfo);

    BuildVertexDescription VertexDescriptionInfo{};
    VertexDescriptionInfo.VertexBindingDescriptions = SkyboxVertexLayout::getBindingDescriptions();
    VertexDescriptionInfo.VertexAttributeDescriptions = SkyboxVertexLayout::getAttributeDescriptions();
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
    RenderPassInfo.ConstBufferSize = sizeof(SkyBoxView);

    BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
    buildGraphicsPipelineInfo.VertexDescription = VertexDescriptionInfo;
    buildGraphicsPipelineInfo.RenderPassDescription = RenderPassInfo;

    if (ShaderPipeline == nullptr)
    {
        jsonPipeline.BuildAndSaveShaderPipeLine(json, buildGraphicsPipelineInfo, DescriptorSetLayout);
        jsonPipeline.SaveGraphicsPipeline("SkyBoxPipeline.txt", json);
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

void GLTF_SkyboxPipeline::Draw(VkCommandBuffer& commandBuffer)
{
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
    GLTFSceneManager::SkyboxMesh->Draw(commandBuffer, ShaderPipelineLayout, DescriptorSet);
}
