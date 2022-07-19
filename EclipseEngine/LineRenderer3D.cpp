#include "LineRenderer3D.h"

LineRenderer3D::LineRenderer3D() : Component()
{
}

LineRenderer3D::LineRenderer3D(std::vector<LineVertex3D> VertexList, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kLineRenderer3D, ComponentSubType::kRenderedObject)
{
	model = std::make_shared<Model>(Model(VertexList, ParentGameObjectID));

}

LineRenderer3D::LineRenderer3D(glm::vec3& StartPoint, glm::vec3& EndPoint, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kLineRenderer3D, ComponentSubType::kRenderedObject)
{
	model = std::make_shared<Model>(Model(StartPoint, EndPoint, ParentGameObjectID));
}

LineRenderer3D::LineRenderer3D(glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec3& LineColor, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kLineRenderer3D, ComponentSubType::kRenderedObject)
{
	model = std::make_shared<Model>(Model(StartPoint, EndPoint, LineColor, ParentGameObjectID));
}

LineRenderer3D::LineRenderer3D(glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec4& LineColor, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kLineRenderer3D, ComponentSubType::kRenderedObject)
{
	model = std::make_shared<Model>(Model(StartPoint, EndPoint, LineColor, ParentGameObjectID));
}

LineRenderer3D::LineRenderer3D(nlohmann::json& json, uint64_t GameObjectID) : Component(GameObjectID, json, ComponentSubType::kRenderedObject)
{
	model = std::make_shared<Model>(Model(json, GameObjectID));
}

LineRenderer3D::~LineRenderer3D()
{
}

void LineRenderer3D::Update(const glm::mat4& GameObjectMatrix, float DeltaTime)
{
	model->Update(GameObjectMatrix);
}

void LineRenderer3D::Destroy()
{
	model->Destroy();
}