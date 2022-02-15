#include "RayTraceRenderer.h"

RayTraceRenderer::RayTraceRenderer()
{
}

RayTraceRenderer::~RayTraceRenderer()
{
}

void RayTraceRenderer::StartUp()
{
    TopLevelAccelerationStructure = AccelerationStructureBuffer();
    SetUpTopLevelAccelerationStructure();

    RayTracePipeline = std::make_shared<RayTracingPipeline>();
    RayTracePipeline->SetUp(TopLevelAccelerationStructure);

    SetUpCommandBuffers();
}

void RayTraceRenderer::SetUpCommandBuffers()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = VulkanRenderer::GetCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(VulkanRenderer::GetDevice(), &allocInfo, &RayTraceCommandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffers.");
    }
}

void RayTraceRenderer::SetUpTopLevelAccelerationStructure()
{
    //uint32_t PrimitiveCount = 1;
    //std::vector<VkAccelerationStructureInstanceKHR> AccelerationStructureInstanceList = {};
    //for (int x = 0; x < assetManager->meshManager->MeshList.size(); x++)
    //{
    //    if (assetManager->meshManager->MeshList[x]->ShowMesh)
    //    {
    //        glm::mat4 GLMTransformMatrix2 = glm::transpose(glm::mat4(1.0f));
    //        if (assetManager->meshManager->MeshList[x]->ParentModelID != 0)
    //        {
    //            const auto model = assetManager->ObjManager->GetModel(assetManager->meshManager->MeshList[x]->ParentModelID);
    //            if (model == nullptr)
    //            {
    //                GLMTransformMatrix2 = glm::transpose(glm::mat4(1.0f));
    //            }
    //            else
    //            {
    //                GLMTransformMatrix2 = glm::transpose(model->ModelTransform);
    //            }
    //        }
    //        VkTransformMatrixKHR transformMatrix = GLMToVkTransformMatrix(GLMTransformMatrix2);

    //        VkAccelerationStructureInstanceKHR AccelerationStructureInstance{};
    //        AccelerationStructureInstance.transform = transformMatrix;
    //        AccelerationStructureInstance.instanceCustomIndex = assetManager->meshManager->MeshList[x]->MeshBufferIndex;
    //        AccelerationStructureInstance.mask = 0xFF;
    //        AccelerationStructureInstance.instanceShaderBindingTableRecordOffset = 0;
    //        AccelerationStructureInstance.flags = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
    //        AccelerationStructureInstance.accelerationStructureReference = assetManager->meshManager->MeshList[x]->BottomLevelAccelerationBuffer.AccelerationBuffer.BufferDeviceAddress;
    //        AccelerationStructureInstanceList.emplace_back(AccelerationStructureInstance);
    //    }
    //}

    //VkDeviceOrHostAddressConstKHR DeviceOrHostAddressConst = {};
    //if (AccelerationStructureInstanceList.size() > 0)
    //{
    //    InstancesBuffer = VulkanBuffer(AccelerationStructureInstanceList.data(), sizeof(VkAccelerationStructureInstanceKHR) * AccelerationStructureInstanceList.size(), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    //    DeviceOrHostAddressConst.deviceAddress = engine->GetBufferDeviceAddress(instancesBuffer.Buffer);
    //}

    //VkAccelerationStructureGeometryKHR AccelerationStructureGeometry{};
    //AccelerationStructureGeometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
    //AccelerationStructureGeometry.geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR;
    //AccelerationStructureGeometry.flags = VK_GEOMETRY_NO_DUPLICATE_ANY_HIT_INVOCATION_BIT_KHR;
    //AccelerationStructureGeometry.geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
    //AccelerationStructureGeometry.geometry.instances.arrayOfPointers = VK_FALSE;
    //AccelerationStructureGeometry.geometry.instances.data = DeviceOrHostAddressConst;

    //VkAccelerationStructureBuildGeometryInfoKHR AccelerationStructureBuildGeometryInfo = {};
    //AccelerationStructureBuildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
    //AccelerationStructureBuildGeometryInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
    //AccelerationStructureBuildGeometryInfo.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR | VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR;
    //AccelerationStructureBuildGeometryInfo.geometryCount = 1;
    //AccelerationStructureBuildGeometryInfo.pGeometries = &AccelerationStructureGeometry;

    //VkAccelerationStructureBuildSizesInfoKHR accelerationStructureBuildSizesInfo{};
    //accelerationStructureBuildSizesInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
    //VulkanRenderer::vkGetAccelerationStructureBuildSizesKHR(engine->Device, VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, &AccelerationStructureBuildGeometryInfo, &PrimitiveCount, &accelerationStructureBuildSizesInfo);

    //if (topLevelAS.handle == VK_NULL_HANDLE)
    //{
    //    topLevelAS.CreateAccelerationStructure(engine, VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR, accelerationStructureBuildSizesInfo);
    //}

    //VulkanBuffer scratchBuffer = VulkanBuffer(accelerationStructureBuildSizesInfo.buildScratchSize, VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    //scratchBuffer.BufferDeviceAddress = engine->GetBufferDeviceAddress(scratchBuffer.Buffer);

    //VkAccelerationStructureBuildGeometryInfoKHR AccelerationStructureBuildGeometryInfo2 = {};
    //AccelerationStructureBuildGeometryInfo2.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
    //AccelerationStructureBuildGeometryInfo2.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
    //AccelerationStructureBuildGeometryInfo2.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR | VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR;
    //AccelerationStructureBuildGeometryInfo2.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
    //AccelerationStructureBuildGeometryInfo2.geometryCount = 1;
    //AccelerationStructureBuildGeometryInfo2.pGeometries = &AccelerationStructureGeometry;
    //AccelerationStructureBuildGeometryInfo2.scratchData.deviceAddress = scratchBuffer.BufferDeviceAddress;

    //if (topLevelAS.handle == VK_NULL_HANDLE)
    //{
    //    AccelerationStructureBuildGeometryInfo2.dstAccelerationStructure = topLevelAS.handle;
    //}
    //else
    //{
    //    AccelerationStructureBuildGeometryInfo2.srcAccelerationStructure = topLevelAS.handle;
    //    AccelerationStructureBuildGeometryInfo2.dstAccelerationStructure = topLevelAS.handle;
    //}

    //VkAccelerationStructureBuildRangeInfoKHR AccelerationStructureBuildRangeInfo = {};
    //AccelerationStructureBuildRangeInfo.primitiveCount = static_cast<uint32_t>(AccelerationStructureInstanceList.size());
    //AccelerationStructureBuildRangeInfo.primitiveOffset = 0;
    //AccelerationStructureBuildRangeInfo.firstVertex = 0;
    //AccelerationStructureBuildRangeInfo.transformOffset = 0;
    //std::vector<VkAccelerationStructureBuildRangeInfoKHR> AccelerationStructureBuildRangeInfoList = { AccelerationStructureBuildRangeInfo };

    //topLevelAS.AcclerationCommandBuffer(engine, AccelerationStructureBuildGeometryInfo2, AccelerationStructureBuildRangeInfoList);

    //scratchBuffer.DestoryBuffer();
    //InstancesBuffer.DestoryBuffer();
}

