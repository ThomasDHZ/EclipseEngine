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
#include "PBRBakeReflectionRenderPass.h"

class PBREditorRenderer
{
private:
	PBRRenderPassTextureSubmitList bakesubmitList;
	DepthRenderPass bakeDepthPassRenderPass;
	DepthCubeMapRenderer bakeDepthCubeMapRenderPass;

	IrradianceRenderPass bakeskyIrradianceRenderPass;
	PrefilterRenderPass bakeskyPrefilterRenderPass;
	PBRReflectionRenderPass bakeskyPBRRenderPass;

	IrradianceRenderPass bakegeoIrradianceRenderPass;
	PrefilterRenderPass bakegeoPrefilterRenderPass;
	PBRReflectionRenderPass bakegeoPBRRenderPass;

	PBRBakeReflectionRenderPass BakeReflectionRenderPass;

	MeshPickerRenderPass3D meshPickerRenderPass;
	EnvironmentToCubeRenderPass environmentToCubeRenderPass;
	BRDFRenderPass brdfRenderPass;

	DepthRenderPass DepthPassRenderPass;
	DepthCubeMapRenderer DepthCubeMapRenderPass;
	//DepthRenderPass spotLightDepthPassRenderPassList;

	IrradianceRenderPass skyIrradianceRenderPass;
	PrefilterRenderPass skyPrefilterRenderPass;
	PBRReflectionRenderPass skyPBRRenderPass;

	IrradianceRenderPass geoIrradianceRenderPass;
	PrefilterRenderPass geoPrefilterRenderPass;
	PBRReflectionRenderPass geoPBRRenderPass;

	IrradianceRenderPass irradianceRenderPass;
	PrefilterRenderPass prefilterRenderPass;
	PBRRenderPass pbrRenderPass;
	PBRBloomRenderPass pbrBloomRenderPass;
	GaussianBlurRenderPass blurRenderPass;
	BloomCombineRenderPass bloomCombinePipeline;

	FrameBufferRenderPass frameBufferRenderPass;

public:
	PBREditorRenderer();
	~PBREditorRenderer();

	std::shared_ptr<BakedTexture> BakeTextures(const char* FileName);

	void BuildRenderer();
	void Update();
	void Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	void Destroy();

	bool UpdateRenderer = true;

	//DepthDebugRenderPass depthDebugRenderPass;
	CubeToEnvironmentRenderPass cubeToEnvironmentRenderPass;
	std::shared_ptr<RenderedColorTexture> GetColorPickerTexture() { return meshPickerRenderPass.RenderedTexture; }
};