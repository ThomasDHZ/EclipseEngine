#pragma once
#include "GenerateImageRenderPass.h"

class ValueNoiseRenderPass : public GenerateImageRenderPass
{
private:
public:
	ValueNoiseRenderPass();
	~ValueNoiseRenderPass();

	void BuildRenderPass(glm::ivec2 textureSize);
	void OneTimeDraw(glm::ivec2 textureSize);
};

