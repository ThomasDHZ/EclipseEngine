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
#include "EnvironmentToCubeRenderPass.h"
#include "IrradianceRenderPass.h"
#include "PrefilterRenderPass.h"
#include "GLTFSceneManager.h"
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"
#include "VulkanPipelineEditor.h"
#include "DepthCubeDebugRenderPass.h"
#include "LightManagerMenu.h"

class PBRRenderer
{
private:
	glm::vec3 pos = glm::vec3(1000.0f, 2000.0f, 0.0f);
	VulkanPipelineEditor pipelineEditor;
	EnvironmentToCubeRenderPass environmentToCubeRenderPass;
	BRDFRenderPass brdfRenderPass;

	//Depth/Shadow Pass
	DepthRenderPass depthRenderPass;
	DepthCubeMapRenderer depthCubeMapRenderPass;

	//SkyBox Reflection
	IrradianceRenderPass skyBoxReflectionIrradianceRenderPass;
	PrefilterRenderPass skyBoxReflectionPrefilterRenderPass;
	PBRReflectionRenderPass skyBoxReflectionRenderPass;

	////Mesh Reflection
	IrradianceRenderPass meshReflectionIrradianceRenderPass;
	PrefilterRenderPass meshReflectionPrefilterRenderPass;
	PBRReflectionRenderPass meshReflectionRenderPass;

	//Main
	IrradianceRenderPass irradianceRenderPass;
	PrefilterRenderPass prefilterRenderPass;
	PBRRenderPass gLTFRenderPass;

	FrameBufferRenderPass frameBufferRenderPass;

	LightManagerMenu lightManagerMenu;

public:
	PBRRenderer();
	~PBRRenderer();

	void BuildRenderer();
	void Update();
	void ImGuiUpdate();
	void Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	void Destroy();

	bool UpdateRenderer = true;
};