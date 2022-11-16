#include "MeshPicker2DPipeline.h"

MeshPicker2DPipeline::MeshPicker2DPipeline()
{
}

MeshPicker2DPipeline::~MeshPicker2DPipeline()
{
}

void MeshPicker2DPipeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct)
{
    std::vector<VkDescriptorBufferInfo> MeshPropertiesmBufferList = MeshRendererManager::GetMeshPropertiesBuffer();
    std::vector<VkDescriptorBufferInfo> MaterialBufferList = MaterialManager::GetMaterialBufferList();
    std::vector<VkDescriptorImageInfo> RenderedTextureBufferInfo = TextureManager::GetTexturemBufferList();

    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "MeshPicker2DVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "MeshPicker2DFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, MeshPropertiesmBufferList);
    AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 1, MaterialBufferList);
    AddTextureDescriptorSetBinding(DescriptorBindingList, 2, RenderedTextureBufferInfo, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR);

    //BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
    //buildGraphicsPipelineInfo.ColorAttachments = pipelineInfoStruct.ColorAttachments;
    //buildGraphicsPipelineInfo.DescriptorBindingList = DescriptorBindingList;
    //buildGraphicsPipelineInfo.renderPass = pipelineInfoStruct.renderPass;
    //buildGraphicsPipelineInfo.PipelineShaderStageList = PipelineShaderStageList;
    //buildGraphicsPipelineInfo.sampleCount = pipelineInfoStruct.SampleCount;
    //buildGraphicsPipelineInfo.PipelineRendererType = PipelineRendererTypeEnum::kRenderMesh;
    //buildGraphicsPipelineInfo.ConstBufferSize = sizeof(SceneProperties);
    //buildGraphicsPipelineInfo.VertexDescriptorType = VertexDescriptorTypeEnum::kVertex2D;

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

void MeshPicker2DPipeline::Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh)
{
    SceneManager::sceneProperites.MeshIndex = mesh->GetMeshBufferIndex();
    SceneManager::sceneProperites.MeshColorID = Converter::PixelToVec3(mesh->GetMeshColorID());

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
    vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SceneProperties), &SceneManager::sceneProperites);
    mesh->Draw(commandBuffer);
}