#pragma once
#include "ComponentRenderer.h"
#include "Model.h"

class LineRenderer3D : public Component
{
private:
	std::shared_ptr<Model> model;
public:
	LineRenderer3D();
	LineRenderer3D(std::vector<LineVertex3D> VertexList, uint64_t GameObjectID);
	LineRenderer3D(glm::vec3& StartPoint, glm::vec3& EndPoint, uint64_t GameObjectID);
	LineRenderer3D(glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec3& LineColor, uint64_t GameObjectID);
	LineRenderer3D(glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec4& LineColor, uint64_t GameObjectID);
	LineRenderer3D(nlohmann::json& json, uint64_t GameObjectID);
	virtual ~LineRenderer3D();

	void Update(const glm::mat4& GameObjectMatrix, float DeltaTime);
	void Destroy() override;
	//std::shared_ptr<Model> GetLine() { return Line; }
	virtual void to_json(nlohmann::json& json) override
	{
		//ComponentRenderer::to_json(json);
	}
	std::shared_ptr<Model> GetModel() { return model; }
};
