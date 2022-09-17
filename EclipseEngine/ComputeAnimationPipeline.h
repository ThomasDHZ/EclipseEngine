#pragma once
#include "GraphicsPipeline.h"
#include "Mesh3D.h"

class ComputeAnimationPipeline : public GraphicsPipeline
{
private:
	VulkanBuffer* VertexBufferCopy;
	Mesh3D* mesh;

	VkCommandBuffer commandBuffer;

	void SetUpDescriptorBindings();
	void CreateShaderPipeLine();
public:
	ComputeAnimationPipeline();
	ComputeAnimationPipeline(Mesh3D* meshptr);
	~ComputeAnimationPipeline();

	void Compute();
};