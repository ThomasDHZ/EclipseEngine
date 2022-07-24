#pragma once
#include "Mesh.h"
#include "MeshRendererManager.h"

class Mesh3D : public Mesh
{
private:
	std::vector<Vertex3D> VertexList;
	std::vector<uint32_t> IndexList;

	VulkanBuffer TransformBuffer;
	VulkanBuffer TransformInverseBuffer;
	VulkanBuffer BoneWeightBuffer;
	VulkanBuffer BoneTransformBuffer;

	glm::vec3 GetPosition() { return MeshPosition; }
	glm::vec3 GetRotation() { return MeshRotation; }
	glm::vec3 GetScale() { return MeshScale; }

protected:
	std::vector<MeshBoneWeights> BoneWeightList;
	std::vector<glm::mat4> BoneTransform;

	VkAccelerationStructureGeometryKHR AccelerationStructureGeometry{};
	VkAccelerationStructureBuildRangeInfoKHR AccelerationStructureBuildRangeInfo{};

	void MeshStartUp(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, uint64_t parentGameObjectID);
	void MeshStartUp(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, std::shared_ptr<Material> materialPtr, uint64_t parentGameObjectID);
	void MeshStartUp(MeshLoadingInfo& meshLoader);
	void RTXMeshStartUp();
	void UpdateMeshBottomLevelAccelerationStructure();

public:
	Mesh3D();
	Mesh3D(std::vector<Vertex3D>& vertices, uint64_t parentGameObjectID);
	Mesh3D(std::vector<Vertex3D>& vertices, std::shared_ptr<Material> materialPtr, uint64_t parentGameObjectID);

	Mesh3D(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, uint64_t parentGameObjectID);
	Mesh3D(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, std::shared_ptr<Material> materialPtr, uint64_t parentGameObjectID);
	Mesh3D(MeshLoadingInfo& meshLoader);
	~Mesh3D();

	glm::vec3 MeshPosition = glm::vec3(0.0f);
	glm::vec3 MeshRotation = glm::vec3(0.0f);
	glm::vec3 MeshScale = glm::vec3(1.0f);

	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;
	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix, const std::vector<std::shared_ptr<Bone>>& BoneList) override;
	virtual void Destroy() override;

	void SetMeshPosition(float x, float y, float z);
	void SetMeshRotation(float x, float y, float z);
	void SetMeshScale(float x, float y, float z);
	void SetMeshPosition(glm::vec3 position);
	void SetMeshRotation(glm::vec3 rotation);
	void SetMeshScale(glm::vec3 scale);
};

