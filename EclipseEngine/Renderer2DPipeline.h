#pragma once
#include "GraphicsPipeline.h"
#include "UniformBuffer.h"
#include "GameObjectManager.h"
#include "Texture2D.h"

class Renderer2DPipeline : public GraphicsPipeline
{
private:
	void SetUpDescriptorBindings();
	void SetUpShaderPipeLine(const VkRenderPass& renderPass);
public:

	Renderer2DPipeline();
	Renderer2DPipeline(const VkRenderPass& renderPass);
	~Renderer2DPipeline();

	void UpdateGraphicsPipeLine(const VkRenderPass& renderPass);
};

