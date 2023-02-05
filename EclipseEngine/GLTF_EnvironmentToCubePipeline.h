#pragma once
#include "GraphicsPipeline.h"
#include "GameObjectManager.h"
#include "MeshRendererManager.h"
#include "LightManager.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"
#include "RenderedCubeMapTexture.h"

class GLTF_EnvironmentToCubePipeline : public GLTF_GraphicsPipeline
{
public:
	GLTF_EnvironmentToCubePipeline();
	~GLTF_EnvironmentToCubePipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<RenderedCubeMapTexture> cubeMap);
	void Draw(VkCommandBuffer& commandBuffer);
};