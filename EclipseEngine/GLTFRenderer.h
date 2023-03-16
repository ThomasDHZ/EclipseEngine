
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
#include "GLTF_EnvironmentToCubeRenderPass.h"
#include "GLTF_BRDFRenderPass.h"
#include "GLTFRenderPass.h"
#include "GLTFIrradianceRenderPass.h"
#include "GLTFPrefilterRenderPass.h"
#include "PerspectiveCamera.h"

class GLTFRenderer
{
private:
	std::shared_ptr<ModelRenderer> model;
	std::shared_ptr<Camera> camera;

	GLTF_EnvironmentToCubeRenderPass environmentToCubeRenderPass;
	GLTF_BRDFRenderPass GLTF_BRDFRenderPass;
	GLTFIrradianceRenderPass irradianceRenderPass;
	GLTFPrefilterRenderPass prefilterRenderPass;
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