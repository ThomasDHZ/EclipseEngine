//#pragma once
//#include "FrameBufferRenderPass.h"
//#include "RayTracePBRRenderer.h"
//
//class GLTFRTXRenderer
//{
//private:
//	std::vector<std::shared_ptr<GameObject>> gameObjectList;
//	RayTraceRenderer rayTraceRenderer;
//	FrameBufferRenderPass frameBufferRenderPass;
//
//	AccelerationStructureBuffer TopLevelAccelerationStructure;
//	VkDeviceOrHostAddressConstKHR DeviceOrHostAddressConst;
//	VulkanBuffer scratchBuffer;
//
//	void UpdateTopLevelAccelerationStructure();
//public:
//	GLTFRTXRenderer();
//	~GLTFRTXRenderer();
//
//	void BuildRenderer();
//	void Update();
//	void ImGuiUpdate();
//	void Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList);
//	void Destroy();
//};
//
