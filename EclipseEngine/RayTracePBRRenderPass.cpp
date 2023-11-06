#include "RayTracePBRRenderPass.h"
#include "EngineMath.h"

RayTracePBRRenderPass::RayTracePBRRenderPass() : RenderPass()
{
}

RayTracePBRRenderPass::~RayTracePBRRenderPass()
{
}

void RayTracePBRRenderPass::StartUp()
{
    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();
    RayTracedTexture = std::make_shared<RenderedColorTexture>(RenderedColorTexture(RenderPassResolution, VK_FORMAT_R8G8B8A8_UNORM));

    uint32_t PrimitiveCount = 1;
    VkDeviceOrHostAddressConstKHR DeviceOrHostAddressConst = {};

    VulkanBuffer InstanceBuffer = VulkanBuffer(nullptr, sizeof(VkAccelerationStructureInstanceKHR), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    DeviceOrHostAddressConst.deviceAddress = VulkanRenderer::GetBufferDeviceAddress(InstanceBuffer.GetBuffer());

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

    TopLevelAccelerationStructure.CreateAccelerationStructure(VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR, accelerationStructureBuildSizesInfo);

    scratchBuffer = VulkanBuffer(nullptr, accelerationStructureBuildSizesInfo.buildScratchSize, VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    scratchBuffer.SetBufferAddress(VulkanRenderer::GetBufferDeviceAddress(scratchBuffer.GetBuffer()));

    InstanceBuffer.DestroyBuffer();

    GLTFSceneManager::Update();

    BuildRenderPassPipelines();
    SetUpCommandBuffers();
    UpdateTopLevelAccelerationStructure();
}

void RayTracePBRRenderPass::Update()
{
    GLTFSceneManager::Update();
    if (GraphicsDevice::IsRayTracingFeatureActive())
    {
        std::vector<VkAccelerationStructureInstanceKHR> AccelerationStructureInstanceList = {};


        for (int x = 0; x < GLTFSceneManager::GameObjectList.size(); x++)
        {
            GLTFSceneManager::GameObjectList[x]->GetGameObjectRenderer()->UpdateModelTopLevelAccelerationStructure(AccelerationStructureInstanceList, x);
        }

        VulkanBuffer InstanceBuffer = VulkanBuffer(AccelerationStructureInstanceList.data(), sizeof(VkAccelerationStructureInstanceKHR) * AccelerationStructureInstanceList.size(), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        DeviceOrHostAddressConst.deviceAddress = VulkanRenderer::GetBufferDeviceAddress(InstanceBuffer.GetBuffer());

        VkAccelerationStructureGeometryKHR AccelerationStructureGeometry{};
        AccelerationStructureGeometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
        AccelerationStructureGeometry.geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR;
        AccelerationStructureGeometry.flags = VK_GEOMETRY_NO_DUPLICATE_ANY_HIT_INVOCATION_BIT_KHR;
        AccelerationStructureGeometry.geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
        AccelerationStructureGeometry.geometry.instances.arrayOfPointers = VK_FALSE;
        AccelerationStructureGeometry.geometry.instances.data = DeviceOrHostAddressConst;

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
        InstanceBuffer.DestroyBuffer();
    }
}

void RayTracePBRRenderPass::SetUpCommandBuffers()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = VulkanRenderer::GetCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(VulkanRenderer::GetDevice(), &allocInfo, &RayTraceCommandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void RayTracePBRRenderPass::UpdateTopLevelAccelerationStructure()
{
    if (GraphicsDevice::IsRayTracingFeatureActive())
    {
        std::vector<VkAccelerationStructureInstanceKHR> AccelerationStructureInstanceList = {};


        for (int x = 0; x < GLTFSceneManager::GameObjectList.size(); x++)
        {
            GLTFSceneManager::GameObjectList[x]->GetGameObjectRenderer()->UpdateModelTopLevelAccelerationStructure(AccelerationStructureInstanceList, 0);
        }

        VulkanBuffer InstanceBuffer = VulkanBuffer(AccelerationStructureInstanceList.data(), sizeof(VkAccelerationStructureInstanceKHR) * AccelerationStructureInstanceList.size(), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        DeviceOrHostAddressConst.deviceAddress = VulkanRenderer::GetBufferDeviceAddress(InstanceBuffer.GetBuffer());

        VkAccelerationStructureGeometryKHR AccelerationStructureGeometry{};
        AccelerationStructureGeometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
        AccelerationStructureGeometry.geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR;
        AccelerationStructureGeometry.flags = VK_GEOMETRY_NO_DUPLICATE_ANY_HIT_INVOCATION_BIT_KHR;
        AccelerationStructureGeometry.geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
        AccelerationStructureGeometry.geometry.instances.arrayOfPointers = VK_FALSE;
        AccelerationStructureGeometry.geometry.instances.data = DeviceOrHostAddressConst;

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
        InstanceBuffer.DestroyBuffer();
    }
}

void RayTracePBRRenderPass::BuildRenderPassPipelines()
{
    std::vector<VkPipelineShaderStageCreateInfo> ShaderList;
    std::vector<VkRayTracingShaderGroupCreateInfoKHR> RayTraceShaderList;
    {
        ShaderList.emplace_back(CreateShader("EclipseEngine/PBRRayGenShader.hlsl", VK_SHADER_STAGE_RAYGEN_BIT_KHR));
        VkRayTracingShaderGroupCreateInfoKHR RayGeneratorShaderInfo = {};
        RayGeneratorShaderInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
        RayGeneratorShaderInfo.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
        RayGeneratorShaderInfo.generalShader = static_cast<uint32_t>(ShaderList.size()) - 1;
        RayGeneratorShaderInfo.closestHitShader = VK_SHADER_UNUSED_KHR;
        RayGeneratorShaderInfo.anyHitShader = VK_SHADER_UNUSED_KHR;
        RayGeneratorShaderInfo.intersectionShader = VK_SHADER_UNUSED_KHR;
        RayTraceShaderList.emplace_back(RayGeneratorShaderInfo);

        ShaderList.emplace_back(CreateShader("../Shaders/PBRMissrmiss.spv", VK_SHADER_STAGE_MISS_BIT_KHR));
        VkRayTracingShaderGroupCreateInfoKHR MissShaderInfo = {};
        MissShaderInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
        MissShaderInfo.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
        MissShaderInfo.generalShader = static_cast<uint32_t>(ShaderList.size()) - 1;
        MissShaderInfo.closestHitShader = VK_SHADER_UNUSED_KHR;
        MissShaderInfo.anyHitShader = VK_SHADER_UNUSED_KHR;
        MissShaderInfo.intersectionShader = VK_SHADER_UNUSED_KHR;
        RayTraceShaderList.emplace_back(MissShaderInfo);

        ShaderList.emplace_back(CreateShader("../Shaders/PBRShadowrmiss.spv", VK_SHADER_STAGE_MISS_BIT_KHR));
        VkRayTracingShaderGroupCreateInfoKHR ShadowShaderInfo = {};
        ShadowShaderInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
        ShadowShaderInfo.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
        ShadowShaderInfo.generalShader = static_cast<uint32_t>(ShaderList.size()) - 1;
        ShadowShaderInfo.closestHitShader = VK_SHADER_UNUSED_KHR;
        ShadowShaderInfo.anyHitShader = VK_SHADER_UNUSED_KHR;
        ShadowShaderInfo.intersectionShader = VK_SHADER_UNUSED_KHR;
        RayTraceShaderList.emplace_back(ShadowShaderInfo);

        ShaderList.emplace_back(CreateShader("EclipseEngine/PBRClosestHitShader.hlsl", VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR));
        VkRayTracingShaderGroupCreateInfoKHR ClosestHitShaderInfo = {};
        ClosestHitShaderInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
        ClosestHitShaderInfo.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR;
        ClosestHitShaderInfo.generalShader = VK_SHADER_UNUSED_KHR;
        ClosestHitShaderInfo.closestHitShader = static_cast<uint32_t>(ShaderList.size()) - 1;

        ShaderList.emplace_back(CreateShader("../Shaders/PBRAnyhitrahit.spv", VK_SHADER_STAGE_ANY_HIT_BIT_KHR));
        ClosestHitShaderInfo.anyHitShader = static_cast<uint32_t>(ShaderList.size()) - 1;
        ClosestHitShaderInfo.intersectionShader = VK_SHADER_UNUSED_KHR;
        RayTraceShaderList.emplace_back(ClosestHitShaderInfo);

        ShaderList.emplace_back(CreateShader("../Shaders/PBRAnyhit1rahit.spv", VK_SHADER_STAGE_ANY_HIT_BIT_KHR));
        VkRayTracingShaderGroupCreateInfoKHR ShadwoHitShaderInfo = {};
        ShadwoHitShaderInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
        ShadwoHitShaderInfo.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR;
        ShadwoHitShaderInfo.generalShader = VK_SHADER_UNUSED_KHR;
        ShadwoHitShaderInfo.closestHitShader = VK_SHADER_UNUSED_KHR;
        ShadwoHitShaderInfo.anyHitShader = static_cast<uint32_t>(ShaderList.size()) - 1;
        ShadwoHitShaderInfo.intersectionShader = VK_SHADER_UNUSED_KHR;
        RayTraceShaderList.emplace_back(ShadwoHitShaderInfo);
    }

    std::vector<VkDescriptorPoolSize> descriptorPoolSizeList;

    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR, 1 });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE  , 1 });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetVertexPropertiesBuffer().size() });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetIndexPropertiesBuffer().size() });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetGameObjectPropertiesBuffer().size() });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetGameObjectTransformBuffer().size() });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetMaterialPropertiesBuffer().size() });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, (uint32_t)GLTFSceneManager::GetTexturePropertiesBuffer().size() });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetSunLightPropertiesBuffer().size() });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetDirectionalLightPropertiesBuffer().size() });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetPointLightPropertiesBuffer().size() });
    descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetSpotLightPropertiesBuffer().size() });
    DescriptorPool = GLTF_GraphicsDescriptors::CreateDescriptorPool(descriptorPoolSizeList);

    std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBinding;
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 0, VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR, 1, VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 1, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetVertexPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 3, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetIndexPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 4, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetGameObjectPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 5, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetGameObjectTransformBuffer().size(), VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 6, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetMaterialPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 7, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, (uint32_t)GLTFSceneManager::GetTexturePropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 8, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 9, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 10, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 11, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetSunLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 12, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetDirectionalLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 13, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetPointLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
    descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 14, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetSpotLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
    DescriptorSetLayout = GLTF_GraphicsDescriptors::CreateDescriptorSetLayout(descriptorSetLayoutBinding);

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    VkWriteDescriptorSetAccelerationStructureKHR AccelerationDescriptorStructure = GLTF_GraphicsDescriptors::AddAcclerationStructureBinding(DescriptorBindingList, TopLevelAccelerationStructure.GetAccelerationStructureHandlePtr());
    VkDescriptorImageInfo RayTracedTextureMaskDescriptor = GLTF_GraphicsDescriptors::AddRayTraceStorageImageDescriptor(DescriptorBindingList, VK_IMAGE_LAYOUT_GENERAL, RayTracedTexture->View);

    GLTF_GraphicsDescriptors::AddAccelerationDescriptorSetBinding(DescriptorBindingList, 0, AccelerationDescriptorStructure);
    GLTF_GraphicsDescriptors::AddStorageTextureSetBinding(DescriptorBindingList, 1, RayTracedTextureMaskDescriptor);
    GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 2, GLTFSceneManager::GetVertexPropertiesBuffer());
    GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 3, GLTFSceneManager::GetIndexPropertiesBuffer());
    GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 4, GLTFSceneManager::GetGameObjectPropertiesBuffer());
    GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 5, GLTFSceneManager::GetGameObjectTransformBuffer());
    GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 6, GLTFSceneManager::GetMaterialPropertiesBuffer());
    GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 7, GLTFSceneManager::GetTexturePropertiesBuffer());
    GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 8, GLTFSceneManager::GetBRDFMapDescriptor());
    GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 9, GLTFSceneManager::GetIrradianceMapDescriptor());
    GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 10, GLTFSceneManager::GetPrefilterMapDescriptor());
    GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 11, GLTFSceneManager::GetSunLightPropertiesBuffer());
    GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 12, GLTFSceneManager::GetDirectionalLightPropertiesBuffer());
    GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 13, GLTFSceneManager::GetPointLightPropertiesBuffer());
    GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 14, GLTFSceneManager::GetSpotLightPropertiesBuffer());
    DescriptorSet = GLTF_GraphicsDescriptors::CreateDescriptorSets(DescriptorPool, DescriptorSetLayout);

    std::vector<VkWriteDescriptorSet> writeDescriptorSet;
    for (auto& DescriptorBinding : DescriptorBindingList)
    {
        if (DescriptorBinding.BufferDescriptor.size() > 0)
        {
            writeDescriptorSet.emplace_back(GLTF_GraphicsDescriptors::AddBufferDescriptorSet(DescriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.BufferDescriptor, DescriptorBinding.DescriptorType));
        }
        else if (DescriptorBinding.TextureDescriptor.size() > 0)
        {
            writeDescriptorSet.emplace_back(GLTF_GraphicsDescriptors::AddTextureDescriptorSet(DescriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.TextureDescriptor, DescriptorBinding.DescriptorType));
        }
        else
        {
            writeDescriptorSet.emplace_back(GLTF_GraphicsDescriptors::AddAccelerationBuffer(DescriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.AccelerationStructureDescriptor));
        }
    }
    vkUpdateDescriptorSets(VulkanRenderer::GetDevice(), static_cast<uint32_t>(writeDescriptorSet.size()), writeDescriptorSet.data(), 0, nullptr);


    if (RayTracePipeline == nullptr)
    {
        RayTracePipeline = std::make_shared<RayTracingPipeline>();
        RayTracePipeline->SetUp(ShaderList, RayTraceShaderList, DescriptorBindingList);
    }
    else
    {
        RayTracePipeline->UpdateGraphicsPipeLine(ShaderList, RayTraceShaderList, DescriptorBindingList);
    }
}