void RayTraceRenderer::Draw()
{
    //VkCommandBufferBeginInfo cmdBufInfo{};
    //cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    //VkImageSubresourceRange subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

    //vkBeginCommandBuffer(RayTraceCommandBuffer, &cmdBufInfo);
    //const uint32_t handleSizeAligned = engine->GetAlignedSize(RayTracePipeline->rayTracingPipelineProperties.shaderGroupHandleSize, RayTracePipeline->rayTracingPipelineProperties.shaderGroupHandleAlignment);

    //VkStridedDeviceAddressRegionKHR raygenShaderSbtEntry{};
    //raygenShaderSbtEntry.deviceAddress = VulkanRenderer::GetBufferDeviceAddress(RayTracePipeline->raygenShaderBindingTable.Buffer);
    //raygenShaderSbtEntry.stride = handleSizeAligned;
    //raygenShaderSbtEntry.size = handleSizeAligned;

    //VkStridedDeviceAddressRegionKHR missShaderSbtEntry{};
    //missShaderSbtEntry.deviceAddress = VulkanRenderer::GetBufferDeviceAddress(RayTracePipeline->missShaderBindingTable.Buffer);
    //missShaderSbtEntry.stride = handleSizeAligned;
    //missShaderSbtEntry.size = handleSizeAligned;

    //VkStridedDeviceAddressRegionKHR hitShaderSbtEntry{};
    //hitShaderSbtEntry.deviceAddress = VulkanRenderer::GetBufferDeviceAddress(RayTracePipeline->hitShaderBindingTable.Buffer);
    //hitShaderSbtEntry.stride = handleSizeAligned;
    //hitShaderSbtEntry.size = handleSizeAligned;

    //VkStridedDeviceAddressRegionKHR callableShaderSbtEntry{};

    ////RTXConst.proj = glm::inverse(ViewCamera->GetProjectionMatrix());
    ////RTXConst.view = glm::inverse(ViewCamera->GetViewMatrix());
    ////RTXConst.CameraPos = ViewCamera->GetPosition();
    ////RTXConst.frame = Frame++;

    ////if (RTXConst.proj != LastCameraProjection)
    ////{
    ////    Frame = 0;
    ////}
    ////if (RTXConst.view != LastCameraView)
    ////{
    ////    Frame = 0;
    ////}

    //LastCameraProjection = RTXConst.proj;
    //LastCameraView = RTXConst.view;

    //vkCmdPushConstants(RayTraceCommandBuffer, RayTracePipeline->ShaderPipelineLayout, VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_MISS_BIT_KHR | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR, 0, sizeof(RayTraceConstants), &RTXConst);
    //vkCmdBindPipeline(RayTraceCommandBuffer, VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, RayTracePipeline->ShaderPipeline);
    //vkCmdBindDescriptorSets(RayTraceCommandBuffer, VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, RayTracePipeline->ShaderPipelineLayout, 0, 1, &RayTracePipeline->DescriptorSet, 0, 0);

    //RayTracedTexture->UpdateImageLayout(RayTraceCommandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL);
    //engine->vkCmdTraceRaysKHR(RayTraceCommandBuffer, &raygenShaderSbtEntry, &missShaderSbtEntry, &hitShaderSbtEntry, &callableShaderSbtEntry, engine->SwapChain.SwapChainResolution.width, engine->SwapChain.SwapChainResolution.height, 1);
    //RayTracedTexture->UpdateImageLayout(RayTraceCommandBuffer, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    //vkEndCommandBuffer(RayTraceCommandBuffer);
}

