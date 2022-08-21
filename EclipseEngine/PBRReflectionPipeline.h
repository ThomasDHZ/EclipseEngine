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
	void BuildGraphicsPipeline(PipelineInfoStruct& pipelineInfoStruct);
public:
	PBRReflectionPipeline();
	~PBRReflectionPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, PBRRenderPassTextureSubmitList& textures);
	void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh, glm::vec3 CubeMapSamplerPos);
	virtual void Destroy();
};
