#pragma once
#include "FrameBufferRenderPass.h"
#include "MeshPickerRenderPass3D.h"
#include "EnvironmentToCubeRenderPass.h"
#include "RayTracePBRRenderPass.h"
#include "PerspectiveCamera.h"

class RayTracePBRRenderer : public RenderPass
{
private:
	RayTracePBRRenderPass rayTraceRenderPass;
	FrameBufferRenderPass frameBufferRenderPass;
	//ComputeAnimationPipeline AnimationRenderer;
public:
	RayTracePBRRenderer();
	~RayTracePBRRenderer();

	void BuildRenderer();
	void Update();
	void ImGuiUpdate();
	void Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	void Destroy();
};
