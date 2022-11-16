#pragma once
#include <GraphicsPipeline.h>
#include "GameObjectManager.h"
#include "MeshRendererManager.h"
#include "LightManager.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"

class IrradiancePipeline : public GraphicsPipeline
{
public:
	IrradiancePipeline();
	~IrradiancePipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::vector<std::shared_ptr<RenderedCubeMapTexture>> cubeMapList);
	void Draw(VkCommandBuffer& commandBuffer, IrradianceSkyboxSettings& irradiance);
};

