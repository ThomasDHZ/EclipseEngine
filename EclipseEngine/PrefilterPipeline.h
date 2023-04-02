#pragma once
#include <GraphicsPipeline.h>
#include "GameObjectManager.h"
#include "MeshRendererManager.h"
#include "LightManager.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"

class PrefilterPipeline : public GraphicsPipeline
{
public:
	PrefilterPipeline();
	~PrefilterPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::vector<std::shared_ptr<RenderedCubeMapTexture>>& cubeMapList);
	void Draw(VkCommandBuffer& commandBuffer, PrefilterSkyboxSettings& prefiliter);
};
