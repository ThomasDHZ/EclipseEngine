#pragma once
#include <GraphicsPipeline.h>
#include "GameObjectManager.h"
#include "MeshRendererManager.h"
#include "LightManager.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"
#include "RenderedCubeMapDepthTexture.h"
#include "ConstMeshInfo.h"
#include <GLTFSceneManager.h>

class PBRInstancedReflectionPipeline : public GraphicsPipeline
{
private:
	CubeMapSamplerBuffer cubeMapSampler;

public:
	PBRInstancedReflectionPipeline();
	~PBRInstancedReflectionPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, PBRRenderPassTextureSubmitList& textures);
	void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> drawMesh, std::shared_ptr<Mesh> reflectingMesh);
	virtual void Destroy();
};
