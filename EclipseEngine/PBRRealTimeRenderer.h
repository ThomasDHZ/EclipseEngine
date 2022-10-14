#pragma once
#include "MeshPickerRenderPass3D.h"
#include "EnvironmentToCubeRenderPass.h"
#include "BRDFRenderPass.h"
#include "DepthRenderPass.h"
#include "DepthCubeMapRenderer.h"
#include "IrradianceRenderPass.h"
#include "PrefilterRenderPass.h"
#include "PBRReflectionRenderPass.h"
#include "PBRRenderPass.h"
#include "FrameBufferRenderPass.h"
#include "PBRBloomRenderPass.h"
#include "GaussianBlurRenderPass.h"
#include "BloomCombineRenderPass.h"
#include "PBRBakeReflectionRenderPass.h"

class PBRRealTimeRenderer
{
private:
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
	PBRBakeReflectionRenderPass geoPBRRenderPass;

	IrradianceRenderPass irradianceRenderPass;
	PrefilterRenderPass prefilterRenderPass;
	PBRRenderPass pbrRenderPass;
	PBRBloomRenderPass pbrBloomRenderPass;
	GaussianBlurRenderPass blurRenderPass;
	BloomCombineRenderPass bloomCombinePipeline;

	FrameBufferRenderPass frameBufferRenderPass;

public:
	PBRRealTimeRenderer();
	~PBRRealTimeRenderer();

	void BuildRenderer();
	void Update();
	void Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	void Destroy();

	bool PreRenderedFlag = false;
	bool UpdatePreRenderer = true;

	//DepthDebugRenderPass depthDebugRenderPass;
	std::shared_ptr<RenderedColorTexture> GetBloomTexture() { return pbrBloomRenderPass.BloomMapList[0]; }
	std::shared_ptr<RenderedColorTexture> GetColorPickerTexture() { return meshPickerRenderPass.RenderedTexture; }
};