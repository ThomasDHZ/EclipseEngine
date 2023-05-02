#pragma once
#include "GameObject.h"
#include "LineMesh3D.h"

class LineGameObject : public GameObject
{
public:
	LineGameObject();
	LineGameObject(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint);
	LineGameObject(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec3& position);
	LineGameObject(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec3& position, const glm::vec3& rotation);
	LineGameObject(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
	LineGameObject(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& Color);
	LineGameObject(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& Color, const glm::vec3& position);
	LineGameObject(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& Color, const glm::vec3& position, const glm::vec3& rotation);
	LineGameObject(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& Color, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
	LineGameObject(const std::string Name, std::vector<LineVertex3D> VertexList);
	LineGameObject(const std::string Name, std::vector<LineVertex3D> VertexList, glm::vec3 position);
	LineGameObject(const std::string Name, std::vector<LineVertex3D> VertexList, glm::vec3 position, glm::vec3 rotation);
	LineGameObject(const std::string Name, std::vector<LineVertex3D> VertexList, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	~LineGameObject();

	void DrawLine(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperties);
};

