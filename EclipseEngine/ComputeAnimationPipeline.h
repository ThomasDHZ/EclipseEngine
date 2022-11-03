#pragma once
#include "GraphicsPipeline.h"
#include "Mesh3D.h"

class ComputeAnimationPipeline : public GraphicsPipeline
{
private:
	VulkanBuffer* VertexBufferCopy;
	Mesh3D* meshPtr;

	VkCommandBuffer commandBuffer;

	void SetUpDescriptorBindings();
	void CreateShaderPipeLine();

public:
	ComputeAnimationPipeline();
	~ComputeAnimationPipeline();

	void StartUp();
	void Compute();
};