#include "Transform3D.h"

Transform3D::Transform3D() : Component(ComponentType::kTransform3D)
{
	Position = glm::vec3(0.0f);
	Rotation = glm::vec3(0.0f);
	Scale = glm::vec3(1.0f);
}

Transform3D::Transform3D(glm::vec3 position) : Component(ComponentType::kTransform3D)
{
	Position = position;
	Rotation = glm::vec3(0.0f);
	Scale = glm::vec3(1.0f);
}

Transform3D::Transform3D(glm::vec3 position, glm::vec3 rotation) : Component(ComponentType::kTransform3D)
{
	Position = position;
	Rotation = rotation;
	Scale = glm::vec3(1.0f);
}

Transform3D::Transform3D(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : Component(ComponentType::kTransform3D)
{
	Position = position;
	Rotation = rotation;
	Scale = scale;
}

Transform3D::~Transform3D()
{
}

void Transform3D::Update(float DeltaTime)
{
	Transform = glm::mat4(1.0f);
	Transform = glm::translate(Transform, Position);
	Transform = glm::rotate(Transform, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	Transform = glm::rotate(Transform, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	Transform = glm::rotate(Transform, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	Transform = glm::scale(Transform, Scale);
}

void Transform3D::Destroy()
{
}
