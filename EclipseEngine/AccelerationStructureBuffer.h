#pragma once
#include "VulkanBuffer.h"

class AccelerationStructureBuffer
{
private:

	VulkanBuffer AccelerationBuffer;

public:

	AccelerationStructureBuffer();
	~AccelerationStructureBuffer();

	void AccelerationCommandBuffer( VkAccelerationStructureBuildGeometryInfoKHR& AccelerationStructureBuildGeometryInfo, std::vector<VkAccelerationStructureBuildRangeInfoKHR>& AccelerationStructureBuildRangeInfo);
	void CreateAccelerationStructure(VkAccelerationStructureTypeKHR type, VkAccelerationStructureBuildSizesInfoKHR& buildSizeInfo);
	void Destroy();

	uint64_t GetAccelerationBufferDeviceAddress() { return AccelerationBuffer.GetBufferDeviceAddress(); }
	VkAccelerationStructureKHR GetAccelerationStructureHandle() { return AccelerationBuffer.GetBufferHandle(); }
	VkAccelerationStructureKHR* GetAccelerationStructureHandlePtr() { return AccelerationBuffer.GetBufferHandlePtr(); }
};

