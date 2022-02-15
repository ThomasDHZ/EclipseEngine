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

	VkAccelerationStructureKHR handle = VK_NULL_HANDLE;
};

