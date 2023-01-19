#pragma once
#include "Mesh.h"
#include <GLTFModel.h>
#include <basetsd.h>

struct GLTFMeshLoader3D
{
	std::string MeshName;
	MeshTypeEnum MeshType;
	MeshSubTypeEnum MeshSubType;

	uint64_t ParentGameObjectID = 0;
	uint64_t ParentModelID = 0;
	int NodeID = -1;

	int ParentMeshID;
	std::vector<int> ChildMeshIDList;

	GLTFPrimitive Primitive;

	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;
	uint32_t BoneCount = 0;
	VulkanBuffer TransformBuffer;

	glm::mat4 GameObjectTransform = glm::mat4(1.0f);
	glm::mat4 ModelTransform = glm::mat4(1.0f);
	glm::mat4 MeshTransform = glm::mat4(1.0f);

	glm::vec3 MeshPosition = glm::vec3(0.0f);
	glm::vec3 MeshRotation = glm::vec3(0.0f);
	glm::vec3 MeshScale = glm::vec3(1.0f);

	std::shared_ptr<Material> MaterialPtr = MaterialManager::GetDefaultMaterial();
	InstancingDataStruct instanceData;

	std::vector<MeshBoneWeights> BoneWeightList;
};

class Temp_GLTFMesh : public Mesh
{
private:
	uint64_t ParentGameObjectID = 0;
	uint64_t ParentModelID = 0;
	int ParentMeshID = -1;
	std::vector<int> ChildMeshIDList;


	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;
	uint32_t BoneCount = 0;

	glm::mat4 GameObjectTransform = glm::mat4(1.0f);
	glm::mat4 ModelTransform = glm::mat4(1.0f);
	glm::mat4 MeshTransform = glm::mat4(1.0f);

	glm::vec3 MeshPosition = glm::vec3(0.0f);
	glm::vec3 MeshRotation = glm::vec3(0.0f);
	glm::vec3 MeshScale = glm::vec3(1.0f);

	VulkanBuffer TransformBuffer;

public:
	Temp_GLTFMesh();
	Temp_GLTFMesh(GLTFMeshLoader3D& meshLoader);
	~Temp_GLTFMesh();

	GLTFPrimitive Primitive;

	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix) override;
	virtual void Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix, const std::vector<std::shared_ptr<Bone>>& BoneList) override;
	void Draw(VkCommandBuffer& commandBuffer);
	virtual void Destroy() override;

	void SetMeshPosition(float x, float y, float z);
	void SetMeshRotation(float x, float y, float z);
	void SetMeshScale(float x, float y, float z);
};