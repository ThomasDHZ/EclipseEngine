#pragma once
#include "GraphicsPipeline.h"
#include "GameObjectManager.h"
#include "MeshRendererManager.h"
#include "LightManager.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"
#include "RenderedCubeMapTexture.h"
class CubeToEnvironmentPipeline : public GraphicsPipeline
{
public:
	CubeToEnvironmentPipeline();
	~CubeToEnvironmentPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<RenderedCubeMapTexture> cubeMap);
	void Draw(VkCommandBuffer& commandBuffer);
};