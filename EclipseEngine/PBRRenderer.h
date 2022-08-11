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

class PBRRenderer
{
private:


	MeshPickerRenderPass3D meshPickerRenderPass;
	EnvironmentToCubeRenderPass environmentToCubeRenderPass;
	BRDFRenderPass brdfRenderPass;

	DepthRenderPass depthPassRendererPass;

	IrradianceRenderPass skyBoxIrradianceRenderPass;
	PrefilterRenderPass skyBoxPrefilterRenderPass;
	PBRReflectionRenderPass skyBoxPBRRenderPass;

	IrradianceRenderPass geoIrradianceRenderPass;
	PrefilterRenderPass geoPrefilterRenderPass;
	PBRReflectionRenderPass geoPBRRenderPass;

	IrradianceRenderPass irradianceRenderPass;
	PrefilterRenderPass prefilterRenderPass;
	PBRRenderPass pbrRenderPass;

	FrameBufferRenderPass frameBufferRenderPass;

public:
	PBRRenderer();
	~PBRRenderer();

	void BuildRenderer();
	void Update();
	void Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	void Destroy();

	bool PreRenderedFlag = false;

	DepthDebugRenderPass depthDebugRenderPass;
	std::shared_ptr<RenderedColorTexture> GetColorPickerTexture() { return meshPickerRenderPass.RenderedTexture; }
};