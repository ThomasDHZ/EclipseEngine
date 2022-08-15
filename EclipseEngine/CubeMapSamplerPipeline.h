#pragma once
#include "GraphicsPipeline.h"
#include "GameObjectManager.h"
#include "MeshRendererManager.h"
#include "LightManager.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"

class CubeMapSamplerPipeline : public GraphicsPipeline
{
private:
	CubeMapSamplerBuffer cubeMapSampler;
public:
	CubeMapSamplerPipeline();
	~CubeMapSamplerPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<CubeMapTexture> cubemap);
	void Draw(VkCommandBuffer& commandBuffer);
	virtual void Destroy();
};

