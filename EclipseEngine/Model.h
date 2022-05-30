#pragma once
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "MeshRendererManager.h"
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

	void from_json(nlohmann::json& json)
	{
	}

	void to_json(nlohmann::json& json)
	{
		//JsonConverter::to_json(json["NodeString"], NodeString);
		//JsonConverter::to_json(json["NodeTransform"], NodeTransform);
		//JsonConverter::to_json(json["ModelScale"], ModelScale);
	}
};

class Model
{
private:
	static uint64_t ModelIDCounter;

	std::string ModelFilePath;

	uint64_t ModelID = 0;
	uint64_t ParentGameObjectID = 0;

	glm::mat4 ModelTransform = glm::mat4(1.0f);

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

	void from_json(nlohmann::json& json)
	{
		JsonConverter::from_json(json["ModelFilePath"], ModelFilePath);
		for (int x = 0; x < json["MeshList"].size(); x++)
		{
			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(json["MeshList"][x], ModelID);
			MeshList.emplace_back(mesh);
			MeshRendererManager::AddMesh(mesh);
		}
	}

public:


	glm::vec3 ModelPosition = glm::vec3(0.0f);
	glm::vec3 ModelRotation = glm::vec3(0.0f);
	glm::vec3 ModelScale = glm::vec3(1.0f);

	Model();
	Model(const std::string& FilePath, uint64_t GameObjectID);
	Model(std::shared_ptr<Mesh> mesh, uint64_t GameObjectID);
	Model(nlohmann::json& json);
	~Model();

	void AddMesh(std::shared_ptr<Mesh> mesh);
	void AddMesh(glm::vec3& StartPoint, glm::vec3& EndPoint);
	void AddMesh(std::vector<LineVertex>& vertices);
	void AddMesh(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices);
	void AddMesh(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices, std::shared_ptr<Material> materialPtr);
	void AddMesh(MeshLoadingInfo& meshLoader);

	void DeleteMesh(std::shared_ptr<Mesh> mesh);
	void RemoveMesh(std::shared_ptr<Mesh> mesh);

	void Update();
	void UpdateMeshTopLevelAccelerationStructure(std::vector<VkAccelerationStructureInstanceKHR>& AccelerationStructureInstanceList);
	void Destroy();

	bool DoesMeshExistInModel(std::shared_ptr<Mesh> mesh);

	glm::mat4 TransposeModelMatrix();
	uint64_t GetModelID() { return ModelID; }
	uint64_t GetParentGameObjectID() { return ParentGameObjectID; }
	std::vector<std::shared_ptr<Mesh>> GetMeshList() { return MeshList; }

	glm::mat4 GetModelMatrix() { return ModelTransform; }
	glm::vec3* GetModelPositionPtr() { return &ModelPosition; }
	glm::vec3* GetModelRotationPtr() { return &ModelRotation; }
	glm::vec3* GetModelScalePtr() { return &ModelScale; }

	void to_json(nlohmann::json& json)
	{
		JsonConverter::to_json(json["ModelFilePath"], ModelFilePath);

		glm::mat4 ModelTransform = glm::mat4(1.0f);

		for (int x = 0; x < MeshList.size(); x++)
		{
			MeshList[x]->to_json(json["MeshList"][x]);
		}
		for (int x = 0; x < MeshList.size(); x++)
		{
			MeshList[x]->to_json(json["MeshList"][x]);
		}
		for (int x = 0; x < NodeMapList.size(); x++)
		{
			NodeMapList[x].to_json(json["NodeMapList"][x]);
		}

		JsonConverter::to_json(json["ModelPosition"], ModelPosition);
		JsonConverter::to_json(json["ModelRotation"], ModelRotation);
		JsonConverter::to_json(json["ModelScale"], ModelScale);
	}
};

