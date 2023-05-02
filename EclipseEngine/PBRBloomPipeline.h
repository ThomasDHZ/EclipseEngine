#pragma once
#include "GraphicsPipeline.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"
#include "RenderedCubeMapDepthTexture.h"
#include "GLTFSceneManager.h"
#include "ConstMeshInfo.h"

class PBRBloomPipeline : public GraphicsPipeline
{

public:
	PBRBloomPipeline();
	~PBRBloomPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, PBRRenderPassTextureSubmitList& textures);
	//void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh);
};
