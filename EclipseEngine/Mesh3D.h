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
};

