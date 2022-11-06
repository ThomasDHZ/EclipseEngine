#include "VulkanBuffer.h"
#include "VulkanBuffer.h"
#include <stdexcept>

VulkanBuffer::VulkanBuffer()
{
}

VulkanBuffer::VulkanBuffer(void* BufferData, VkDeviceSize BufferSize, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
	CreateBuffer(BufferData, BufferSize, usage, properties);
}

VulkanBuffer::~VulkanBuffer()
{
}

VkResult VulkanBuffer::CreateBuffer(void* BufferData, VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
	BufferSize = bufferSize;
	BufferUsage = usage;
	BufferProperties = properties;

	VkBufferCreateInfo buffer = {};
	buffer.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer.size = BufferSize;
	buffer.usage = BufferUsage;
	buffer.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	if (vkCreateBuffer(VulkanRenderer::GetDevice(), &buffer, nullptr, &Buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create buffer.");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(VulkanRenderer::GetDevice(), Buffer, &memRequirements);

	VkMemoryAllocateFlagsInfoKHR ExtendedAllocFlagsInfo{};
	ExtendedAllocFlagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO_KHR;
	ExtendedAllocFlagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT_KHR;

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = VulkanRenderer::GetMemoryType(memRequirements.memoryTypeBits, BufferProperties);
	allocInfo.pNext = &ExtendedAllocFlagsInfo;

	if (vkAllocateMemory(VulkanRenderer::GetDevice(), &allocInfo, nullptr, &BufferMemory) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate buffer memory.");
	}

	if (BufferData != nullptr)
	{
		vkMapMemory(VulkanRenderer::GetDevice(), BufferMemory, 0, BufferSize, 0, &data);
		memcpy(data, BufferData, (size_t)BufferSize);
		vkUnmapMemory(VulkanRenderer::GetDevice(), BufferMemory);
	}
	
	vkBindBufferMemory(VulkanRenderer::GetDevice(), Buffer, BufferMemory, 0);
	return vkMapMemory(VulkanRenderer::GetDevice(), BufferMemory, 0, BufferSize, 0, &data);
}

void VulkanBuffer::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	VkCommandBuffer commandBuffer = VulkanRenderer::BeginSingleTimeCommands();

	VkBufferCopy copyRegion = {};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	VulkanRenderer::EndSingleTimeCommands(commandBuffer);
}

void VulkanBuffer::CopyBufferToMemory(void* DataToCopy, VkDeviceSize BufferSize)
{
	memcpy(data, DataToCopy, (size_t)BufferSize);
}

VkResult VulkanBuffer::UpdateBufferSize(VkDeviceSize bufferSize)
{
	BufferSize = bufferSize;

	DestoryBuffer();
	
	VkBufferCreateInfo buffer = {};
	buffer.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer.size = BufferSize;
	buffer.usage = BufferUsage;
	buffer.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	if (vkCreateBuffer(VulkanRenderer::GetDevice(), &buffer, nullptr, &Buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create buffer.");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(VulkanRenderer::GetDevice(), Buffer, &memRequirements);

	VkMemoryAllocateFlagsInfoKHR ExtendedAllocFlagsInfo{};
	ExtendedAllocFlagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO_KHR;
	ExtendedAllocFlagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT_KHR;

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = VulkanRenderer::GetMemoryType(memRequirements.memoryTypeBits, BufferProperties);
	allocInfo.pNext = &ExtendedAllocFlagsInfo;

	if (vkAllocateMemory(VulkanRenderer::GetDevice(), &allocInfo, nullptr, &BufferMemory) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate buffer memory.");
	}

	vkBindBufferMemory(VulkanRenderer::GetDevice(), Buffer, BufferMemory, 0);
	return vkMapMemory(VulkanRenderer::GetDevice(), BufferMemory, 0, BufferSize, 0, &data);
}

void VulkanBuffer::DestoryBuffer()
{
	BufferDeviceAddress = 0;
	if (Buffer != nullptr &&
		BufferMemory != nullptr)
	{
		vkDestroyBuffer(VulkanRenderer::GetDevice(), Buffer, nullptr);
		vkFreeMemory(VulkanRenderer::GetDevice(), BufferMemory, nullptr);

		Buffer = VK_NULL_HANDLE;
		BufferMemory = VK_NULL_HANDLE;

		if (BufferHandle != VK_NULL_HANDLE)
		{
			BufferDeviceAddress = 0;

			VulkanRenderer::vkDestroyAccelerationStructureKHR(VulkanRenderer::GetDevice(), BufferHandle, nullptr);
			BufferHandle = VK_NULL_HANDLE;
		}
	}
}

void VulkanBuffer::SetBufferAddress(uint64_t BufferAddress)
{
	BufferDeviceAddress = BufferAddress;
}
