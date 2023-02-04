#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"
#include "GLTF_BRDFPipeline.h"

class GLTF_BRDFRenderPass : public RenderPass
{
private:
	void RenderPassDesc();
	void BuildRenderPassPipelines();

public:
	GLTF_BRDFRenderPass();
	~GLTF_BRDFRenderPass();

	GLTF_BRDFPipeline brdfPipeline;

	void BuildRenderPass(uint32_t textureSize);
	void Draw();
	void Destroy();
};

