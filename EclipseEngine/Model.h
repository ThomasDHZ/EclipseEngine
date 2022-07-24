#pragma once
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "MeshRendererManager.h"
#include "Converters.h"
#include "Mesh.h"
#include "Mesh3D.h"
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
	void LoadModel(const std::string& FilePath, uint64_t GameObjectID);
	void LoadMesh(const std::string& FilePath, aiNode* node, const aiScene* scene);
	std::vector<Vertex3D> LoadVertices(aiMesh* mesh);
	std::vector<uint32_t> LoadIndices(aiMesh* mesh);
	void LoadBones(const aiNode* RootNode, const aiMesh* mesh, std::vector<Vertex3D>& VertexList);
	std::vector<MeshBoneWeights> LoadBoneWeights(aiMesh* mesh, std::vector<Vertex3D>& VertexList);
	std::shared_ptr<Material> LoadMaterial(const std::string& FilePath, aiMesh* mesh, const aiScene* scene);

	void from_json(nlohmann::json& json, uint64_t GameObjectID)
	{
		/*JsonConverter::from_json(json["ModelFilePath"], ModelFilePath);
		LoadModel(ModelFilePath, GameObjectID);

		for (int x = 0; x < json["MeshList"].size(); x++)
		{
			MeshList[x]->SetMaterial(json["MeshList"][x]["MaterialPath"]);

			glm::vec3 Position = glm::vec3(0.0f);
			JsonConverter::from_json(json["MeshList"][x]["MeshPosition"], Position);
			MeshList[x]->SetMeshPosition(Position);

			glm::vec3 Rotation = glm::vec3(0.0f);
			JsonConverter::from_json(json["MeshList"][x]["MeshRotation"], Rotation);
			MeshList[x]->SetMeshPosition(Rotation);

			glm::vec3 Scale = glm::vec3(0.0f);
			JsonConverter::from_json(json["MeshList"][x]["MeshScale"], Scale);
		    MeshList[x]->SetMeshPosition(Scale);
		}

		JsonConverter::from_json(json["ModelPosition"], ModelPosition);
		JsonConverter::from_json(json["ModelRotation"], ModelRotation);
		JsonConverter::from_json(json["ModelScale"], ModelScale);*/
	}

public:


	glm::vec3 ModelPosition = glm::vec3(0.0f);
	glm::vec3 ModelRotation = glm::vec3(0.0f);
	glm::vec3 ModelScale = glm::vec3(1.0f);

	Model();
	Model(const std::string& FilePath, uint64_t GameObjectID);
	Model(std::shared_ptr<Mesh> mesh, uint64_t GameObjectID);
	Model(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, uint64_t GameObjectID);
	Model(std::vector<Mesh>& meshList, uint64_t GameObjectID);
	Model(std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, uint64_t GameObjectID);
	Model(std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, std::shared_ptr<Material> materialPtr, uint64_t GameObjectID);
	Model(nlohmann::json& json, uint64_t GameObjectID);
	~Model();

	void AddMesh(std::shared_ptr<Mesh> mesh);
	void AddMesh(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);
	void AddMesh(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices);
	void AddMesh(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, std::shared_ptr<Material> materialPtr);
	void AddMesh(MeshLoadingInfo& meshLoader);

	void DeleteMesh(std::shared_ptr<Mesh> mesh);
	void RemoveMesh(std::shared_ptr<Mesh> mesh);

	void Update(const glm::mat4& GameObjectMatrix);
	void UpdateMeshTopLevelAccelerationStructure(std::vector<VkAccelerationStructureInstanceKHR>& AccelerationStructureInstanceList);
	void Destroy();

	bool DoesMeshExistInModel(std::shared_ptr<Mesh> mesh);

	void SetModelPosition(float x, float y, float z);
	void SetModelRotation(float x, float y, float z);
	void SetModelScale(float x, float y, float z);
	void SetModelPosition(glm::vec3 modelPosition);
	void SetModelRotation(glm::vec3 modelRotation);
	void SetModelScale(glm::vec3 modelScale);

	glm::mat4 TransposeModelMatrix();
	uint64_t GetModelID() { return ModelID; }
	uint64_t GetParentGameObjectID() { return ParentGameObjectID; }
	std::vector<std::shared_ptr<Mesh>> GetMeshList() { return MeshList; }

	std::string GetModelFilePath() { return ModelFilePath; }
	glm::mat4 GetModelMatrix() { return ModelTransform; }
	glm::vec3 GetModelPosition() { return ModelPosition; }
	glm::vec3 GetModelRotation() { return ModelRotation; }
	glm::vec3 GetModelScale() { return ModelScale; }
	glm::vec3* GetModelPositionPtr() { return &ModelPosition; }
	glm::vec3* GetModelRotationPtr() { return &ModelRotation; }
	glm::vec3* GetModelScalePtr() { return &ModelScale; }

	void to_json(nlohmann::json& json)
	{
		//JsonConverter::to_json(json["ModelFilePath"], ModelFilePath);

		////glm::mat4 ModelTransform = glm::mat4(1.0f);

		//for (int x = 0; x < MeshList.size(); x++)
		//{
		//	MeshList[x]->to_json(json["MeshList"][x]);
		//}
		////for (int x = 0; x < BoneList.size(); x++)
		////{
		////	BoneList[x]->to_json(json["BoneList"][x]);
		////}
		////for (int x = 0; x < NodeMapList.size(); x++)
		////{
		////	NodeMapList[x].to_json(json["NodeMapList"][x]);
		////}

		//JsonConverter::to_json(json["ModelPosition"], ModelPosition);
		//JsonConverter::to_json(json["ModelRotation"], ModelRotation);
		//JsonConverter::to_json(json["ModelScale"], ModelScale);
	}
};

