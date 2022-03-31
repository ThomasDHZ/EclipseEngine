#pragma once
#include "MeshPickerRenderPass3D.h"
#include "BlinnPhongRenderPass.h"
#include "FrameBufferRenderPass.h"
#include "InterfaceRenderPass.h"
class BlinnPhongRenderer
{
private:
	MeshPickerRenderPass3D meshPickerRenderPass;
	BlinnPhongRenderPass blinnPhongRenderPass;
	FrameBufferRenderPass frameBufferRenderPass;
public:
	BlinnPhongRenderer();
	~BlinnPhongRenderer();

	void StartUp();
	void Update();
	void RebuildRenderers();
	void Draw(SceneProperties& sceneProperites, std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	void Destroy();


	std::shared_ptr<RenderedColorTexture> GetColorPickerTexture() { return meshPickerRenderPass.RenderedTexture; }
};

