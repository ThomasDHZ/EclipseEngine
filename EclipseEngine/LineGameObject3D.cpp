#include "LineGameObject3D.h"

LineGameObject3D::LineGameObject3D()
{
}

LineGameObject3D::LineGameObject3D(const std::string Name) : GameObject3D(Name, kLineRenderer3D)
{
}

LineGameObject3D::LineGameObject3D(const std::string Name, const glm::vec3& position) : GameObject3D(Name, kLineRenderer3D, position)
{
}

LineGameObject3D::LineGameObject3D(const std::string Name, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, kLineRenderer3D, position, rotation)
{
}

LineGameObject3D::LineGameObject3D(const std::string Name, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject3D(Name, kLineRenderer3D, position, rotation, scale)
{
}

LineGameObject3D::~LineGameObject3D()
{
}

void LineGameObject3D::LoadLineGameObject3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint)
{
	GameObjectRenderer = std::make_shared<Model>(Model());
	GameObjectRenderer->LoadLineMesh3D(Name, StartPoint, EndPoint, GameObjectTransform, GameObjectID);
	VulkanRenderer::UpdateRendererFlag = true;
}

void LineGameObject3D::LoadLineGameObject3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& Color)
{
	GameObjectRenderer = std::make_shared<Model>(Model());
	GameObjectRenderer->LoadLineMesh3D(Name, StartPoint, EndPoint, Color, GameObjectTransform, GameObjectID);
	VulkanRenderer::UpdateRendererFlag = true;
}

void LineGameObject3D::LoadLineGameObject3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& StartColor, const glm::vec4& EndColor)
{
	GameObjectRenderer = std::make_shared<Model>(Model());
	GameObjectRenderer->LoadLineMesh3D(Name, StartPoint, EndPoint, StartColor, EndColor, GameObjectTransform, GameObjectID);
	VulkanRenderer::UpdateRendererFlag = true;
}

void LineGameObject3D::LoadLineGameObject3D(const std::string Name, std::vector<LineVertex3D> VertexList)
{
	GameObjectRenderer = std::make_shared<Model>(Model());
	GameObjectRenderer->LoadLineMesh3D(Name, VertexList, GameObjectTransform, GameObjectID);
	VulkanRenderer::UpdateRendererFlag = true;
}
