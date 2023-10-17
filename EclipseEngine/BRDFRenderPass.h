#pragma once
#include "GenerateImageRenderPass.h"

class BRDFRenderPass : public GenerateImageRenderPass
{
private:
public:
	BRDFRenderPass();
	~BRDFRenderPass();

	void BuildRenderPass(uint32_t textureSize);
	void OneTimeDraw(uint32_t textureSize);
};