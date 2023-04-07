#pragma once
#include "FrameBufferRenderPass.h"

class GLTFRenderer2D
{
private:

	FrameBufferRenderPass frameBufferRenderPass;
public:
	GLTFRenderer2D();
	~GLTFRenderer2D();

	void BuildRenderer();
	void Update();
	void ImGuiUpdate();
	void Draw(SceneProperties& sceneProperites, std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	void Destroy();
};

