#pragma once
#include "FrameBufferRenderPass.h"
#include "RenderPass2D.h"

class GLTFRenderer2D
{
private:
	RenderPass2D LevelRenderPass2D;
	FrameBufferRenderPass frameBufferRenderPass;
public:
	GLTFRenderer2D();
	~GLTFRenderer2D();

	void BuildRenderer();
	void Update();
	void ImGuiUpdate();
	void Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	void Destroy();
};

