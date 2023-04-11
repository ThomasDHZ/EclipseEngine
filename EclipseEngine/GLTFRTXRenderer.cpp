#include "GLTFRTXRenderer.h"
#include <PerspectiveCamera.h>

void GLTFRTXRenderer::UpdateTopLevelAccelerationStructure()
{
    if (GraphicsDevice::IsRayTracingFeatureActive())
    {
        std::vector<VkAccelerationStructureInstanceKHR> AccelerationStructureInstanceList = {};


        for (int x = 0; x < gameObjectList.size(); x++)
        {
            gameObjectList[x]->GetGameObjectRenderer()->UpdateModelTopLevelAccelerationStructure(AccelerationStructureInstanceList, 1);
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

GLTFRTXRenderer::GLTFRTXRenderer()
{
}

GLTFRTXRenderer::~GLTFRTXRenderer()
{
}

void GLTFRTXRenderer::BuildRenderer()
{
	GLTFSceneManager::StartUp();
    std::string a = "C:/Users/dotha/source/repos/EclipseEngine/Models/GLTFSponza/Sponza.gltf";
    auto b = "C:/Users/dotha/source/repos/EclipseEngine/Models/GLTFIron/Iron.gltf";
    auto d = "C:/Users/dotha/source/repos/EclipseEngine/Models/GLTFGold/Gold.gltf";
    auto c = "C:/Users/dotha/source/repos/EclipseEngine/Models/glTF-Sample-Models-master/2.0/SciFiHelmet/glTF/SciFiHelmet.gltf";

    GLTFSceneManager::ActiveCamera = std::make_shared<PerspectiveCamera>(PerspectiveCamera("DefaultCamera", VulkanRenderer::GetSwapChainResolutionVec2(), glm::vec3(0.0f, 0.0f, 5.0f)));

    gameObjectList.emplace_back(std::make_shared<GameObject3D>(GameObject3D("Sphere", GameObjectRenderType::kModelRenderer)));
    gameObjectList.back()->LoadRenderObject<Vertex3D>(b);

    rayTraceRenderer.StartUp();
	frameBufferRenderPass.BuildRenderPass(rayTraceRenderer.RayTracedTexture);
}

void GLTFRTXRenderer::Update()
{
	GLTFSceneManager::Update();
	for (auto& obj : gameObjectList)
	{
		obj->Update(VulkanRenderer::GetFrameTimeDurationMilliseconds());
	}
}

void GLTFRTXRenderer::ImGuiUpdate()
{
}

void GLTFRTXRenderer::Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	//CommandBufferSubmitList.emplace_back(rayTraceRenderer.Draw(gameObjectList[0]));
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
}

void GLTFRTXRenderer::Destroy()
{
	GLTFSceneManager::Destroy();
	for (auto& obj : gameObjectList)
	{
		obj->Destroy();
	}

    rayTraceRenderer.Destroy();
	frameBufferRenderPass.Destroy();
}
