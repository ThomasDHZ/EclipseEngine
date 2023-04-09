#pragma once
#include "FrameBufferRenderPass.h"
#include "RenderPass2D.h"

class GLTFRenderer2D
{
private:
	std::vector<std::shared_ptr<GameObject>> gameObjectList;
	RenderPass2D renderPass2D;
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

