#pragma once
#include "Mesh.h"

class Mesh3D : public Mesh
{
private:
	std::vector<MeshVertex> VertexList;
	std::vector<uint32_t> IndexList;
	std::vector<MeshBoneWeights> BoneWeightList;
	std::vector<glm::mat4> BoneTransform;

	VulkanBuffer BoneWeightBuffer;
	VulkanBuffer BoneTransformBuffer;
public:
	Mesh3D(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices);
	Mesh3D(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices, std::shared_ptr<Material> materialPtr);
	Mesh3D(MeshLoadingInfo& meshLoader);
	~Mesh3D();

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

