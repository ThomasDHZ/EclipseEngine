#pragma once
#include "MeshPickerRenderPass3D.h"
#include "FrameBufferRenderPass.h"
#include "PBRRenderPass.h"
#include "EnvironmentToCubeRenderPass.h"

class PBRRenderer
{
private:
	MeshPickerRenderPass3D meshPickerRenderPass;
	EnvironmentToCubeRenderPass environmentToCubeRenderPass;
	PBRRenderPass pbrRenderPass;
	FrameBufferRenderPass frameBufferRenderPass;

public:
	PBRRenderer();
	~PBRRenderer();

	void StartUp();
	void Update();
	void RebuildRenderers();
	void Draw(SceneProperties& sceneProperties, ConstSkyBoxView& skyboxView, std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	void Destroy();


	std::shared_ptr<RenderedColorTexture> GetColorPickerTexture() { return meshPickerRenderPass.RenderedTexture; }
};