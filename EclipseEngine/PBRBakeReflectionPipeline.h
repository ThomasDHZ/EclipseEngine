#pragma once
#include "GraphicsPipeline.h"
#include "GameObjectManager.h"
#include "MeshRendererManager.h"
#include "LightManager.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"
#include "RenderedCubeMapDepthTexture.h"
#include "GLTFSceneManager.h"
#include "ConstMeshInfo.h"

class PBRBakeReflectionPipeline : public GraphicsPipeline
{
private:
	std::vector<CubeMapSamplerBuffer> cubeMapSamplerList;

public:
	PBRBakeReflectionPipeline();
	~PBRBakeReflectionPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, PBRRenderPassTextureSubmitList& textures);
	void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> drawMesh, std::shared_ptr<Mesh> reflectingMesh, uint32_t counter);
	virtual void Destroy();
};
