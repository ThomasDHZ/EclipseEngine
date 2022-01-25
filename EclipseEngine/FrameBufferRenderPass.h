#pragma once
#include "FrameBufferPipeline.h"
#include "BaseRenderPass.h"

class FrameBufferRenderPass : public BaseRenderPass
{
private:
	void CreateRenderPass();
	void CreateRendererFramebuffers();

public:
	FrameBufferRenderPass();
	~FrameBufferRenderPass();

	std::shared_ptr<FrameBufferPipeline> frameBufferPipeline;

	void RebuildSwapChain();
	void Draw();
	void Destroy();
};