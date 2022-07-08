#pragma once
#include "ComponentRenderer.h"
#include "Mesh.h"

class LineRenderer3D : public Component
{
private:
public:
	LineRenderer3D();
	LineRenderer3D(std::vector<LineVertex> VertexList, uint64_t GameObjectID);
	LineRenderer3D(std::vector<LineVertex> VertexList, glm::vec3 position, uint64_t GameObjectID);
	LineRenderer3D(std::vector<LineVertex> VertexList, glm::vec3 position, glm::vec3 rotation, uint64_t GameObjectID);
	LineRenderer3D(std::vector<LineVertex> VertexList, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, uint64_t GameObjectID);
	LineRenderer3D(glm::vec3 StartPoint, glm::vec3 EndPoint, uint64_t GameObjectID);
	LineRenderer3D(glm::vec3 StartPoint, glm::vec3 EndPoint, glm::vec3 position, uint64_t GameObjectID);
	LineRenderer3D(glm::vec3 StartPoint, glm::vec3 EndPoint, glm::vec3 position, glm::vec3 rotation, uint64_t GameObjectID);
	LineRenderer3D(glm::vec3 StartPoint, glm::vec3 EndPoint, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, uint64_t GameObjectID);
	LineRenderer3D(nlohmann::json& json, uint64_t GameObjectID);
	virtual ~LineRenderer3D();

	void Update(const glm::mat4& GameObjectMatrix, float DeltaTime);
	void Destroy() override;

	virtual void to_json(nlohmann::json& json) override
	{
		//ComponentRenderer::to_json(json);
	}

};
