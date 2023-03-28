#include "GLTFPBRRenderPIpeline.h"
#include "SceneManager.h"

GLTFPBRRenderPIpeline::GLTFPBRRenderPIpeline()
{
}

GLTFPBRRenderPIpeline::~GLTFPBRRenderPIpeline()
{
}

void GLTFPBRRenderPIpeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::vector<GLTF_Temp_Model> modelList)
{
    GLTFSceneManager::AddDirectionalLight(std::make_shared<GLTFDirectionalLight>(GLTFDirectionalLight("sdf", glm::vec3(0.01f), glm::vec3(1.0f), 30.8f)));

    for (int x = 0; x < modelList.size(); x++)
    {
        importer.emplace_back(JsonGraphicsPipeline());
        importer.back().LoadGraphicsPipeline("GLTFPBRPipeline.txt", pipelineInfoStruct.renderPass, modelList, x, pipelineInfoStruct.ColorAttachments, pipelineInfoStruct.SampleCount, sizeof(SceneProperties));
    }
   // importer.emplace_back(std::make_shared<JsonGraphicsPipeline>("GLTFPBRPipeline.txt", pipelineInfoStruct.renderPass, modelList[1], pipelineInfoStruct.ColorAttachments, pipelineInfoStruct.SampleCount, sizeof(SceneProperties)));
    if (ShaderPipeline == nullptr)
    {
    }
    else
    {
        Destroy();
      //  UpdateGraphicsPipeLine(buildGraphicsPipelineInfo);
    }

}

void GLTFPBRRenderPIpeline::Draw(VkCommandBuffer& commandBuffer, GLTF_Temp_Model model, uint32_t drawIndex, uint32_t descriptorsetIndex, uint32_t descriptorsetcount)
{
  
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, importer[drawIndex].ShaderPipeline);
        model.Draw(commandBuffer, importer[drawIndex].ShaderPipelineLayout, descriptorsetIndex, descriptorsetcount);

}
