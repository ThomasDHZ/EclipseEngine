#pragma once
#include "GraphicsPipeline.h"
#include "GameObjectManager.h"
#include "MeshRendererManager.h"
#include "LightManager.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"

class PBRReflectionPipeline : public GraphicsPipeline
{
private:
	CubeMapSamplerBuffer cubeMapSampler;
public:
	PBRReflectionPipeline();
	~PBRReflectionPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<RenderedCubeMapTexture> reflectionIrradianceMap, std::shared_ptr<RenderedCubeMapTexture> reflectionPrefilterMap);
	void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh, glm::vec3 CubeMapSamplerPos);
	virtual void Destroy();
};
