#pragma once
#include "MeshPickerRenderPass3D.h"
#include "BlinnPhongRenderPass.h"
#include "FrameBufferRenderPass.h"
#include "InterfaceRenderPass.h"
#include "RaytraceHybridPass.h"
#include "DepthRenderPass.h"
#include "DepthDebugRenderPass.h"

class BlinnPhongRenderer
{
private:
	MeshPickerRenderPass3D meshPickerRenderPass;
	//DepthRenderPass depthRenderPass;
	BlinnPhongRenderPass blinnPhongRenderPass;
	FrameBufferRenderPass frameBufferRenderPass;

public:
	BlinnPhongRenderer();
	~BlinnPhongRenderer();

//	DepthDebugRenderPass depthDebugRenderPass;

	void BuildRenderer();
	void Update();
	void Draw(SceneProperties& sceneProperties, ConstSkyBoxView& skyboxView, std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	void Destroy();


	std::shared_ptr<RenderedColorTexture> GetColorPickerTexture() { return meshPickerRenderPass.RenderedTexture; }
};

