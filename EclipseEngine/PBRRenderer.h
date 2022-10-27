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

class PBRRenderer
{
private:
	MeshPickerRenderPass3D meshPickerRenderPass;
	EnvironmentToCubeRenderPass environmentToCubeRenderPass;
	BRDFRenderPass brdfRenderPass;

	DepthRenderPass DepthPassRenderPass;
	DepthCubeMapRenderer DepthCubeMapRenderPass;

	IrradianceRenderPass skyIrradianceRenderPass;
	PrefilterRenderPass skyPrefilterRenderPass;
	PBRReflectionRenderPass skyPBRRenderPass;

	IrradianceRenderPass geoIrradianceRenderPass;
	PrefilterRenderPass geoPrefilterRenderPass;
	PBRReflectionPreRenderPass ReflectionPreRenderPass;

	IrradianceRenderPass irradianceRenderPass;
	PrefilterRenderPass prefilterRenderPass;
	PBRRenderPass pbrRenderPass;
	//PBRBloomRenderPass pbrBloomRenderPass;
	//GaussianBlurRenderPass blurRenderPass;
	//BloomCombineRenderPass bloomCombinePipeline;

	FrameBufferRenderPass frameBufferRenderPass;

public:
	PBRRenderer();
	~PBRRenderer();

	void BakeTextures(const char* FileName);

	void BuildRenderer();
	void Update();
	void ImGuiUpdate();
	void Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	void Destroy();

	bool UpdateRenderer = true;

	std::shared_ptr<RenderedColorTexture> GetColorPickerTexture() { return meshPickerRenderPass.RenderedTexture; }
};