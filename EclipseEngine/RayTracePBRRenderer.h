#pragma once
#include "RenderPass.h"
#include "MeshPickerRenderPass3D.h"
#include "EnvironmentToCubeRenderPass.h"
#include "FrameBufferRenderPass.h"
#include "RayTracePBRRenderPass.h"

class RayTracePBRRenderer : public RenderPass
{
private:
	//MeshPickerRenderPass3D meshPickerRenderPass;
	EnvironmentToCubeRenderPass environmentToCubeRenderPass;
	FrameBufferRenderPass FrameBufferRenderer;

public:
	RayTracePBRRenderer();
	~RayTracePBRRenderer();

	RayTracePBRRenderPass rayTracePBRRenderPass;

	static std::string BaseShaderFilePath;
	void BuildRenderer();
	void Update();
	void ImGuiUpdate();
	void Draw(SceneProperties& sceneProperties, std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	void Destroy();
};