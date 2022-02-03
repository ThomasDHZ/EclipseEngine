#pragma once
#include "BaseRenderPass.h"
#include "Renderer2DPipeline.h"
#include "RenderedTexture.h"
#include "RenderedColorTexture.h"
#include "RenderedDepthTexture.h"
#include "GameObject.h"

class RenderPass2D : public BaseRenderPass
{
private:
	void CreateRenderPass();
	void CreateRendererFramebuffers();

public:
	RenderPass2D();
	~RenderPass2D();

	std::shared_ptr<RenderedColorTexture> renderedTexture;
	std::shared_ptr<RenderedDepthTexture> depthTexture;

	std::shared_ptr<Renderer2DPipeline> renderer2DPipeline;

	void StartUp(GameObject obj, GameObject obj2);
	void RebuildSwapChain(GameObject obj, GameObject obj2);

	void Draw(std::vector<GameObject>& GameObjectList, SceneProperties sceneProperties);
	void Destroy();
};

