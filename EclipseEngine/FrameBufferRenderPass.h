#pragma once
#include "FrameBufferPipeline.h"
#include "BaseRenderPass.h"

class FrameBufferRenderPass : public BaseRenderPass
{
private:
	std::shared_ptr<FrameBufferPipeline> frameBufferPipeline;

	void CreateRenderPass();
	void CreateRendererFramebuffers();

public:
	FrameBufferRenderPass();
	~FrameBufferRenderPass();

	void StartUp();
	void RebuildSwapChain();
	void Draw();
	void Destroy();
};