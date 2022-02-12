#include "Transform2D.h"

Transform2D::Transform2D() : Component(ComponentType::kTransform2D)
{
	Position = glm::vec3(0.0f);
	Rotation = glm::vec3(0.0f);
	Scale = glm::vec3(1.0f);
	ZIndex = 0;
}

Transform2D::Transform2D(glm::vec2 position, float zIndex) : Component(ComponentType::kTransform2D)
{
	Position = glm::vec3(position.x, position.y, zIndex);
	Rotation = glm::vec3(0.0f);
	Scale = glm::vec3(1.0f);
	ZIndex = zIndex;
}

Transform2D::Transform2D(glm::vec2 position, glm::vec2 rotation, float zIndex) : Component(ComponentType::kTransform2D)
{
	Position = glm::vec3(position.x, position.y, zIndex);
	Rotation = glm::vec3(rotation.x, rotation.y, 0.0f);
	Scale = glm::vec3(1.0f);
	ZIndex = zIndex;
}

Transform2D::Transform2D(glm::vec2 position, glm::vec2 rotation, glm::vec2 scale, float zIndex) : Component(ComponentType::kTransform2D)
{
	Position = glm::vec3(position.x, position.y, zIndex);
	Rotation = glm::vec3(rotation.x, rotation.y, 0.0f);
	Scale = glm::vec3(scale.x, scale.y, 0.0f);
	ZIndex = zIndex;
}

Transform2D::Transform2D(const nlohmann::json& json)
{
	FromJson(json);
}

Transform2D::~Transform2D()
{
}

void Transform2D::Update(float DeltaTime)
{
	Transform = glm::mat4(1.0f);
	Transform = glm::translate(Transform, glm::vec3(Position.x, Position.y, ZIndex));
	Transform = glm::rotate(Transform, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	Transform = glm::rotate(Transform, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	Transform = glm::rotate(Transform, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	Transform = glm::scale(Transform, Scale);
}

void Transform2D::Destroy()
{
}