void RayTraceRenderer::RebuildSwapChain()
{
    RayTracePipeline->UpdateGraphicsPipeLine(TopLevelAccelerationStructure);
}

void RayTraceRenderer::Destroy()
{
    //{
    //    vkFreeMemory(VulkanRenderer::GetDevice(), TopLevelAccelerationStructure.AccelerationBuffer.BufferMemory, nullptr);
    //    vkDestroyBuffer(VulkanRenderer::GetDevice(), TopLevelAccelerationStructure.AccelerationBuffer.Buffer, nullptr);
    //    VulkanRenderer::vkDestroyAccelerationStructureKHR(VulkanRenderer::GetDevice(), TopLevelAccelerationStructure.handle, nullptr);

    //    TopLevelAccelerationStructure.AccelerationBuffer.BufferMemory = VK_NULL_HANDLE;
    //    TopLevelAccelerationStructure.B.Buffer = VK_NULL_HANDLE;
    //    TopLevelAccelerationStructure.handle = VK_NULL_HANDLE;
    //    TopLevelAccelerationStructure.AccelerationBuffer.BufferDeviceAddress = 0;
    //}
    //{
    //    RayTracedTexture->Delete();
    //    ShadowTextureMask->Delete();
    //    ReflectionTextureMask->Delete();
    //    ReflectionTexture->Delete();
    //    SSAOTexture->Delete();
    //}
    //RayTracePipeline->Destroy();
}
