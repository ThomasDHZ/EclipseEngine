#pragma once
#include "GraphicsPipeline.h"
#include "GameObjectManager.h"
#include "MeshRendererManager.h"
#include "LightManager.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"

class CubeMapViewPipeline : public GraphicsPipeline
{
private:
	ViewSamplerBuffer cubeMapSampler;
public:
	CubeMapViewPipeline();
	~CubeMapViewPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<CubeMapTexture> cubemap);
	void Draw(VkCommandBuffer& commandBuffer, uint32_t view);
};

