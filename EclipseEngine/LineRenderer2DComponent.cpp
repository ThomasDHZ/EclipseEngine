#include "LineRenderer2DComponent.h"

LineRenderer2DComponent::LineRenderer2DComponent()
{
}

LineRenderer2DComponent::LineRenderer2DComponent(std::vector<LineVertex2D>& VertexList, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kLineRenderer2D, ComponentSubType::kRenderedObject)
{
	Line = std::make_shared<LineMesh2D>(LineMesh2D(VertexList, GameObjectID));
	MeshRendererManager::AddMesh(Line);
}

LineRenderer2DComponent::LineRenderer2DComponent(glm::vec2& StartPoint, glm::vec2& EndPoint, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kLineRenderer2D, ComponentSubType::kRenderedObject)
{
	Line = std::make_shared<LineMesh2D>(LineMesh2D(StartPoint, EndPoint, GameObjectID));
	MeshRendererManager::AddMesh(Line);
}

LineRenderer2DComponent::LineRenderer2DComponent(glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec3& LineColor, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kLineRenderer2D, ComponentSubType::kRenderedObject)
{
	Line = std::make_shared<LineMesh2D>(LineMesh2D(StartPoint, EndPoint, LineColor, GameObjectID));
	MeshRendererManager::AddMesh(Line);
}

LineRenderer2DComponent::LineRenderer2DComponent(glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec4& LineColor, uint64_t GameObjectID) : Component(GameObjectID, ComponentType::kLineRenderer2D, ComponentSubType::kRenderedObject)
{
	Line = std::make_shared<LineMesh2D>(LineMesh2D(StartPoint, EndPoint, LineColor, GameObjectID));
	MeshRendererManager::AddMesh(Line);
}

LineRenderer2DComponent::~LineRenderer2DComponent()
{
}

void LineRenderer2DComponent::Update(const glm::mat4& GameObjectMatrix, float DeltaTime)
{
	Line->Update(GameObjectMatrix, glm::mat4(1.0f));
}

void LineRenderer2DComponent::Destroy()
{
	Line->Destroy();
}