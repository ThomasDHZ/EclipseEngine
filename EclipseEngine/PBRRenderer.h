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
#include "ReflectionIrradianceRenderPass.h"
#include "ReflectionPrefilterRenderPass.h"

class PBRRenderer
{
private:
	MeshPickerRenderPass3D meshPickerRenderPass;
	EnvironmentToCubeRenderPass environmentToCubeRenderPass;
	BRDFRenderPass brdfRenderPass;
	ReflectionIrradianceRenderPass reflectionIrradianceRenderPass;
	ReflectionPrefilterRenderPass reflectionPrefilterRenderPass;
	PBRReflectionRenderPass pbrReflectionRenderPass;
	IrradianceRenderPass irradianceRenderPass;
	PrefilterRenderPass prefilterRenderPass;
	PBRRenderPass pbrRenderPass;
	FrameBufferRenderPass frameBufferRenderPass;

public:
	PBRRenderer();
	~PBRRenderer();

	void StartUp();
	void Update();
	void RebuildRenderers();
	void Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList);
	void Destroy();


	std::shared_ptr<RenderedColorTexture> GetColorPickerTexture() { return meshPickerRenderPass.RenderedTexture; }
};