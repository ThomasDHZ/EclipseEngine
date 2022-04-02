#pragma once
#include "ComponentRenderer.h"
#include "Model.h"

class LineRenderer : public ComponentRenderer
{
private:
	std::shared_ptr<Model> model;
public:
	LineRenderer();
	LineRenderer(std::vector<LineVertex> VertexList);
	LineRenderer(std::vector<LineVertex> VertexList, glm::vec3 position);
	LineRenderer(std::vector<LineVertex> VertexList, glm::vec3 position, glm::vec3 rotation);
	LineRenderer(std::vector<LineVertex> VertexList, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	LineRenderer(glm::vec3 StartPoint, glm::vec3 EndPoint);
	LineRenderer(glm::vec3 StartPoint, glm::vec3 EndPoint, glm::vec3 position);
	LineRenderer(glm::vec3 StartPoint, glm::vec3 EndPoint, glm::vec3 position, glm::vec3 rotation);
	LineRenderer(glm::vec3 StartPoint, glm::vec3 EndPoint, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	LineRenderer(nlohmann::json& json);
	virtual ~LineRenderer();

	void Update(float DeltaTime) override;
	void Destroy() override;

	std::shared_ptr<Model> GetModel() { return model; }
};