VkCommandBuffer RayTracePBRRenderPass::Draw()
{
    VkCommandBufferBeginInfo cmdBufInfo{};
    cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

    VkImageSubresourceRange subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

    vkBeginCommandBuffer(RayTraceCommandBuffer, &cmdBufInfo);
    const uint32_t handleSizeAligned = GraphicsDevice::GetAlignedSize(GraphicsDevice::GetRayTracingPipelineProperties().shaderGroupHandleSize, GraphicsDevice::GetRayTracingPipelineProperties().shaderGroupHandleAlignment);

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

    GLTFSceneManager::sceneProperites.proj = glm::inverse(GLTFSceneManager::sceneProperites.proj);
    GLTFSceneManager::sceneProperites.view = glm::inverse(GLTFSceneManager::sceneProperites.view);

    vkCmdPushConstants(RayTraceCommandBuffer, RayTracePipeline->GetShaderPipelineLayout(), VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_MISS_BIT_KHR | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_ANY_HIT_BIT_KHR, 0, sizeof(SceneProperties), &GLTFSceneManager::sceneProperites);
    vkCmdBindPipeline(RayTraceCommandBuffer, VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, RayTracePipeline->GetShaderPipeline());
    vkCmdBindDescriptorSets(RayTraceCommandBuffer, VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, RayTracePipeline->GetShaderPipelineLayout(), 0, 1, RayTracePipeline->GetDescriptorSetPtr(), 0, 0);

    VkStridedDeviceAddressRegionKHR CallableShaderSbtEntry{};
    RayTracedTexture->UpdateImageLayout(RayTraceCommandBuffer, VK_IMAGE_LAYOUT_GENERAL);
    VulkanRenderer::vkCmdTraceRaysKHR(RayTraceCommandBuffer, &raygenShaderSbtEntry, &missShaderSbtEntry, &hitShaderSbtEntry, &CallableShaderSbtEntry, VulkanRenderer::GetSwapChainResolution().width, VulkanRenderer::GetSwapChainResolution().height, 1);
    RayTracedTexture->UpdateImageLayout(RayTraceCommandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkEndCommandBuffer(RayTraceCommandBuffer);
    return RayTraceCommandBuffer;
}

void RayTracePBRRenderPass::RebuildSwapChain()
{
    RenderPassResolution = VulkanRenderer::GetSwapChainResolutionVec2();

    RayTracedTexture->RecreateRendererTexture(RenderPassResolution);
  //  BuildRenderPassPipelines();
}

void RayTracePBRRenderPass::Destroy()
{
    RayTracedTexture->Destroy();
    RayTracePipeline->Destroy();

    RenderPass::Destroy();
}
