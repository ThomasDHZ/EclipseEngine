#pragma once
#include "Vertex.h"
#include "Mesh.h"

class GridMesh2D : public Mesh
{
private:
	std::vector<LineVertex2D> LineVertexList;
	std::vector<uint32_t> LineIndexList;
public:
	GridMesh2D(const std::string& GridName, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID);
	GridMesh2D(const std::string& GridName, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, const glm::vec3& LineColor, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID);
	GridMesh2D(const std::string& GridName, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, const glm::vec4& LineColor, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID);
	~GridMesh2D();

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