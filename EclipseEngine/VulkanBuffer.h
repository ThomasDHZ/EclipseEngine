#pragma once
#include "VulkanRenderer.h"

class VulkanBuffer
{
private:
	VkBuffer Buffer = VK_NULL_HANDLE;
	VkDeviceMemory BufferMemory = VK_NULL_HANDLE;
	VkDeviceSize BufferSize = 0;
	uint64_t BufferDeviceAddress = 0;
	VkAccelerationStructureKHR BufferHandle = VK_NULL_HANDLE;

public:
	VulkanBuffer();
	VulkanBuffer(void* BufferData, VkDeviceSize BufferSize, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
	~VulkanBuffer();

	VkResult CreateBuffer(void* BufferData, VkDeviceSize BufferSize, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void CopyBufferToMemory(void* DataToCopy, VkDeviceSize BufferSize);
	void DestoryBuffer();

	VkBuffer GetBuffer() { return Buffer; }
	VkBuffer* GetBufferPtr() { return &Buffer; }
	VkDeviceMemory GetBufferMemory() { return BufferMemory; }
	VkDeviceSize GetBufferSize() { return BufferSize; }
	uint64_t GetBufferDeviceAddress() { return BufferDeviceAddress; }
	VkAccelerationStructureKHR GetBufferHandle() { return BufferHandle; }
};