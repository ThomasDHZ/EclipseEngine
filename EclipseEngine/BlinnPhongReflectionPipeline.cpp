//#include "BlinnPhongReflectionPipeline.h"
//
//
//BlinnPhongReflectionPipeline::BlinnPhongReflectionPipeline()
//{
//}
//
//BlinnPhongReflectionPipeline::~BlinnPhongReflectionPipeline()
//{
//}
//
//void BlinnPhongReflectionPipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<CubeMapTexture> cubemap, std::shared_ptr<RenderedDepthTexture> depthTexture)
//{
//    std::vector<VkDescriptorBufferInfo> MeshPropertiesBufferList = MeshRendererManager::GetMeshPropertiesBuffer();
//    std::vector<VkDescriptorBufferInfo> DirectionalLightBufferInfoList = LightManager::GetDirectionalLightBuffer();
//    std::vector<VkDescriptorBufferInfo> PointLightBufferInfoList = LightManager::GetPointLightBuffer();
//    std::vector<VkDescriptorBufferInfo> SpotLightBufferInfoList = LightManager::GetSpotLightBuffer();
//    std::vector<VkDescriptorImageInfo> RenderedTextureBufferInfo = TextureManager::GetTexturemBufferList();
//
//    VkDescriptorBufferInfo ViewBufferListInfo = {};
//    ViewBufferListInfo.buffer = cubeMapSampler.GetVulkanBufferData().Buffer;
//    ViewBufferListInfo.offset = 0;
//    ViewBufferListInfo.range = VK_WHOLE_SIZE;
//
//    VkDescriptorImageInfo SkyboxBufferInfo;
//    SkyboxBufferInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//    SkyboxBufferInfo.imageView = cubemap->View;
//    SkyboxBufferInfo.sampler = cubemap->Sampler;
//
//    VkDescriptorImageInfo ShadowMapBufferInfo;
//    ShadowMapBufferInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
//    ShadowMapBufferInfo.imageView = depthTexture->View;
//    ShadowMapBufferInfo.sampler = depthTexture->Sampler;
//
//    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
//    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "BlinnPhoneReflectVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
//    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "BlinnPhoneReflectFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));
//
//    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
//    AddUniformBufferDescriptorSetBinding(DescriptorBindingList, 0, ViewBufferListInfo, VK_SHADER_STAGE_VERTEX_BIT);
//    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 1, MeshPropertiesBufferList);
//    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 2, DirectionalLightBufferInfoList);
//    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 3, PointLightBufferInfoList);
//    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 4, SpotLightBufferInfoList);
//    AddTextureDescriptorSetBinding(DescriptorBindingList, 5, RenderedTextureBufferInfo, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR);
//    AddTextureDescriptorSetBinding(DescriptorBindingList, 6, SkyboxBufferInfo, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
//    AddTextureDescriptorSetBinding(DescriptorBindingList, 7, ShadowMapBufferInfo, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
//
//    BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
//    buildGraphicsPipelineInfo.ColorAttachments = pipelineInfoStruct.ColorAttachments;
//    buildGraphicsPipelineInfo.DescriptorBindingList = DescriptorBindingList;
//    buildGraphicsPipelineInfo.renderPass = pipelineInfoStruct.renderPass;
//    buildGraphicsPipelineInfo.PipelineShaderStageList = PipelineShaderStageList;
//    buildGraphicsPipelineInfo.sampleCount = pipelineInfoStruct.SampleCount;
//    buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderMesh;
//    buildGraphicsPipelineInfo.ConstBufferSize = sizeof(SceneProperties);
//    buildGraphicsPipelineInfo.VertexDescriptorType = VertexDescriptorTypeEnum::kVertex3D;
//
//    if (ShaderPipeline == nullptr)
//    {
//        CreateGraphicsPipeline(buildGraphicsPipelineInfo);
//    }
//    else
//    {
//        Destroy();
//        UpdateGraphicsPipeLine(buildGraphicsPipelineInfo);
//    }
//
//    for (auto& shader : PipelineShaderStageList)
//    {
//        vkDestroyShaderModule(VulkanRenderer::GetDevice(), shader.module, nullptr);
//    }
//}
//
//void BlinnPhongReflectionPipeline::Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh, glm::vec3 CubeMapSamplerPos)
//{
//    SceneManager::sceneProperites.MeshIndex = mesh->GetMeshBufferIndex();
//    SceneManager::sceneProperites.MeshColorID = Converter::PixelToVec3(mesh->GetMeshColorID());
//
//    glm::mat4 reflectionProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10000.0f);
//
//    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[0] = reflectionProj * glm::lookAt(CubeMapSamplerPos, CubeMapSamplerPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
//    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[1] = reflectionProj * glm::lookAt(CubeMapSamplerPos, CubeMapSamplerPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
//    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[2] = reflectionProj * glm::lookAt(CubeMapSamplerPos, CubeMapSamplerPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[3] = reflectionProj * glm::lookAt(CubeMapSamplerPos, CubeMapSamplerPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
//    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[4] = reflectionProj * glm::lookAt(CubeMapSamplerPos, CubeMapSamplerPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
//    cubeMapSampler.UniformDataInfo.CubeMapFaceMatrix[5] = reflectionProj * glm::lookAt(CubeMapSamplerPos, CubeMapSamplerPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
//    cubeMapSampler.Update();
//
//    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
//    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
//    vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SceneProperties), &SceneManager::sceneProperites);
//    mesh->Draw(commandBuffer);
//}
