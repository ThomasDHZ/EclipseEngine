#pragma once
#include "GraphicsPipeline.h"
#include "GameObjectManager.h"

class DrawLinePipeline : public GraphicsPipeline
{
private:
	void SetUpDescriptorBindings();
	void SetUpShaderPipeLine(const VkRenderPass& renderPass);
public:
	DrawLinePipeline();
	DrawLinePipeline(const VkRenderPass& renderPass);
	~DrawLinePipeline();

	void UpdateGraphicsPipeLine(const VkRenderPass& renderPass);
};


