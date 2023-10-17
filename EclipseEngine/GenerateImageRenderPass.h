#pragma once
#include "RenderPass.h"
#include "RenderedColorTexture.h"
#include "JsonGraphicsPipeline.h"

class GenerateImageRenderPass : public RenderPass
{
private:
	virtual void RenderPassDesc();
	virtual void BuildRenderPassPipelines(const char* pipelineFileName);

protected:
	TextureCreatorProperties textureCreatorProperties;
	JsonGraphicsPipeline ImagePipeline;
	std::shared_ptr<RenderedColorTexture> ImageTexture;
public:
	GenerateImageRenderPass();
	~GenerateImageRenderPass();

	void BuildRenderPass(const char* pipelineFileName, glm::ivec2 textureSize);
	void OneTimeDraw(const char* pipelineFileName, glm::ivec2 textureSize);
	VkCommandBuffer Draw(float time);
	//void Save();
	void Destroy();
	void SaveTexture(const char* filename, BakeTextureFormat textureFormat);

	std::shared_ptr<RenderedColorTexture> GetImageTexture() { return ImageTexture; }
};

