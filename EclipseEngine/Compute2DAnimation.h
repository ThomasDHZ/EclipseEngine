#pragma once
#include "JsonGraphicsPipeline.h"

class Compute2DAnimation : public JsonGraphicsPipeline
{
private:
	std::vector<VulkanBuffer*> VertexBufferCopy;
	//std::vector<Mesh3D*> meshPtr;

	VkCommandBuffer commandBuffer;

	void SetUpDescriptorBindings();
	void CreateShaderPipeLine();

public:
	Compute2DAnimation();
	~Compute2DAnimation();

	void StartUp();
	void Compute();
};

