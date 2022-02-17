#pragma once
#include "VulkanBuffer.h"

class AccelerationStructureBuffer
{
private:

	VulkanBuffer AccelerationBuffer;

public:

	AccelerationStructureBuffer();
	~AccelerationStructureBuffer();

	void AcclerationCommandBuffer( VkAccelerationStructureBuildGeometryInfoKHR& AccelerationStructureBuildGeometryInfo, std::vector<VkAccelerationStructureBuildRangeInfoKHR>& AccelerationStructureBuildRangeInfo);
	void CreateAccelerationStructure(VkAccelerationStructureTypeKHR type, VkAccelerationStructureBuildSizesInfoKHR& buildSizeInfo);
	void Destroy();

	uint64_t GetAccelerationBufferDeviceAddress() { return AccelerationBuffer.GetBufferDeviceAddress(); }

	VkAccelerationStructureKHR handle = VK_NULL_HANDLE;
};

