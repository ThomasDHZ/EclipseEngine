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

class PBRPlayRenderer
{
private:
	EnvironmentToCubeRenderPass environmentToCubeRenderPass;
	BRDFRenderPass brdfRenderPass;

	DepthRenderPass DepthPassRenderPass;
	DepthCubeMapRenderer DepthCubeMapRenderPass;

	PBRRenderPass pbrRenderPass;
	PBRBloomRenderPass pbrBloomRenderPass;
	GaussianBlurRenderPass blurRenderPass;
	BloomCombineRenderPass bloomCombinePipeline;

	FrameBufferRenderPass frameBufferRenderPass;

	void BuildCubeMaps();
public:
	PBRPlayRenderer();
	~PBRPlayRenderer();

	IrradianceRenderPass irradianceRenderPass;
	PrefilterRenderPass prefilterRenderPass;

	void BuildRenderer();
	void Update();
	void Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	void Destroy();
};