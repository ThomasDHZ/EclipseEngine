#include "LineRenderer3D.h"

LineRenderer3D::LineRenderer3D() : Component()
{
}

LineRenderer3D::LineRenderer3D(std::vector<LineVertex3D> VertexList, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kLineRenderer3D, ComponentSubType::kRenderedObject)
{
	//std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(VertexList));
	//model = std::make_shared<Model>(Model(mesh, ParentGameObjectID));

	//ModelManager::AddModel(model);
}

LineRenderer3D::LineRenderer3D(glm::vec3 StartPoint, glm::vec3 EndPoint, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kLineRenderer3D, ComponentSubType::kRenderedObject)
{
	//std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(StartPoint, EndPoint));
	//model = std::make_shared<Model>(Model(mesh, ParentGameObjectID));
	//ModelManager::AddModel(model);
}


LineRenderer3D::LineRenderer3D(nlohmann::json& json, uint64_t GameObjectID) : Component(GameObjectID, json, ComponentSubType::kRenderedObject)
{
	//mesh = Mesh(vertices, indices);
}

LineRenderer3D::~LineRenderer3D()
{
}

void LineRenderer3D::Update(const glm::mat4& GameObjectMatrix, float DeltaTime)
{
//	model->Update();
}

void LineRenderer3D::Destroy()
{
	//model->Destroy();
}