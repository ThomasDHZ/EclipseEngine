#include "LineRenderer2D.h"

LineRenderer2D::LineRenderer2D()
{
}

LineRenderer2D::LineRenderer2D(std::vector<LineVertex> VertexList, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kLineRenderer2D, ComponentSubType::kRenderedObject)
{
	Line = std::make_shared<LineMesh2D>(LineMesh2D(VertexList, GameObjectID));
	MeshRendererManager::AddMesh(Line);
}

LineRenderer2D::LineRenderer2D(glm::vec2 StartPoint, glm::vec2 EndPoint, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kLineRenderer2D, ComponentSubType::kRenderedObject)
{
	Line = std::make_shared<LineMesh2D>(LineMesh2D(StartPoint, EndPoint, GameObjectID));
	MeshRendererManager::AddMesh(Line);
}

LineRenderer2D::~LineRenderer2D()
{
}

void LineRenderer2D::Update(const glm::mat4& GameObjectMatrix, float DeltaTime)
{
	Line->Update(GameObjectMatrix, glm::mat4(1.0f));
}

void LineRenderer2D::Destroy()
{
	Line->Destroy();
}