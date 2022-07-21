#pragma once
#include "ComponentRenderer.h"
#include "Model.h"

class LineRenderer3DComponent : public Component
{
private:
	std::shared_ptr<Model> model;
public:
	LineRenderer3DComponent();
	LineRenderer3DComponent(std::vector<LineVertex3D> VertexList, uint64_t GameObjectID);
	LineRenderer3DComponent(glm::vec3& StartPoint, glm::vec3& EndPoint, uint64_t GameObjectID);
	LineRenderer3DComponent(glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec3& LineColor, uint64_t GameObjectID);
	LineRenderer3DComponent(glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec4& LineColor, uint64_t GameObjectID);
	LineRenderer3DComponent(nlohmann::json& json, uint64_t GameObjectID);
	virtual ~LineRenderer3DComponent();

	void Update(const glm::mat4& GameObjectMatrix, float DeltaTime);
	void Destroy() override;
	//std::shared_ptr<Model> GetLine() { return Line; }
	virtual void to_json(nlohmann::json& json) override
	{
		//ComponentRenderer::to_json(json);
	}
	std::shared_ptr<Model> GetModel() { return model; }
};
