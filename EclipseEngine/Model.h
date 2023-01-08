#pragma once
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "MeshRendererManager.h"
#include "Converters.h"
#include "Mesh.h"
#include "Mesh3D.h"
#include "Bone.h"
#include "Animation3D.h"
#include "AnimationPlayer3D.h"
#include "ComputeAnimationPipeline.h"
#include <GLTFScene.h>
#include "TextureManager.h"

const unsigned int MAX_BONE_VERTEX_COUNT = 4;

struct ModelLoader
{
	uint64_t ParentGameObjectID = 0;

	glm::mat4 GameObjectTransform = glm::mat4(1.0f);
	glm::mat4 MeshTransform = glm::mat4(1.0f);

	std::string FilePath;
	InstancingDataStruct instanceData;

	MeshTypeEnum MeshType;
};

class Model
{
private:
	static uint64_t ModelIDCounter;

	std::string ModelFilePath;

	uint64_t ModelID = 0;
	uint64_t ParentGameObjectID = 0;

	glm::mat4 GameObjectTransform = glm::mat4(1.0f);
	glm::mat4 ModelTransform = glm::mat4(1.0f);
	glm::mat4 GlobalInverseTransformMatrix = glm::mat4(1.0f);

	std::vector<std::shared_ptr<Mesh>> MeshList;
	std::vector<std::shared_ptr<Bone>> BoneList;
	std::vector<Animation3D> AnimationList;
	std::vector<NodeMap> NodeMapList;

	AnimationPlayer3D AnimationPlayer;

	void GenerateID();
	void LoadModel(ModelLoader& modelLoader);
	void LoadMesh(ModelLoader& modelLoader, aiNode* node, const aiScene* scene);
	std::vector<Vertex3D> LoadVertices(aiMesh* mesh);
	std::vector<uint32_t> LoadIndices(aiMesh* mesh);
	void LoadNodeTree(const aiNode* Node, int parentNodeID = -1);
	void LoadAnimations(const aiScene* scene);
	void LoadBones(const aiNode* RootNode, const aiMesh* mesh, std::vector<Vertex3D>& VertexList);
	std::vector<MeshBoneWeights> LoadBoneWeights(aiMesh* mesh, std::vector<Vertex3D>& VertexList);
	void LoadBoneMeshTransform(const int NodeID, const glm::mat4& ParentMatrix);
	std::shared_ptr<Material> LoadMaterial(const std::string& FilePath, aiMesh* mesh, const aiScene* scene);

	void BoneWeightPlacement(std::vector<MeshBoneWeights>& VertexList, unsigned int vertexID, unsigned int bone_id, float weight);

public:


	glm::vec3 ModelPosition = glm::vec3(0.0f);
	glm::vec3 ModelRotation = glm::vec3(0.0f);
	glm::vec3 ModelScale = glm::vec3(1.0f);

	Model();
	Model(ModelLoader& modelLoader);
	Model(std::shared_ptr<Mesh> mesh, uint64_t GameObjectID);
	Model(std::vector<std::shared_ptr<Mesh>>& meshList, uint64_t GameObjectID);
	Model(MeshLoader3D& meshLoader);
	Model(std::vector<MeshLoader3D>& meshLoaderList);
	~Model();

	void AddMesh(MeshLoader3D& meshLoader);
	void AddMesh(std::shared_ptr<Mesh> mesh);

	void AddMeshList(std::vector<MeshLoader3D>& meshLoaderList);
	void AddMeshList(std::vector<std::shared_ptr<Mesh>>& mesh);

	void DeleteMesh(std::shared_ptr<Mesh> mesh);
	void RemoveMesh(std::shared_ptr<Mesh> mesh);

	void Update(const glm::mat4& GameObjectMatrix);
	void UpdateMeshTopLevelAccelerationStructure(const glm::mat4& GameObjectMatrix, std::vector<VkAccelerationStructureInstanceKHR>& AccelerationStructureInstanceList);
	void Destroy();

	bool DoesMeshExistInModel(std::shared_ptr<Mesh> mesh);

	void SetModelPosition(float x, float y, float z);
	void SetModelRotation(float x, float y, float z);
	void SetModelScale(float x, float y, float z);
	void SetModelPosition(const glm::vec3& modelPosition);
	void SetModelRotation(const glm::vec3& modelRotation);
	void SetModelScale(const glm::vec3& modelScale);

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
};

