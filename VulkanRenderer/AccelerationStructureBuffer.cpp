#include "AccelerationStructureBuffer.h"

AccelerationStructureBuffer::AccelerationStructureBuffer()
{
}

AccelerationStructureBuffer::~AccelerationStructureBuffer()
{
}

void AccelerationStructureBuffer::AccelerationCommandBuffer(VkAccelerationStructureBuildGeometryInfoKHR& AccelerationStructureBuildGeometryInfo, std::vector<VkAccelerationStructureBuildRangeInfoKHR>& AccelerationStructureBuildRangeInfo)
{
	VkCommandBufferAllocateInfo CommandBufferAllocateInfo{};
	CommandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	CommandBufferAllocateInfo.commandPool = VulkanRenderer::GetCommandPool();
	CommandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	CommandBufferAllocateInfo.commandBufferCount = 1;

	VkCommandBuffer cmdBuffer;
	vkAllocateCommandBuffers(VulkanRenderer::GetDevice(), &CommandBufferAllocateInfo, &cmdBuffer);

	VkCommandBufferBeginInfo cmdBufferBeginInfo{};
	cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBufferBeginInfo.flags = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;

	auto AccelerationStructureBuildRangeInfoPtr = AccelerationStructureBuildRangeInfo.data();
	vkBeginCommandBuffer(cmdBuffer, &cmdBufferBeginInfo);
	VulkanRenderer::vkCmdBuildAccelerationStructuresKHR(cmdBuffer, 1, &AccelerationStructureBuildGeometryInfo, &AccelerationStructureBuildRangeInfoPtr);
	vkEndCommandBuffer(cmdBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &cmdBuffer;
	VkFenceCreateInfo fenceCreateInfo{};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = 0;

	VkFence fence;
	vkCreateFence(VulkanRenderer::GetDevice(), &fenceCreateInfo, nullptr, &fence);
	vkQueueSubmit(VulkanRenderer::GetGraphicsQueue(), 1, &submitInfo, fence);
	vkWaitForFences(VulkanRenderer::GetDevice(), 1, &fence, VK_TRUE, UINT64_MAX);
	vkDestroyFence(VulkanRenderer::GetDevice(), fence, nullptr);
	vkFreeCommandBuffers(VulkanRenderer::GetDevice(), VulkanRenderer::GetCommandPool(), 1, &cmdBuffer);
}

void AccelerationStructureBuffer::CreateAccelerationStructure(VkAccelerationStructureTypeKHR type, VkAccelerationStructureBuildSizesInfoKHR& buildSizeInfo)
{
	VkBufferCreateInfo bufferCreateInfo{};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = buildSizeInfo.accelerationStructureSize;
	bufferCreateInfo.usage = VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
	vkCreateBuffer(VulkanRenderer::GetDevice(), &bufferCreateInfo, nullptr, AccelerationBuffer.GetBufferPtr());

	VkMemoryRequirements memoryRequirements{};
	vkGetBufferMemoryRequirements(VulkanRenderer::GetDevice(), AccelerationBuffer.GetBuffer(), &memoryRequirements);

	VkMemoryAllocateFlagsInfo MemoryAllocateFlags{};
	MemoryAllocateFlags.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
	MemoryAllocateFlags.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT_KHR;

	VkMemoryAllocateInfo MemoryAllocate{};
	MemoryAllocate.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	MemoryAllocate.pNext = &MemoryAllocateFlags;
	MemoryAllocate.allocationSize = memoryRequirements.size;
	MemoryAllocate.memoryTypeIndex = VulkanRenderer::GetMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	vkAllocateMemory(VulkanRenderer::GetDevice(), &MemoryAllocate, nullptr, AccelerationBuffer.GetBufferMemoryPtr());
	vkBindBufferMemory(VulkanRenderer::GetDevice(), AccelerationBuffer.GetBuffer(), AccelerationBuffer.GetBufferMemory(), 0);

	VkAccelerationStructureCreateInfoKHR AccelerationStructureCreateInfo{};
	AccelerationStructureCreateInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
	AccelerationStructureCreateInfo.buffer = AccelerationBuffer.GetBuffer();
	AccelerationStructureCreateInfo.size = buildSizeInfo.accelerationStructureSize;
	AccelerationStructureCreateInfo.type = type;
	VulkanRenderer::vkCreateAccelerationStructureKHR(VulkanRenderer::GetDevice(), &AccelerationStructureCreateInfo, nullptr, AccelerationBuffer.GetBufferHandlePtr());

	VkAccelerationStructureDeviceAddressInfoKHR AccelerationStructureDeviceAddressInfo{};
	AccelerationStructureDeviceAddressInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR;
	AccelerationStructureDeviceAddressInfo.accelerationStructure = AccelerationBuffer.GetBufferHandle();
	AccelerationBuffer.SetBufferAddress(VulkanRenderer::vkGetAccelerationStructureDeviceAddressKHR(VulkanRenderer::GetDevice(), &AccelerationStructureDeviceAddressInfo));
}

void AccelerationStructureBuffer::Destroy()
{
	AccelerationBuffer.DestoryBuffer();
}