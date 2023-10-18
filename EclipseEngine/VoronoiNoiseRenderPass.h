#pragma once
#include "GenerateImageRenderPass.h"

class VoronoiNoiseRenderPass : public GenerateImageRenderPass
{
private:
public:
	VoronoiNoiseRenderPass();
	~VoronoiNoiseRenderPass();

	void BuildRenderPass(glm::ivec2 textureSize);
	void OneTimeDraw(glm::ivec2 textureSize);
};