
#pragma once
#include "MeshPickerRenderPass3D.h"
#include "FrameBufferRenderPass.h"
#include "PBRRenderPass.h"
#include "EnvironmentToCubeRenderPass.h"
#include "IrradianceRenderPass.h"
#include "PrefilterRenderPass.h"
#include "BRDFRenderPass.h"
#include "SceneManager.h"
#include "PBRReflectionMeshRenderPass.h"
#include "DepthRenderPass.h"
#include "DepthDebugRenderPass.h"
#include "DepthCubeMapRenderer.h"
#include "PBRBloomRenderPass.h"
#include "GaussianBlurRenderPass.h"
#include "BloomCombineRenderPass.h"
#include "CubeToEnvironmentRenderPass.h"
#include "PBRReflectionMeshRenderPass.h"
#include "EnvironmentToCubeRenderPass.h"
#include "GLTFRenderPass.h"
#include "IrradianceRenderPass.h"
#include "PrefilterRenderPass.h"
#include "GLTFSceneManager.h"
#include "PerspectiveCamera.h"
#include "VulkanPipelineEditor.h"
#include "PBRReflectionSkyRenderPass.h"

class GLTFRenderer
{
private:
	VulkanPipelineEditor pipelineEditor;
	EnvironmentToCubeRenderPass environmentToCubeRenderPass;
	BRDFRenderPass GLTF_BRDFRenderPass;

	//SkyBox Reflection
	IrradianceRenderPass skyBoxReflectionIrradianceRenderPass;
	PrefilterRenderPass skyBoxReflectionPrefilterRenderPass;
	PBRReflectionMeshRenderPass skyBoxReflectionRenderPass;

	//Mesh Reflection
	IrradianceRenderPass meshReflectionIrradianceRenderPass;
	PrefilterRenderPass meshReflectionPrefilterRenderPass;
	PBRReflectionMeshRenderPass meshReflectionRenderPass;
	//Main
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