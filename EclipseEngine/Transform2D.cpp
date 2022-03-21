#include "Transform2D.h"

Transform2D::Transform2D() : Transform(ComponentType::kTransform2D)
{
	Position = glm::vec3(0.0f);
	Rotation = glm::vec3(0.0f);
	Scale = glm::vec3(1.0f);
	ZIndex = 0;
}

Transform2D::Transform2D(glm::vec2 position, float zIndex) : Transform(ComponentType::kTransform2D)
{
	Position = glm::vec3(position.x, position.y, zIndex);
	Rotation = glm::vec3(0.0f);
	Scale = glm::vec3(1.0f);
	ZIndex = zIndex;
}

Transform2D::Transform2D(glm::vec2 position, glm::vec2 rotation, float zIndex) : Transform(ComponentType::kTransform2D)
{
	Position = glm::vec3(position.x, position.y, zIndex);
	Rotation = glm::vec3(rotation.x, rotation.y, 0.0f);
	Scale = glm::vec3(1.0f);
	ZIndex = zIndex;
}

Transform2D::Transform2D(glm::vec2 position, glm::vec2 rotation, glm::vec2 scale, float zIndex) : Transform(ComponentType::kTransform2D)
{
	Position = glm::vec3(position.x, position.y, zIndex);
	Rotation = glm::vec3(rotation.x, rotation.y, 0.0f);
	Scale = glm::vec3(scale.x, scale.y, 0.0f);
	ZIndex = zIndex;
}

Transform2D::Transform2D(const nlohmann::json& json) : Transform(json)
{
	json.at("componentType").get_to(componentType);
	json.at("Position")[0].get_to(Position.x);
	json.at("Position")[1].get_to(Position.y);
	json.at("Rotation")[0].get_to(Rotation.x);
	json.at("Rotation")[1].get_to(Rotation.y);
	json.at("Scale")[0].get_to(Scale.x);
	json.at("Scale")[1].get_to(Scale.y);
	json.at("ZIndex").get_to(ZIndex);
}

Transform2D::~Transform2D()
{
}

void Transform2D::Update(float DeltaTime)
{
	TransformMatrix = glm::mat4(1.0f);
	TransformMatrix = glm::translate(TransformMatrix, glm::vec3(Position.x, Position.y, ZIndex));
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	TransformMatrix = glm::scale(TransformMatrix, Scale);
}

void Transform2D::Destroy()
{
}
