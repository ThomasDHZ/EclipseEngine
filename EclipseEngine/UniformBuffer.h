#pragma once
#include "VulkanBuffer.h"

template <class T>
class UniformBuffer
{
private: 
	T UniformDataInfo;
	VulkanBuffer VulkanBufferData;

public:
	UniformBuffer()
	{
	}

	UniformBuffer(T UniformData)
	{
		VulkanBufferData.CreateBuffer(sizeof(T), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		Update();
	}

	void Update(T UniformData)
	{
		UniformDataInfo = UniformData;
		VulkanBufferData.CopyBufferToMemory(&UniformDataInfo, sizeof(T));
	}

	void Destroy()
	{
		VulkanBufferData.DestoryBuffer();
	}

	VulkanBuffer GetVulkanBufferData() { return VulkanBufferData; }
};
