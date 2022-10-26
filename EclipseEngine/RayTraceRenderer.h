#pragma once
#include "FrameBufferRenderPass.h"
#include "RayTraceRenderPass.h"
#include "MeshPickerRenderPass3D.h"
#include "EnvironmentToCubeRenderPass.h"

class RayTraceRenderer : public RenderPass
{
private:
	MeshPickerRenderPass3D meshPickerRenderPass;
	EnvironmentToCubeRenderPass environmentToCubeRenderPass;
	FrameBufferRenderPass FrameBufferRenderer;

public:
	RayTraceRenderer();
	~RayTraceRenderer();
	RayTraceRenderPass rayTraceRenderPass;
	static std::string BaseShaderFilePath;
	void BuildRenderer();
	void Update();
	void GUIUpdate();
	void Draw(SceneProperties& sceneProperties, std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	void Destroy();
};

