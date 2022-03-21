#include "Transform3D.h"

Transform3D::Transform3D() : Transform(ComponentType::kTransform3D)
{
	Position = glm::vec3(0.0f);
	Rotation = glm::vec3(0.0f);
	Scale = glm::vec3(1.0f);
}

Transform3D::Transform3D(glm::vec3 position) : Transform(ComponentType::kTransform3D)
{
	Position = position;
	Rotation = glm::vec3(0.0f);
	Scale = glm::vec3(1.0f);
}

Transform3D::Transform3D(glm::vec3 position, glm::vec3 rotation) : Transform(ComponentType::kTransform3D)
{
	Position = position;
	Rotation = rotation;
	Scale = glm::vec3(1.0f);
}

Transform3D::Transform3D(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : Transform(ComponentType::kTransform3D)
{
	Position = position;
	Rotation = rotation;
	Scale = scale;
}

Transform3D::Transform3D(const nlohmann::json& json) : Transform(json)
{
	json.at("Position")[0].get_to(Position.x);
	json.at("Position")[1].get_to(Position.y);
	json.at("Position")[2].get_to(Position.z);
	json.at("Rotation")[0].get_to(Rotation.x);
	json.at("Rotation")[1].get_to(Rotation.y);
	json.at("Rotation")[2].get_to(Rotation.z);
	json.at("Scale")[0].get_to(Scale.x);
	json.at("Scale")[1].get_to(Scale.y);
	json.at("Scale")[2].get_to(Scale.z);
}

Transform3D::~Transform3D()
{
}

void Transform3D::Update(float DeltaTime)
{
	TransformMatrix = glm::mat4(1.0f);
	TransformMatrix = glm::translate(TransformMatrix, Position);
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	TransformMatrix = glm::scale(TransformMatrix, Scale);
}

void Transform3D::Destroy()
{
}
