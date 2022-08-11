#pragma once
#include "GraphicsPipeline.h"
#include "GameObjectManager.h"
#include "MeshRendererManager.h"
#include "LightManager.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"

class IrradiancePipeline : public GraphicsPipeline
{
private:
	CubeMapSamplerBuffer cubeMapSampler;
public:
	IrradiancePipeline();
	~IrradiancePipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<RenderedCubeMapTexture> cubeMap);
	void Draw(VkCommandBuffer& commandBuffer, IrradianceSkyboxSettings& irradiance, glm::vec3 CubeMapSamplerPos);
};

