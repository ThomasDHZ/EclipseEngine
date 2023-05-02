#pragma once
#include "GraphicsPipeline.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"
#include "RenderedCubeMapDepthTexture.h"
#include "GLTFSceneManager.h"
#include "ConstMeshInfo.h"

class PBRReflectionPipeline : public GraphicsPipeline
{
private: 
	CubeMapSamplerBuffer cubeMapSampler;

public:
	PBRReflectionPipeline();
	~PBRReflectionPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, PBRRenderPassTextureSubmitList& textures);
	//void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> drawMesh, std::shared_ptr<Mesh> reflectingMesh);
	virtual void Destroy();
};
