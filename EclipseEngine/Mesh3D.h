#pragma once
#include "Mesh.h"

class Mesh3D : public Mesh
{
private:
	VulkanBuffer BoneWeightBuffer;
	VulkanBuffer BoneTransformBuffer;

protected:
	std::vector<MeshVertex> VertexList;
	std::vector<uint32_t> IndexList;
	std::vector<MeshBoneWeights> BoneWeightList;
	std::vector<glm::mat4> BoneTransform;

	glm::vec3 MeshPosition = glm::vec3(0.0f);
	glm::vec3 MeshRotation = glm::vec3(0.0f);
	glm::vec3 MeshScale = glm::vec3(1.0f);

public:
	Mesh3D();
	Mesh3D(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices);
	Mesh3D(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices, std::shared_ptr<Material> materialPtr);
	Mesh3D(MeshLoadingInfo& meshLoader);
	~Mesh3D();

	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;
	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix, const std::vector<std::shared_ptr<Bone>>& BoneList) override;

	void SetMeshPosition(float x, float y, float z);
	void SetMeshRotation(float x, float y, float z);
	void SetMeshScale(float x, float y, float z);
	void SetMeshPosition(glm::vec3 position);
	void SetMeshRotation(glm::vec3 rotation);
	void SetMeshScale(glm::vec3 scale);

	glm::vec3 GetPosition() { return MeshPosition; }
	glm::vec3 GetRotation() { return MeshRotation; }
	glm::vec3 GetScale() { return MeshScale; }
};

