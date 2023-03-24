#include "DepthCubeMapPipeline.h"

DepthCubeMapPipeline::DepthCubeMapPipeline()
{
}

DepthCubeMapPipeline::~DepthCubeMapPipeline()
{
}

void DepthCubeMapPipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct)
{
    std::vector<VkDescriptorBufferInfo> MeshPropertiesBufferList = MeshRendererManager::GetMeshPropertiesBuffer();
    std::vector<VkDescriptorBufferInfo> MaterialBufferList = MaterialManager::GetMaterialBufferList();
    std::vector<VkDescriptorBufferInfo> DirectionalLightBufferInfoList = GLTFSceneManager::GetDirectionalLightPropertiesBuffer();
    std::vector<VkDescriptorImageInfo> RenderedTextureBufferInfo = TextureManager::GetTexturemBufferList();

    VkDescriptorBufferInfo CubeMapSamplerBufferInfo = {};
    CubeMapSamplerBufferInfo.buffer = cubeMapSampler.GetVulkanBufferData().Buffer;
    CubeMapSamplerBufferInfo.offset = 0;
    CubeMapSamplerBufferInfo.range = VK_WHOLE_SIZE;

    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "DepthCubeShaderVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "DepthCubeShaderFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    AddUniformBufferDescriptorSetBinding(DescriptorBindingList, 0, CubeMapSamplerBufferInfo, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 1, MeshPropertiesBufferList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 2, MaterialBufferList);
 //  AddTextureDescriptorSetBinding(DescriptorBindingList, 3, RenderedTextureBufferInfo, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR);

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

void DepthCubeMapPipeline::Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh, glm::vec3 CubeMapSamplerPos, uint32_t x)
{
    DirectionalLightProjection directionalLightProjection;
    directionalLightProjection.MeshIndex = mesh->GetMeshBufferIndex();
    directionalLightProjection.LightIndex = x;

    glm::mat4 reflectionProj = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 25.0f);

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
void DepthCubeMapPipeline::Destroy()
{
    cubeMapSampler.Destroy();
    GraphicsPipeline::Destroy();
}