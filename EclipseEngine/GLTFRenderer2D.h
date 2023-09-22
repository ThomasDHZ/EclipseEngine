#pragma once
#include "FrameBufferRenderPass.h"
#include "RenderPass2D.h"
#include "GaussianBlurRenderPass.h"
#include "BloomCombineRenderPass.h"

class GLTFRenderer2D
{
private:
	RenderPass2D LevelRenderPass2D;
	GaussianBlurRenderPass BloomRenderPass;
	BloomCombineRenderPass bloomCombineRenderPass;
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

