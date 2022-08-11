#pragma once
#include "GraphicsPipeline.h"
#include "GameObjectManager.h"
#include "MeshRendererManager.h"
#include "LightManager.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"

class PrefilterPipeline : public GraphicsPipeline
{
private:
	CubeMapSamplerBuffer cubeMapSampler;
public:
	PrefilterPipeline();
	~PrefilterPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<RenderedCubeMapTexture> cubeMap);
	void Draw(VkCommandBuffer& commandBuffer, PrefilterSkyboxSettings& prefiliter, glm::vec3 CubeMapSamplerPos);
};