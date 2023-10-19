#pragma once
#include "GenerateImageRenderPass.h"

class MultiplyRenderPass : public GenerateImageRenderPass
{
private:
public:
	MultiplyRenderPass();
	~MultiplyRenderPass();

	void BuildRenderPass(glm::ivec2 textureSize, std::shared_ptr<Texture> texture1, std::shared_ptr<Texture> texture2);
	void OneTimeDraw(glm::ivec2 textureSize, std::shared_ptr<Texture> texture1, std::shared_ptr<Texture> texture2);
};

