#pragma once
#include "GraphicsPipeline.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"
#include "GLTFSceneManager.h"

class DepthCubeMapPipeline : public GraphicsPipeline
{
private:

public:
	DepthCubeMapPipeline();
	~DepthCubeMapPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, CubeMapSamplerBuffer& cubeMapSampler);
	void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> mesh, glm::vec2 Aspect, glm::vec3 CubeMapSamplerPos, uint32_t x);
	void Destroy() override;
};

