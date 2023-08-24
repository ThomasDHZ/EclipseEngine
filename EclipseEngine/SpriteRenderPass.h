#pragma once
#include <vector>
#include "Vertex.h"
class SpriteRenderPass
{
private:
	std::vector<Vertex2D> RenderPassVertexList;
	std::vector<uint32_t> RenderPassIndexList;

public:
	SpriteRenderPass();
	~SpriteRenderPass();


};

