#pragma once
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "Converters.h"
#include "Mesh.h"
#include "Bone.h"

const unsigned int MAX_BONE_VERTEX_COUNT = 4;

struct NodeMap
{
	std::string NodeString;
	aiMatrix4x4 NodeTransform;
	int ParentNodeID;
	int NodeID;
	std::vector<int> ChildNodeList;
	int MeshID;
};

class Model
{
private:
	static uint64_t ModelIDCounter;

	uint64_t ModelID;

	glm::vec3 ModelPosition = glm::vec3(0.0f);
	glm::vec3 ModelRotation = glm::vec3(0.0f);
	glm::vec3 ModelScale = glm::vec3(1.0f);
	glm::mat4 ModelTransform;

	std::vector<std::shared_ptr<Mesh>> MeshList;
	std::vector<std::shared_ptr<Bone>> BoneList;
	std::vector<NodeMap> NodeMapList;

	void GenerateID();
	void LoadMesh(const std::string& FilePath, aiNode* node, const aiScene* scene);
	std::vector<MeshVertex> LoadVertices(aiMesh* mesh);
	std::vector<uint32_t> LoadIndices(aiMesh* mesh);
	void LoadBones(const aiNode* RootNode, const aiMesh* mesh, std::vector<MeshVertex>& VertexList);
	std::vector<MeshBoneWeights> LoadBoneWeights(aiMesh* mesh, std::vector<MeshVertex>& VertexList);
	std::shared_ptr<Material> LoadMaterial(const std::string& FilePath, aiMesh* mesh, const aiScene* scene);

public:
	Model();
	Model(const std::string& FilePath);
	Model(std::shared_ptr<Mesh> mesh);
	~Model();

	void AddMesh(std::shared_ptr<Mesh> mesh);
	void AddMesh(glm::vec3& StartPoint, glm::vec3& EndPoint);
	void AddMesh(std::vector<LineVertex>& vertices);
	void AddMesh(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices);
	void AddMesh(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices, std::shared_ptr<Material> materialPtr);
	void DeleteMesh(std::shared_ptr<Mesh> mesh);
	void RemoveMesh(std::shared_ptr<Mesh> mesh);

	void Draw(VkCommandBuffer& commandBuffer);
	void Update();
	void Destroy();

	std::vector<std::shared_ptr<Mesh>> GetMeshList() { return MeshList; }
};

