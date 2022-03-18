#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "Bone.h"

const unsigned int MAX_BONE_VERTEX_COUNT = 4;

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

	void GenerateID();
	void LoadMesh(const std::string& FilePath, aiNode* node, const aiScene* scene);
	std::vector<MeshVertex> LoadVertices(aiMesh* mesh);
	std::vector<uint32_t> LoadIndices(aiMesh* mesh);
	void LoadBones(const aiNode* RootNode, const aiMesh* mesh, std::vector<MeshVertex>& VertexList);
	std::vector<MeshBoneWeights> LoadBoneWeights(aiMesh* mesh, std::vector<MeshVertex>& VertexList);
	uint64_t LoadMaterial(const std::string& FilePath, aiMesh* mesh, const aiScene* scene);

public:
	Model();
	~Model();

	void AddMesh(std::shared_ptr<Mesh> mesh);
	void DeleteMesh(std::shared_ptr<Mesh> mesh);
	void RemoveMesh(std::shared_ptr<Mesh> mesh);

	void Update();
	void Destroy();
};

