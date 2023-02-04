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
#include "GLTFPBRRenderPIpeline.h"
#include "GLTF_BRDFRenderPass.h"
#include "GLTFRenderPass.h"

class PBRRenderer
{
private:
	//MeshPickerRenderPass3D meshPickerRenderPass;
	//EnvironmentToCubeRenderPass environmentToCubeRenderPass;
	//BRDFRenderPass brdfRenderPass;

	//DepthRenderPass DepthPassRenderPass;
	//DepthCubeMapRenderer DepthCubeMapRenderPass;

	//IrradianceRenderPass skyIrradianceRenderPass;
	//PrefilterRenderPass skyPrefilterRenderPass;
	//PBRReflectionRenderPass skyPBRRenderPass;

	//IrradianceRenderPass geoIrradianceRenderPass;
	//PrefilterRenderPass geoPrefilterRenderPass;
	//PBRReflectionPreRenderPass ReflectionPreRenderPass;

	//IrradianceRenderPass irradianceRenderPass;
	//PrefilterRenderPass prefilterRenderPass;
	//PBRRenderPass pbrRenderPass;
	//PBRBloomRenderPass pbrBloomRenderPass;
	//GaussianBlurRenderPass blurRenderPass;
	//BloomCombineRenderPass bloomCombinePipeline;
	GLTF_Temp_Model model;
	GLTF_BRDFRenderPass GLTF_BRDFRenderPass;
	GLTFRenderPass gLTFRenderPass;
	FrameBufferRenderPass frameBufferRenderPass;

	//ComputeAnimationPipeline AnimationRenderer;

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