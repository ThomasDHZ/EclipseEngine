#pragma once
#include "Component.h"
#include "Mesh.h"

class LineRenderer : public Component
{
private:
public:
	LineRenderer();
	LineRenderer(std::vector<LineVertex> VertexList, uint64_t GameObjectID);
	LineRenderer(std::vector<LineVertex> VertexList, glm::vec3 position, uint64_t GameObjectID);
	LineRenderer(std::vector<LineVertex> VertexList, glm::vec3 position, glm::vec3 rotation, uint64_t GameObjectID);
	LineRenderer(std::vector<LineVertex> VertexList, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, uint64_t GameObjectID);
	LineRenderer(glm::vec3 StartPoint, glm::vec3 EndPoint, uint64_t GameObjectID);
	LineRenderer(glm::vec3 StartPoint, glm::vec3 EndPoint, glm::vec3 position, uint64_t GameObjectID);
	LineRenderer(glm::vec3 StartPoint, glm::vec3 EndPoint, glm::vec3 position, glm::vec3 rotation, uint64_t GameObjectID);
	LineRenderer(glm::vec3 StartPoint, glm::vec3 EndPoint, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, uint64_t GameObjectID);
	LineRenderer(nlohmann::json& json, uint64_t GameObjectID);
	virtual ~LineRenderer();

	void Update(float DeltaTime) override;
	void Destroy() override;

	virtual void to_json(nlohmann::json& json) override
	{
		//ComponentRenderer::to_json(json);
	}

};
