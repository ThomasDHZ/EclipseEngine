#pragma once
#include "MeshPickerRenderPass3D.h"
#include "BlinnPhongRenderPass.h"
#include "FrameBufferRenderPass.h"
#include "GBufferRenderPass.h"
#include "RaytraceHybridPass.h"
#include "DeferredRenderPass.h"

class HybridRenderer
{
private:
	MeshPickerRenderPass3D meshPickerRenderPass;
	GBufferRenderPass GBufferRenderPass;
	DeferredRenderPass deferredRenderPass;
	RaytraceHybridPass raytraceHybridPass;
	FrameBufferRenderPass frameBufferRenderPass;

public:
	HybridRenderer();
	~HybridRenderer();

	void StartUp();
	void Update();
	void RebuildRenderers();
	void Draw(SceneProperties& sceneProperites, std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	void Destroy();


	std::shared_ptr<RenderedColorTexture> GetColorPickerTexture() { return meshPickerRenderPass.RenderedTexture; }
};