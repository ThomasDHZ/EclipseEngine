#pragma once
#include "VulkanRenderer.h"

class VulkanBuffer
{
private:
	VkDeviceMemory BufferMemory = VK_NULL_HANDLE;
	VkDeviceSize BufferSize = 0;
	VkBufferUsageFlags BufferUsage;
	VkMemoryPropertyFlags BufferProperties;
	uint64_t BufferDeviceAddress = 0;
	VkAccelerationStructureKHR BufferHandle = VK_NULL_HANDLE;
	void* data;

public:
	VulkanBuffer();
	VulkanBuffer(void* BufferData, VkDeviceSize BufferSize, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
	~VulkanBuffer();

	VkResult CreateBuffer(void* BufferData, VkDeviceSize BufferSize, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void CopyBufferToMemory(void* DataToCopy, VkDeviceSize BufferSize);
	VkResult UpdateBufferSize(VkDeviceSize bufferSize);
	void DestroyBuffer();

	void SetBufferAddress(uint64_t bufferSize);

	VkBuffer Buffer = VK_NULL_HANDLE;
	VkBuffer GetBuffer() { return Buffer; }
	VkBuffer* GetBufferPtr() { return &Buffer; }
	VkDeviceMemory GetBufferMemory() { return BufferMemory; }
	VkDeviceMemory* GetBufferMemoryPtr() { return &BufferMemory; }
	VkDeviceSize GetBufferSize() { return BufferSize; }
	uint64_t GetBufferDeviceAddress() { return BufferDeviceAddress; }
	VkAccelerationStructureKHR GetBufferHandle() { return BufferHandle; }
	VkAccelerationStructureKHR* GetBufferHandlePtr() { return &BufferHandle; }
	void* GetData() { return data; }
	bool IsBufferMemoryCleared();

	template<class T>
	T* DataView() { return ((T*)data); }
};