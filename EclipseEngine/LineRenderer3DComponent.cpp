#include "LineRenderer3DComponent.h"

LineRenderer3DComponent::LineRenderer3DComponent() : Component()
{
}

LineRenderer3DComponent::LineRenderer3DComponent(std::vector<LineVertex3D> VertexList, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kLineRenderer3D, ComponentSubType::kRenderedObject)
{
	model = std::make_shared<Model>(Model(VertexList, ParentGameObjectID));

}

LineRenderer3DComponent::LineRenderer3DComponent(glm::vec3& StartPoint, glm::vec3& EndPoint, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kLineRenderer3D, ComponentSubType::kRenderedObject)
{
	model = std::make_shared<Model>(Model(StartPoint, EndPoint, ParentGameObjectID));
}

LineRenderer3DComponent::LineRenderer3DComponent(glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec3& LineColor, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kLineRenderer3D, ComponentSubType::kRenderedObject)
{
	model = std::make_shared<Model>(Model(StartPoint, EndPoint, LineColor, ParentGameObjectID));
}

LineRenderer3DComponent::LineRenderer3DComponent(glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec4& LineColor, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kLineRenderer3D, ComponentSubType::kRenderedObject)
{
	model = std::make_shared<Model>(Model(StartPoint, EndPoint, LineColor, ParentGameObjectID));
}

LineRenderer3DComponent::LineRenderer3DComponent(nlohmann::json& json, uint64_t GameObjectID) : Component(GameObjectID, json, ComponentSubType::kRenderedObject)
{
	model = std::make_shared<Model>(Model(json, GameObjectID));
}

LineRenderer3DComponent::~LineRenderer3DComponent()
{
}

void LineRenderer3DComponent::Update(const glm::mat4& GameObjectMatrix, float DeltaTime)
{
	model->Update(GameObjectMatrix);
}

void LineRenderer3DComponent::Destroy()
{
	model->Destroy();
}