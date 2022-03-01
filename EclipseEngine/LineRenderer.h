#pragma once
#include "ComponentRenderer.h"

class LineRenderer : public ComponentRenderer
{
private:

public:
	LineRenderer();
	LineRenderer(std::vector<Vertex> VertexList);
	LineRenderer(glm::vec3 StartPoint, glm::vec3 EndPoint);
	LineRenderer(nlohmann::json& json);
	virtual ~LineRenderer();

	void Update(float DeltaTime) override;
	void Destroy() override;
};

