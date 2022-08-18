#pragma once
#include "GraphicsPipeline.h"
#include "GameObjectManager.h"
#include "MeshRendererManager.h"
#include "LightManager.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"
#include "RenderedCubeMapDepthTexture.h"

class PBRPipeline : public GraphicsPipeline
{

public:
	PBRPipeline();
	~PBRPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<RenderedCubeMapTexture> reflectionIrradianceMap, std::shared_ptr<RenderedCubeMapTexture> reflectionPrefilterMap, std::shared_ptr<RenderedDepthTexture> depthTexture, std::vector<std::shared_ptr<RenderedCubeMapDepthTexture>> pointLightShadowMaps);
	void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh);
};
