#pragma once
#include <GraphicsPipeline.h>
#include "GameObjectManager.h"
#include "MeshRendererManager.h"
#include "LightManager.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"
#include "RenderedCubeMapDepthTexture.h"
#include <GLTFSceneManager.h>

class PBRBloomPipeline : public GraphicsPipeline
{

public:
	PBRBloomPipeline();
	~PBRBloomPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, PBRRenderPassTextureSubmitList& textures);
	void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh);
};
