#pragma once
#include "Vertex.h"
#include "Mesh.h"

class LineMesh2D : public Mesh
{
private:
	std::vector<LineVertex2D> LineVertexList;
	std::vector<uint32_t> LineIndexList;
public:
	glm::vec2 StartPoint;
	glm::vec2 EndPoint;

	LineMesh2D(const std::string& LineName, std::vector<LineVertex2D>& vertices, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID);
	LineMesh2D(const std::string& LineName, const glm::vec2& startPoint, const glm::vec2& endPoint, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID);
	LineMesh2D(const std::string& LineName, const glm::vec2& startPoint, const glm::vec2& endPoint, const glm::vec3& Color, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID);
	LineMesh2D(const std::string& LineName, const glm::vec2& startPoint, const glm::vec2& endPoint, const glm::vec4& Color, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID);
	LineMesh2D(const std::string& LineName, const glm::vec2& startPoint, const glm::vec2& endPoint, const glm::vec3& StartColor, const glm::vec3& EndColor, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID);
	LineMesh2D(const std::string& LineName, const glm::vec2& startPoint, const glm::vec2& endPoint, const glm::vec4& StartColor, const glm::vec4& EndColor, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID);
	~LineMesh2D();

	virtual void DrawLine(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout) override;
	virtual void Update(float DeltaTime, const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;
	virtual void Destroy() override;

	void SetMeshPosition(float x, float y);
	void SetMeshRotation(float x, float y);
	void SetMeshScale(float x, float y);
	void SetMeshPosition(const glm::vec2& position);
	void SetMeshRotation(const glm::vec2& rotation);
	void SetMeshScale(const glm::vec2& scale);

	glm::vec3 GetPosition() { return MeshPosition; }
	glm::vec3 GetRotation() { return MeshRotation; }
	glm::vec3 GetScale() { return MeshScale; }

};

