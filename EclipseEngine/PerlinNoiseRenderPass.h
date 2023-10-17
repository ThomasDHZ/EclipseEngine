#pragma once
#include "GenerateImageRenderPass.h"

class PerlinNoiseRenderPass : public GenerateImageRenderPass
{
private:
public:
	PerlinNoiseRenderPass();
	~PerlinNoiseRenderPass();

	void BuildRenderPass(glm::ivec2 textureSize);
	void OneTimeDraw(glm::ivec2 textureSize);
};