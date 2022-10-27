#pragma once
#include "MeshPickerRenderPass3D.h"
#include "BlinnPhongRenderPass.h"
#include "FrameBufferRenderPass.h"
#include "GBufferRenderPass.h"
#include "RaytraceHybridPass.h"
#include "DeferredRenderPass.h"
#include "EnvironmentToCubeRenderPass.h"

class HybridRenderer
{
private:
	MeshPickerRenderPass3D meshPickerRenderPass;
	EnvironmentToCubeRenderPass environmentToCubeRenderPass;
	RaytraceHybridPass raytraceHybridPass;
	FrameBufferRenderPass frameBufferRenderPass;

public:
	HybridRenderer();
	~HybridRenderer();

	GBufferRenderPass GBufferRenderPass;
	DeferredRenderPass deferredRenderPass;
	static std::string BaseShaderFilePath;
	void BuildRenderer();
	void Update();
	void ImGuiUpdate();
	void Draw(SceneProperties& sceneProperites, std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	void Destroy();


	std::shared_ptr<RenderedColorTexture> GetColorPickerTexture() { return meshPickerRenderPass.RenderedTexture; }
};