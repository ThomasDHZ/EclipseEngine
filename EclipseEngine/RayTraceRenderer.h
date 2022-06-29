#pragma once
#include "FrameBufferRenderPass.h"
#include "RayTraceRenderPass.h"
#include "MeshPickerRenderPass3D.h"

class RayTraceRenderer : public RenderPass
{
private:
	MeshPickerRenderPass3D meshPickerRenderPass;
	FrameBufferRenderPass FrameBufferRenderer;

public:
	RayTraceRenderer();
	~RayTraceRenderer();
	RayTraceRenderPass rayTraceRenderPass;

	void BuildRenderer();
	void Update();
	void GUIUpdate();
	void Draw(SceneProperties& sceneProperties, std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	void Destroy();
};

