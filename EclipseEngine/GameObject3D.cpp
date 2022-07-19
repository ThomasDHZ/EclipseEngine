#include "GameObject3D.h"
#include "MeshRenderer.h"
#include "LineRenderer3D.h"
#include "GridRenderer3DComponent.h"

GameObject3D::GameObject3D() 
{
}

GameObject3D::GameObject3D(const std::string Name, const std::string filePath) : GameObject(Name)
{
	AddComponent(std::make_shared<MeshRenderer>(MeshRenderer(filePath, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, const std::string filePath, glm::vec3 position) : GameObject(Name, position)
{
	AddComponent(std::make_shared<MeshRenderer>(MeshRenderer(filePath, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, const std::string filePath, glm::vec3 position, glm::vec3 rotation) : GameObject(Name, position, rotation)
{
	AddComponent(std::make_shared<MeshRenderer>(MeshRenderer(filePath, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, const std::string filePath, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : GameObject(Name, position, rotation, scale)
{
	AddComponent(std::make_shared<MeshRenderer>(MeshRenderer(filePath, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList) : GameObject(Name)
{
	AddComponent(std::make_shared<MeshRenderer>(MeshRenderer(VertexList, IndexList, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, glm::vec3 position) : GameObject(Name, position)
{
	AddComponent(std::make_shared<MeshRenderer>(MeshRenderer(VertexList, IndexList, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, glm::vec3 position, glm::vec3 rotation) : GameObject(Name, position, rotation)
{
	AddComponent(std::make_shared<MeshRenderer>(MeshRenderer(VertexList, IndexList, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, std::vector<Vertex3D> VertexList, std::vector<uint32_t> IndexList, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : GameObject(Name, position, rotation, scale)
{
	AddComponent(std::make_shared<MeshRenderer>(MeshRenderer(VertexList, IndexList, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint) : GameObject(Name)
{
	AddComponent(std::make_shared<LineRenderer3D>(LineRenderer3D(StartPoint, EndPoint, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec3& position) : GameObject(Name, position)
{
	AddComponent(std::make_shared<LineRenderer3D>(LineRenderer3D(StartPoint, EndPoint, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec3& position, glm::vec3& rotation) : GameObject(Name, position, rotation)
{
	AddComponent(std::make_shared<LineRenderer3D>(LineRenderer3D(StartPoint, EndPoint, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale) : GameObject(Name, position, rotation, scale)
{
	AddComponent(std::make_shared<LineRenderer3D>(LineRenderer3D(StartPoint, EndPoint, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec4& Color) : GameObject(Name)
{
	AddComponent(std::make_shared<LineRenderer3D>(LineRenderer3D(StartPoint, EndPoint, Color, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec4& Color, glm::vec3& position) : GameObject(Name, position)
{
	AddComponent(std::make_shared<LineRenderer3D>(LineRenderer3D(StartPoint, EndPoint, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec4& Color, glm::vec3& position, glm::vec3& rotation) : GameObject(Name, position, rotation)
{
	AddComponent(std::make_shared<LineRenderer3D>(LineRenderer3D(StartPoint, EndPoint, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec4& Color, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale) : GameObject(Name, position, rotation, scale)
{
	AddComponent(std::make_shared<LineRenderer3D>(LineRenderer3D(StartPoint, EndPoint, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, std::vector<LineVertex3D> VertexList)
{
	AddComponent(std::make_shared<LineRenderer3D>(LineRenderer3D(VertexList, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, std::vector<LineVertex3D> VertexList, glm::vec3 position) : GameObject(Name, position)
{
	AddComponent(std::make_shared<LineRenderer3D>(LineRenderer3D(VertexList, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, std::vector<LineVertex3D> VertexList, glm::vec3 position, glm::vec3 rotation) : GameObject(Name, position, rotation)
{
	AddComponent(std::make_shared<LineRenderer3D>(LineRenderer3D(VertexList, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, std::vector<LineVertex3D> VertexList, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : GameObject(Name, position, rotation, scale)
{
	AddComponent(std::make_shared<LineRenderer3D>(LineRenderer3D(VertexList, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, int GridSize, float GridSpacing)
{
	AddComponent(std::make_shared<GridRenderer3DComponent>(GridRenderer3DComponent(GridSize, GridSpacing, GameObjectID)));
}

GameObject3D::GameObject3D(const std::string Name, int GridSizeX, int GridSizeY, int GridSizeZ, float GridSpacingX, float GridSpacingY, float GridSpacingZ) : GameObject(Name)
{
	AddComponent(std::make_shared<GridRenderer3DComponent>(GridRenderer3DComponent(GridSizeX, GridSizeY, GridSizeZ, GridSpacingX, GridSpacingY, GridSpacingZ, GameObjectID)));
}

GameObject3D::~GameObject3D()
{
}

void GameObject3D::Update(float DeltaTime)
{
	GameObject::Update(DeltaTime);

	if (auto meshRendererComponent = GetComponentByType(ComponentType::kMeshRenderer))
	{
		auto meshRenderer = static_cast<MeshRenderer*>(meshRendererComponent.get());
		meshRenderer->Update(GameObjectTransform, DeltaTime);
	}

	if (auto lineRenderer3DComponent = GetComponentByType(ComponentType::kLineRenderer3D))
	{
		auto lineRenderer3D = static_cast<LineRenderer3D*>(lineRenderer3DComponent.get());
		lineRenderer3D->Update(GameObjectTransform, DeltaTime);
	}
}

void GameObject3D::SetGameObjectPosition(float x, float y, float z)
{
	GameObject::SetGameObjectPosition(x, y, z);
}

void GameObject3D::SetGameObjectRotation(float x, float y, float z)
{
	GameObject::SetGameObjectRotation(x, y, z);
}

void GameObject3D::SetGameObjectScale(float x, float y, float z)
{
	GameObject::SetGameObjectScale(x, y, z);
}

void GameObject3D::SetGameObjectPosition(glm::vec3 gameObjectPosition)
{
	GameObject::SetGameObjectPosition(gameObjectPosition);
}

void GameObject3D::SetGameObjectRotation(glm::vec3 gameObjectRotation)
{
	GameObject::SetGameObjectRotation(gameObjectRotation);
}

void GameObject3D::SetGameObjectScale(glm::vec3 gameObjectScale)
{
	GameObject::SetGameObjectScale(gameObjectScale);
}