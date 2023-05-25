
#pragma once
#include "MeshPickerRenderPass3D.h"
#include "FrameBufferRenderPass.h"
#include "PBRRenderPass.h"
#include "EnvironmentToCubeRenderPass.h"
#include "IrradianceRenderPass.h"
#include "PrefilterRenderPass.h"
#include "BRDFRenderPass.h"
#include "SceneManager.h"
#include "PBRReflectionRenderPass.h"
#include "DepthRenderPass.h"
#include "DepthDebugRenderPass.h"
#include "DepthCubeMapRenderer.h"
#include "PBRBloomRenderPass.h"
#include "GaussianBlurRenderPass.h"
#include "BloomCombineRenderPass.h"
#include "CubeToEnvironmentRenderPass.h"
#include "PBRReflectionPreRenderPass.h"
#include "EnvironmentToCubeRenderPass.h"
#include "GLTFRenderPass.h"
#include "IrradianceRenderPass.h"
#include "PrefilterRenderPass.h"
#include "GLTFSceneManager.h"
#include "PerspectiveCamera.h"
#include "VulkanPipelineEditor.h"

class GLTFRenderer
{
private:
	VulkanPipelineEditor pipelineEditor;
	EnvironmentToCubeRenderPass environmentToCubeRenderPass;
	BRDFRenderPass GLTF_BRDFRenderPass;
	IrradianceRenderPass irradianceRenderPass;
	PrefilterRenderPass prefilterRenderPass;
	GLTFRenderPass gLTFRenderPass;
	FrameBufferRenderPass frameBufferRenderPass;

public:
	GLTFRenderer();
	~GLTFRenderer();

	void BuildRenderer();
	void Update();
	void ImGuiUpdate();
	void Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	void Destroy();

	bool UpdateRenderer = true;
};