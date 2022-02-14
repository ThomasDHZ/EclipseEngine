#include "AccelerationStructureBuffer.h"

AccelerationStructureBuffer::AccelerationStructureBuffer()
{
}

AccelerationStructureBuffer::~AccelerationStructureBuffer()
{
}

void AccelerationStructureBuffer::AcclerationCommandBuffer(VkAccelerationStructureBuildGeometryInfoKHR& AccelerationStructureBuildGeometryInfo, std::vector<VkAccelerationStructureBuildRangeInfoKHR>& AccelerationStructureBuildRangeInfo)
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

	VkMemoryAllocateFlagsInfo memoryAllocateFlagsInfo{};
	memoryAllocateFlagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
	memoryAllocateFlagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT_KHR;

	VkMemoryAllocateInfo memoryAllocateInfo{};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.pNext = &memoryAllocateFlagsInfo;
	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	memoryAllocateInfo.memoryTypeIndex = VulkanRenderer::GetMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	vkAllocateMemory(VulkanRenderer::GetDevice(), &memoryAllocateInfo, nullptr, AccelerationBuffer.GetBufferMemoryPtr());
	vkBindBufferMemory(VulkanRenderer::GetDevice(), AccelerationBuffer.GetBuffer(), AccelerationBuffer.GetBufferMemory(), 0);

	VkAccelerationStructureCreateInfoKHR accelerationStructureCreate_info{};
	accelerationStructureCreate_info.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
	accelerationStructureCreate_info.buffer = AccelerationBuffer.GetBuffer();
	accelerationStructureCreate_info.size = buildSizeInfo.accelerationStructureSize;
	accelerationStructureCreate_info.type = type;
	VulkanRenderer::vkCreateAccelerationStructureKHR(VulkanRenderer::GetDevice(), &accelerationStructureCreate_info, nullptr, &handle);

	VkAccelerationStructureDeviceAddressInfoKHR accelerationDeviceAddressInfo{};
	accelerationDeviceAddressInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR;
	accelerationDeviceAddressInfo.accelerationStructure = handle;
	AccelerationBuffer.SetBufferAddress(VulkanRenderer::vkGetAccelerationStructureDeviceAddressKHR(VulkanRenderer::GetDevice(), &accelerationDeviceAddressInfo));
}

void AccelerationStructureBuffer::Destroy()
{
	AccelerationBuffer.DestoryBuffer();
	handle = VK_NULL_HANDLE;
}