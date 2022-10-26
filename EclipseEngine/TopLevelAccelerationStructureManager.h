#pragma once
#include "VulkanRenderer.h"
#include "Model.h"
#include "GraphicsPipeline.h"
#include "GameObjectManager.h"
class TopLevelAccelerationStructureManager
{
private: 
    static AccelerationStructureBuffer TopLevelAccelerationStructure;
    static VulkanBuffer InstancesBuffer;

public:
    static void Update()
    {

    /*    if (VulkanRenderer::UpdateTLAS)
        {*/
        UpdateTopLevelAccelerationStructure();
       /* }*/

        VulkanRenderer::UpdateBLAS = false;
        VulkanRenderer::UpdateTLAS = false;
	}

    static void UpdateTopLevelAccelerationStructure()
    {
        if (GraphicsDevice::IsRayTracingFeatureActive())
        {
            uint32_t PrimitiveCount = 1;
            std::vector<VkAccelerationStructureInstanceKHR> AccelerationStructureInstanceList = {};
            for (int x = 0; x < GameObjectManager::GetGameObjectList().size(); x++)
            {
              /*  if (auto component = GameObjectManager::GetGameObjectList()[x]->GetComponentByType(ComponentType::kMeshRenderer))
                {
                    auto meshRenderer = static_cast<MeshRendererComponent*>(component.get());
                    meshRenderer->GetModel()->UpdateMeshTopLevelAccelerationStructure(AccelerationStructureInstanceList);
                }*/
            }

            VkDeviceOrHostAddressConstKHR DeviceOrHostAddressConst = {};
            if (AccelerationStructureInstanceList.size() > 0)
            {
                InstancesBuffer = VulkanBuffer(AccelerationStructureInstanceList.data(), sizeof(VkAccelerationStructureInstanceKHR) * AccelerationStructureInstanceList.size(), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

                DeviceOrHostAddressConst.deviceAddress = VulkanRenderer::GetBufferDeviceAddress(InstancesBuffer.GetBuffer());
            }

            VkAccelerationStructureGeometryKHR AccelerationStructureGeometry{};
            AccelerationStructureGeometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
            AccelerationStructureGeometry.geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR;
            AccelerationStructureGeometry.flags = VK_GEOMETRY_NO_DUPLICATE_ANY_HIT_INVOCATION_BIT_KHR;
            AccelerationStructureGeometry.geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
            AccelerationStructureGeometry.geometry.instances.arrayOfPointers = VK_FALSE;
            AccelerationStructureGeometry.geometry.instances.data = DeviceOrHostAddressConst;

            VkAccelerationStructureBuildGeometryInfoKHR AccelerationStructureBuildGeometryInfo = {};
            AccelerationStructureBuildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
            AccelerationStructureBuildGeometryInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
            AccelerationStructureBuildGeometryInfo.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR | VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR;
            AccelerationStructureBuildGeometryInfo.geometryCount = 1;
            AccelerationStructureBuildGeometryInfo.pGeometries = &AccelerationStructureGeometry;

            VkAccelerationStructureBuildSizesInfoKHR accelerationStructureBuildSizesInfo{};
            accelerationStructureBuildSizesInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
            VulkanRenderer::vkGetAccelerationStructureBuildSizesKHR(VulkanRenderer::GetDevice(), VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, &AccelerationStructureBuildGeometryInfo, &PrimitiveCount, &accelerationStructureBuildSizesInfo);

            if (TopLevelAccelerationStructure.GetAccelerationStructureHandle() == VK_NULL_HANDLE)
            {
                TopLevelAccelerationStructure.CreateAccelerationStructure(VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR, accelerationStructureBuildSizesInfo);
            }

            VulkanBuffer scratchBuffer = VulkanBuffer(nullptr, accelerationStructureBuildSizesInfo.buildScratchSize, VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
            scratchBuffer.SetBufferAddress(VulkanRenderer::GetBufferDeviceAddress(scratchBuffer.GetBuffer()));

            VkAccelerationStructureBuildGeometryInfoKHR AccelerationStructureBuildGeometryInfo2 = {};
            AccelerationStructureBuildGeometryInfo2.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
            AccelerationStructureBuildGeometryInfo2.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
            AccelerationStructureBuildGeometryInfo2.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR | VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR;
            AccelerationStructureBuildGeometryInfo2.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
            AccelerationStructureBuildGeometryInfo2.geometryCount = 1;
            AccelerationStructureBuildGeometryInfo2.pGeometries = &AccelerationStructureGeometry;
            AccelerationStructureBuildGeometryInfo2.scratchData.deviceAddress = scratchBuffer.GetBufferDeviceAddress();

            if (TopLevelAccelerationStructure.GetAccelerationStructureHandle() == VK_NULL_HANDLE)
            {
                AccelerationStructureBuildGeometryInfo2.dstAccelerationStructure = TopLevelAccelerationStructure.GetAccelerationStructureHandle();
            }
            else
            {
                AccelerationStructureBuildGeometryInfo2.srcAccelerationStructure = TopLevelAccelerationStructure.GetAccelerationStructureHandle();
                AccelerationStructureBuildGeometryInfo2.dstAccelerationStructure = TopLevelAccelerationStructure.GetAccelerationStructureHandle();
            }

            VkAccelerationStructureBuildRangeInfoKHR AccelerationStructureBuildRangeInfo = {};
            AccelerationStructureBuildRangeInfo.primitiveCount = static_cast<uint32_t>(AccelerationStructureInstanceList.size());
            AccelerationStructureBuildRangeInfo.primitiveOffset = 0;
            AccelerationStructureBuildRangeInfo.firstVertex = 0;
            AccelerationStructureBuildRangeInfo.transformOffset = 0;
            std::vector<VkAccelerationStructureBuildRangeInfoKHR> AccelerationStructureBuildRangeInfoList = { AccelerationStructureBuildRangeInfo };

            TopLevelAccelerationStructure.AccelerationCommandBuffer(AccelerationStructureBuildGeometryInfo2, AccelerationStructureBuildRangeInfoList);

            scratchBuffer.DestoryBuffer();
            InstancesBuffer.DestoryBuffer();
        }
    }

    static void Destroy()
    {
        TopLevelAccelerationStructure.Destroy();
    }

    static VkAccelerationStructureKHR* GetAccelerationStructureHandlePtr() { return TopLevelAccelerationStructure.GetAccelerationStructureHandlePtr(); }
};

