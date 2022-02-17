#include "RayTraceRenderPass.h"
#include "Math.h"

RayTraceRenderPass::RayTraceRenderPass()
{
}

RayTraceRenderPass::~RayTraceRenderPass()
{
}

void RayTraceRenderPass::StartUp()
{
    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();
    RayTracedTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution));

    TopLevelAccelerationStructure = AccelerationStructureBuffer();
    SetUpTopLevelAccelerationStructure();

    RayTracePipeline = std::make_shared<RayTracingPipeline>();
    RayTracePipeline->SetUp(TopLevelAccelerationStructure, RayTracedTexture);

    SetUpCommandBuffers();
}

void RayTraceRenderPass::SetUpCommandBuffers()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = VulkanRenderer::GetCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(VulkanRenderer::GetDevice(), &allocInfo, &CommandBuffer[VulkanRenderer::GetCMDIndex()]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffers.");
    }
}

void RayTraceRenderPass::SetUpTopLevelAccelerationStructure()
{
    uint32_t PrimitiveCount = 1;
    std::vector<VkAccelerationStructureInstanceKHR> AccelerationStructureInstanceList = {};
    for (int x = 0; x < GameObjectManager::GetGameObjectList().size(); x++)
    {

        const std::shared_ptr<GameObject> obj = GameObjectManager::GetGameObjectList()[x];
        ComponentRenderer* componentRenderer = nullptr;

        if (obj->GetComponentByType(ComponentType::kSpriteRenderer) ||
            obj->GetComponentByType(ComponentType::kMeshRenderer))
        {
            if (obj->GetComponentByType(ComponentType::kSpriteRenderer))
            {
                componentRenderer = static_cast<ComponentRenderer*>(obj->GetComponentByType(ComponentType::kSpriteRenderer).get());
            }

            if (obj->GetComponentByType(ComponentType::kMeshRenderer))
            {
                componentRenderer = static_cast<ComponentRenderer*>(obj->GetComponentByType(ComponentType::kMeshRenderer).get());
            }
        }
        else
        {
            continue;
        }

      /*  if (assetManager->meshManager->MeshList[x]->ShowMesh)
        {*/
            glm::mat4 GLMTransformMatrix2 = glm::transpose(glm::mat4(1.0f));
            //if (assetManager->meshManager->MeshList[x]->ParentModelID != 0)
            //{
            //    const auto model = assetManager->ObjManager->GetModel(assetManager->meshManager->MeshList[x]->ParentModelID);
            //    if (model == nullptr)
            //    {
            //        GLMTransformMatrix2 = glm::transpose(glm::mat4(1.0f));
            //    }
            //    else
            //    {
            //        GLMTransformMatrix2 = glm::transpose(model->ModelTransform);
            //    }
          /*  }*/
            VkTransformMatrixKHR transformMatrix = EngineMath::GLMToVkTransformMatrix(GLMTransformMatrix2);
            
            VkAccelerationStructureInstanceKHR AccelerationStructureInstance{};
            AccelerationStructureInstance.transform = transformMatrix;
            AccelerationStructureInstance.instanceCustomIndex = componentRenderer->GetMeshBufferIndex();
            AccelerationStructureInstance.mask = 0xFF;
            AccelerationStructureInstance.instanceShaderBindingTableRecordOffset = 0;
            AccelerationStructureInstance.flags = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
            AccelerationStructureInstance.accelerationStructureReference = componentRenderer->GetBLASBufferDeviceAddress();
            AccelerationStructureInstanceList.emplace_back(AccelerationStructureInstance);
      //  }
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

    if (TopLevelAccelerationStructure.handle == VK_NULL_HANDLE)
    {
        TopLevelAccelerationStructure.CreateAccelerationStructure(VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR, accelerationStructureBuildSizesInfo);
    }

    VulkanBuffer scratchBuffer = VulkanBuffer(accelerationStructureBuildSizesInfo.buildScratchSize, VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    scratchBuffer.SetBufferAddress(VulkanRenderer::GetBufferDeviceAddress(scratchBuffer.GetBuffer()));

    VkAccelerationStructureBuildGeometryInfoKHR AccelerationStructureBuildGeometryInfo2 = {};
    AccelerationStructureBuildGeometryInfo2.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
    AccelerationStructureBuildGeometryInfo2.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
    AccelerationStructureBuildGeometryInfo2.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR | VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR;
    AccelerationStructureBuildGeometryInfo2.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
    AccelerationStructureBuildGeometryInfo2.geometryCount = 1;
    AccelerationStructureBuildGeometryInfo2.pGeometries = &AccelerationStructureGeometry;
    AccelerationStructureBuildGeometryInfo2.scratchData.deviceAddress = scratchBuffer.GetBufferDeviceAddress();

    if (TopLevelAccelerationStructure.handle == VK_NULL_HANDLE)
    {
        AccelerationStructureBuildGeometryInfo2.dstAccelerationStructure = TopLevelAccelerationStructure.handle;
    }
    else
    {
        AccelerationStructureBuildGeometryInfo2.srcAccelerationStructure = TopLevelAccelerationStructure.handle;
        AccelerationStructureBuildGeometryInfo2.dstAccelerationStructure = TopLevelAccelerationStructure.handle;
    }

    VkAccelerationStructureBuildRangeInfoKHR AccelerationStructureBuildRangeInfo = {};
    AccelerationStructureBuildRangeInfo.primitiveCount = static_cast<uint32_t>(AccelerationStructureInstanceList.size());
    AccelerationStructureBuildRangeInfo.primitiveOffset = 0;
    AccelerationStructureBuildRangeInfo.firstVertex = 0;
    AccelerationStructureBuildRangeInfo.transformOffset = 0;
    std::vector<VkAccelerationStructureBuildRangeInfoKHR> AccelerationStructureBuildRangeInfoList = { AccelerationStructureBuildRangeInfo };

    TopLevelAccelerationStructure.AcclerationCommandBuffer(AccelerationStructureBuildGeometryInfo2, AccelerationStructureBuildRangeInfoList);

    scratchBuffer.DestoryBuffer();
    InstancesBuffer.DestoryBuffer();
}

void RayTraceRenderPass::Draw(SceneProperties& sceneProperties)
{
    VkCommandBufferBeginInfo cmdBufInfo{};
    cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    VkImageSubresourceRange subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

    vkBeginCommandBuffer(CommandBuffer[VulkanRenderer::GetCMDIndex()], &cmdBufInfo);
    const uint32_t handleSizeAligned = GraphicsDevice::GetAlignedSize(RayTracePipeline->GetRayTracingPipelineProperties().shaderGroupHandleSize, RayTracePipeline->GetRayTracingPipelineProperties().shaderGroupHandleAlignment);

    VkStridedDeviceAddressRegionKHR raygenShaderSbtEntry{};
    raygenShaderSbtEntry.deviceAddress = VulkanRenderer::GetBufferDeviceAddress(RayTracePipeline->GetRaygenShaderBindingTable());
    raygenShaderSbtEntry.stride = handleSizeAligned;
    raygenShaderSbtEntry.size = handleSizeAligned;

    VkStridedDeviceAddressRegionKHR missShaderSbtEntry{};
    missShaderSbtEntry.deviceAddress = VulkanRenderer::GetBufferDeviceAddress(RayTracePipeline->GetMissShaderBindingTable());
    missShaderSbtEntry.stride = handleSizeAligned;
    missShaderSbtEntry.size = handleSizeAligned;

    VkStridedDeviceAddressRegionKHR hitShaderSbtEntry{};
    hitShaderSbtEntry.deviceAddress = VulkanRenderer::GetBufferDeviceAddress(RayTracePipeline->GetHitShaderBindingTable());
    hitShaderSbtEntry.stride = handleSizeAligned;
    hitShaderSbtEntry.size = handleSizeAligned;


    vkCmdPushConstants(CommandBuffer[VulkanRenderer::GetCMDIndex()], RayTracePipeline->GetShaderPipelineLayout(), VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_MISS_BIT_KHR | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR, 0, sizeof(SceneProperties), &sceneProperties);
    vkCmdBindPipeline(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, RayTracePipeline->GetShaderPipeline());
    vkCmdBindDescriptorSets(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, RayTracePipeline->GetShaderPipelineLayout(), 0, 1, RayTracePipeline->GetDescriptorSetPtr(), 0, 0);

    VkStridedDeviceAddressRegionKHR CallableShaderSbtEntry{};
    RayTracedTexture->UpdateImageLayout(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_IMAGE_LAYOUT_GENERAL);
    VulkanRenderer::vkCmdTraceRaysKHR(CommandBuffer[VulkanRenderer::GetCMDIndex()], &raygenShaderSbtEntry, &missShaderSbtEntry, &hitShaderSbtEntry, &CallableShaderSbtEntry, VulkanRenderer::GetSwapChainResolution().width, VulkanRenderer::GetSwapChainResolution().height, 1);
    RayTracedTexture->UpdateImageLayout(CommandBuffer[VulkanRenderer::GetCMDIndex()], VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkEndCommandBuffer(CommandBuffer[VulkanRenderer::GetCMDIndex()]);
}

void RayTraceRenderPass::RebuildSwapChain()
{
    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();

    RayTracedTexture->RecreateRendererTexture(RenderPassResolution);
    RayTracePipeline->UpdateGraphicsPipeLine(TopLevelAccelerationStructure, RayTracedTexture);
}

void RayTraceRenderPass::Destroy()
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
