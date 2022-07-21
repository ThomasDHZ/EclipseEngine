#include "LineRenderer3D.h"

LineRenderer3D::LineRenderer3D()
{
}

LineRenderer3D::LineRenderer3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint) : GameObject(Name)
{
	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(StartPoint, EndPoint));
	GameObjectManager::AddGameObject(std::make_shared<LineRenderer3D>(*this));
}

LineRenderer3D::LineRenderer3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec3& position) : GameObject(Name, position)
{
	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(StartPoint, EndPoint));
	GameObjectManager::AddGameObject(std::make_shared<LineRenderer3D>(*this));
}

LineRenderer3D::LineRenderer3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec3& position, glm::vec3& rotation) : GameObject(Name, position, rotation)
{
	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(StartPoint, EndPoint));
	GameObjectManager::AddGameObject(std::make_shared<LineRenderer3D>(*this));
}

LineRenderer3D::LineRenderer3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale) : GameObject(Name, position, rotation, scale)
{
	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(StartPoint, EndPoint));
	GameObjectManager::AddGameObject(std::make_shared<LineRenderer3D>(*this));
}


LineRenderer3D::LineRenderer3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec4& Color) : GameObject(Name)
{
	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(StartPoint, EndPoint, Color));
	GameObjectManager::AddGameObject(std::make_shared<LineRenderer3D>(*this));
}

LineRenderer3D::LineRenderer3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec4& Color, glm::vec3& position) : GameObject(Name, position)
{
	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(StartPoint, EndPoint, Color));
	GameObjectManager::AddGameObject(std::make_shared<LineRenderer3D>(*this));
}

LineRenderer3D::LineRenderer3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec4& Color, glm::vec3& position, glm::vec3& rotation) : GameObject(Name, position, rotation)
{
	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(StartPoint, EndPoint, Color));
	GameObjectManager::AddGameObject(std::make_shared<LineRenderer3D>(*this));
}

LineRenderer3D::LineRenderer3D(const std::string Name, glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec4& Color, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale) : GameObject(Name, position, rotation, scale)
{
	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(StartPoint, EndPoint, Color));
	GameObjectManager::AddGameObject(std::make_shared<LineRenderer3D>(*this));
}

LineRenderer3D::LineRenderer3D(const std::string Name, std::vector<LineVertex3D> VertexList)
{
	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(VertexList));
	GameObjectManager::AddGameObject(std::make_shared<LineRenderer3D>(*this));
}

LineRenderer3D::LineRenderer3D(const std::string Name, std::vector<LineVertex3D> VertexList, glm::vec3 position) : GameObject(Name, position)
{
	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(VertexList));
	GameObjectManager::AddGameObject(std::make_shared<LineRenderer3D>(*this));
}

LineRenderer3D::LineRenderer3D(const std::string Name, std::vector<LineVertex3D> VertexList, glm::vec3 position, glm::vec3 rotation) : GameObject(Name, position, rotation)
{
	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(VertexList));
	GameObjectManager::AddGameObject(std::make_shared<LineRenderer3D>(*this));
}

LineRenderer3D::LineRenderer3D(const std::string Name, std::vector<LineVertex3D> VertexList, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : GameObject(Name, position, rotation, scale)
{
	LineObject = std::make_shared<LineMesh3D>(LineMesh3D(VertexList));
	GameObjectManager::AddGameObject(std::make_shared<LineRenderer3D>(*this));
}

LineRenderer3D::~LineRenderer3D()
{
}

void LineRenderer3D::Update(float DeltaTime)
{
	GameObject::Update(DeltaTime);
	LineObject->Update(GameObjectTransform, glm::mat4(1.0f));
}