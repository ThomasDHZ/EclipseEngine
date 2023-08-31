#pragma once
#include "Mesh.h"

class SquareMesh2D : public Mesh
{
private:
	std::vector<LineVertex2D> LineVertexList;
	std::vector<uint32_t> LineIndexList;
public:

	SquareMesh2D(const std::string& LineName, float size, const glm::vec3& Color, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID);
	SquareMesh2D(const std::string& LineName, float size, const glm::vec4& Color, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID);
	~SquareMesh2D();

	virtual void DrawLine(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout) override;
	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;
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

