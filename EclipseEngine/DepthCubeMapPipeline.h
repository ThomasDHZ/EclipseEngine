#pragma once
#include "GraphicsPipeline.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"
#include "GLTFSceneManager.h"

class DepthCubeMapPipeline : public GraphicsPipeline
{
private:
	CubeMapSamplerBuffer cubeMapSampler;

public:
	DepthCubeMapPipeline();
	~DepthCubeMapPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct);
	void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> mesh, glm::vec3 CubeMapSamplerPos, uint32_t x);
	void Destroy() override;
};

