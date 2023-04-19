#pragma once
#include "GraphicsPipeline.h"
#include "SceneManager.h"
#include "Mesh.h"
#include "GLTFSceneManager.h"

class DeferredRendererPipeline : public GraphicsPipeline
{
public:
	DeferredRendererPipeline();
	~DeferredRendererPipeline();

	void InitializePipeline(std::shared_ptr<RenderedColorTexture> PositionTexture,
        std::shared_ptr<RenderedColorTexture> TangentTexture,
        std::shared_ptr<RenderedColorTexture> BiTangentTexture,
        std::shared_ptr<RenderedColorTexture> TBNormalTexture,
        std::shared_ptr<RenderedColorTexture> NormalTexture,
        std::shared_ptr<RenderedColorTexture> AlbedoTexture,
        std::shared_ptr<RenderedColorTexture> SpecularTexture,
        std::shared_ptr<RenderedColorTexture> BloomTexture,
        std::shared_ptr<RenderedColorTexture> ShadowTexture,
        PipelineInfoStruct& pipelineInfoStruct);
	void Draw(VkCommandBuffer& commandBuffer);
};
