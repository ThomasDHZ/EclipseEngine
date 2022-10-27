#pragma once
#include "RenderPass2D.h"
#include "FrameBufferRenderPass.h"
#include "InterfaceRenderPass.h"
#include "MeshPickerRenderPass2D.h"
class Renderer2D
{
private:
	MeshPickerRenderPass2D meshPickerRenderPass;
	RenderPass2D renderPass2D;
	FrameBufferRenderPass frameBufferRenderPass;
public:
	Renderer2D();
	~Renderer2D();

	void BuildRenderer();
	void Update();
	void ImGuiUpdate();
	void Draw(SceneProperties& sceneProperites, std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	void Destroy();
};

