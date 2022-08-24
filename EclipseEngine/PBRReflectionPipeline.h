#pragma once
#include "GraphicsPipeline.h"
#include "GameObjectManager.h"
#include "MeshRendererManager.h"
#include "LightManager.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"
#include "RenderedCubeMapDepthTexture.h"

class PBRReflectionPipeline : public GraphicsPipeline
{
private: 
	ViewSamplerBuffer cubeMapSampler;
	void BuildGraphicsPipeline(PipelineInfoStruct& pipelineInfoStruct);
public:
	PBRReflectionPipeline();
	~PBRReflectionPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, PBRRenderPassTextureSubmitList& textures);
	void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh, int View, glm::vec3 pos);
};
