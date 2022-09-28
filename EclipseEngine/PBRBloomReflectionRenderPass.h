//#pragma once
//#include "RenderPass.h"
//#include "PBRBloomReflectionPipeline.h"
//#include "RenderedCubeMapDepthTexture.h"
//
//class PBRBloomReflectionRenderPass : public RenderPass
//{
//private:
//	uint32_t TextureMapMipLevels = 0;
//
//	void RenderPassDesc();
//	void BuildRenderPassPipelines(PBRRenderPassTextureSubmitList& textures);
//
//	std::shared_ptr<RenderedCubeMapTexture> DrawToBloomMap;
//	std::shared_ptr<RenderedCubeMapDepthTexture> DepthTexture;
//	PBRBloomReflectionPipeline pbrBloomPipeline;
//
//public:
//	PBRBloomReflectionRenderPass();
//	~PBRBloomReflectionRenderPass();
//
//
//	std::vector<std::shared_ptr<RenderedCubeMapTexture>> BloomMapList;
//
//	void BuildRenderPass(PBRRenderPassTextureSubmitList& textures);
//	void OneTimeDraw(PBRRenderPassTextureSubmitList& textures);
//	VkCommandBuffer Draw();
//	void Destroy();
//};