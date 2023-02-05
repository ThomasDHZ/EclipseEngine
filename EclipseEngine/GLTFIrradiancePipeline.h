#pragma once

#include <GLTF_GraphicsPipeline.h>
#include "GameObjectManager.h"
#include "MeshRendererManager.h"
#include "LightManager.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"

class GLTFIrradiancePipeline : public GLTF_GraphicsPipeline
{
public:
	GLTFIrradiancePipeline();
	~GLTFIrradiancePipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::vector<std::shared_ptr<RenderedCubeMapTexture>> cubeMapList);
	void Draw(VkCommandBuffer& commandBuffer, IrradianceSkyboxSettings& irradiance);
};