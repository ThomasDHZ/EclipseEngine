#pragma once
#include "GraphicsPipeline.h"
#include "Mesh3D.h"
#include "ConstMeshInfo.h"

class ComputeAnimationPipeline : public GraphicsPipeline
{
private:
	std::vector<VulkanBuffer*> VertexBufferCopy;
	//std::vector<Mesh3D*> meshPtr;

	VkCommandBuffer commandBuffer;

	void SetUpDescriptorBindings();
	void CreateShaderPipeLine();

public:
	ComputeAnimationPipeline();
	~ComputeAnimationPipeline();

	void StartUp();
	void Compute();
};