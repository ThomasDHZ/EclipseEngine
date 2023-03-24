#include "BillboardPipeline.h"

BillboardPipeline::BillboardPipeline()
{
}

BillboardPipeline::~BillboardPipeline()
{
}

void BillboardPipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct)
{
    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    std::vector<VkDescriptorBufferInfo> MeshPropertiesmBufferList = MeshRendererManager::GetMeshPropertiesBuffer();
    std::vector<VkDescriptorBufferInfo> MaterialBufferList = MaterialManager::GetMaterialBufferList();
    std::vector<VkDescriptorBufferInfo> DirectionalLightBufferInfoList = GLTFSceneManager::GetDirectionalLightPropertiesBuffer();
    std::vector<VkDescriptorBufferInfo> PointLightBufferInfoList = GLTFSceneManager::GetPointLightPropertiesBuffer();
    std::vector<VkDescriptorBufferInfo> SpotLightBufferInfoList = GLTFSceneManager::GetSpotLightPropertiesBuffer();
    std::vector<VkDescriptorImageInfo> RenderedTextureBufferInfo = TextureManager::GetTexturemBufferList();

    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "BillBoardShaderVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "BillBoardShaderFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, MeshPropertiesmBufferList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 1, MaterialBufferList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 2, DirectionalLightBufferInfoList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 3, PointLightBufferInfoList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 4, SpotLightBufferInfoList);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 5, RenderedTextureBufferInfo, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR);

    //BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
    //buildGraphicsPipelineInfo.ColorAttachments = pipelineInfoStruct.ColorAttachments;
    //buildGraphicsPipelineInfo.DescriptorBindingList = DescriptorBindingList;
    //buildGraphicsPipelineInfo.renderPass = pipelineInfoStruct.renderPass;
    //buildGraphicsPipelineInfo.PipelineShaderStageList = PipelineShaderStageList;
    //buildGraphicsPipelineInfo.sampleCount = pipelineInfoStruct.SampleCount;
    //buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderMesh;
    //buildGraphicsPipelineInfo.ConstBufferSize = sizeof(BillboardSceneProperties);
    //buildGraphicsPipelineInfo.VertexDescriptorType = VertexDescriptorTypeEnum::kVertex3D;

    //if (ShaderPipeline == nullptr)
    //{
    //    CreateGraphicsPipeline(buildGraphicsPipelineInfo);
    //}
    //else
    //{
    //    Destroy();
    //    UpdateGraphicsPipeLine(buildGraphicsPipelineInfo);
    //}

    for (auto& shader : PipelineShaderStageList)
    {
        vkDestroyShaderModule(VulkanRenderer::GetDevice(), shader.module, nullptr);
    }
}

void BillboardPipeline::Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh)
{
    SceneManager::sceneProperites.MeshIndex = mesh->GetMeshBufferIndex();
    SceneManager::sceneProperites.MeshColorID = Converter::PixelToVec3(mesh->GetMeshColorID());

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
    vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(BillboardSceneProperties), &SceneManager::sceneProperites);
    mesh->Draw(commandBuffer);
}
