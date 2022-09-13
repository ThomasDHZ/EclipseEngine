#include "LineRenderer2D.h"

LineRenderer2D::LineRenderer2D()
{
}

LineRenderer2D::LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint) : GameObject(Name)
{
	LineObject = std::make_shared<LineMesh2D>(LineMesh2D(StartPoint, EndPoint, GameObjectID));
}

LineRenderer2D::LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec2& position) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f))
{
	LineObject = std::make_shared<LineMesh2D>(LineMesh2D(StartPoint, EndPoint, GameObjectID));
}

LineRenderer2D::LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec2& position, glm::vec2& rotation) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f), glm::vec3(rotation.x, rotation.y, 0.0f))
{
	LineObject = std::make_shared<LineMesh2D>(LineMesh2D(StartPoint, EndPoint, GameObjectID));
}

LineRenderer2D::LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec2& position, glm::vec2& rotation, glm::vec2& scale) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f), glm::vec3(rotation.x, rotation.y, 0.0f), glm::vec3(scale.x, scale.y, 1.0f))
{
	LineObject = std::make_shared<LineMesh2D>(LineMesh2D(StartPoint, EndPoint, GameObjectID));
}

LineRenderer2D::LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec3& Color) : GameObject(Name)
{
	LineObject = std::make_shared<LineMesh2D>(LineMesh2D(StartPoint, EndPoint, Color, GameObjectID));
}

LineRenderer2D::LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec3& Color, glm::vec2& position) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f))
{
	LineObject = std::make_shared<LineMesh2D>(LineMesh2D(StartPoint, EndPoint, Color, GameObjectID));
}

LineRenderer2D::LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec3& Color, glm::vec2& position, glm::vec2& rotation) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f), glm::vec3(rotation.x, rotation.y, 0.0f))
{
	LineObject = std::make_shared<LineMesh2D>(LineMesh2D(StartPoint, EndPoint, Color, GameObjectID));
}

LineRenderer2D::LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec3& Color, glm::vec2& position, glm::vec2& rotation, glm::vec2& scale) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f), glm::vec3(rotation.x, rotation.y, 0.0f), glm::vec3(scale.x, scale.y, 1.0f))
{
	LineObject = std::make_shared<LineMesh2D>(LineMesh2D(StartPoint, EndPoint, Color, GameObjectID));
}

LineRenderer2D::LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec4& Color) : GameObject(Name)
{
	LineObject = std::make_shared<LineMesh2D>(LineMesh2D(StartPoint, EndPoint, Color, GameObjectID));
}

LineRenderer2D::LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec4& Color, glm::vec2& position) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f))
{
	LineObject = std::make_shared<LineMesh2D>(LineMesh2D(StartPoint, EndPoint, Color, GameObjectID));
}

LineRenderer2D::LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec4& Color, glm::vec2& position, glm::vec2& rotation) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f), glm::vec3(rotation.x, rotation.y, 0.0f))
{
	LineObject = std::make_shared<LineMesh2D>(LineMesh2D(StartPoint, EndPoint, Color, GameObjectID));
}

LineRenderer2D::LineRenderer2D(const std::string Name, glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec4& Color, glm::vec2& position, glm::vec2& rotation, glm::vec2& scale) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f), glm::vec3(rotation.x, rotation.y, 0.0f), glm::vec3(scale.x, scale.y, 1.0f))
{
	LineObject = std::make_shared<LineMesh2D>(LineMesh2D(StartPoint, EndPoint, Color, GameObjectID));
}

LineRenderer2D::LineRenderer2D(const std::string Name, std::vector<LineVertex2D>& VertexList) : GameObject(Name)
{
	LineObject = std::make_shared<LineMesh2D>(LineMesh2D(VertexList, GameObjectID));
}

LineRenderer2D::LineRenderer2D(const std::string Name, std::vector<LineVertex2D>& VertexList, glm::vec2& position) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f))
{
	LineObject = std::make_shared<LineMesh2D>(LineMesh2D(VertexList, GameObjectID));
}

LineRenderer2D::LineRenderer2D(const std::string Name, std::vector<LineVertex2D>& VertexList, glm::vec2& position, glm::vec2& rotation) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f), glm::vec3(rotation.x, rotation.y, 0.0f))
{
	LineObject = std::make_shared<LineMesh2D>(LineMesh2D(VertexList, GameObjectID));
}

LineRenderer2D::LineRenderer2D(const std::string Name, std::vector<LineVertex2D>& VertexList, glm::vec2& position, glm::vec2& rotation, glm::vec2& scale) : GameObject(Name, glm::vec3(position.x, position.y, 0.0f), glm::vec3(rotation.x, rotation.y, 0.0f), glm::vec3(scale.x, scale.y, 1.0f))
{
	LineObject = std::make_shared<LineMesh2D>(LineMesh2D(VertexList, GameObjectID));
}

LineRenderer2D::~LineRenderer2D()
{
}

void LineRenderer2D::Update(float DeltaTime)
{
	GameObject::Update(DeltaTime);
	LineObject->Update(GameObjectTransform, glm::mat4(1.0f));
}