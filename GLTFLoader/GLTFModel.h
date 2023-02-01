#pragma once
#include "GLTFFileLoader.h"
#include <Texture2D.h>
#include <GLTFMaterial.h>
#include <VRAMManager.h>
#include "GLTFVertex.h"

struct GLTFPrimitive
{
	uint32_t FirstIndex = 0;
	uint32_t IndexCount = 0;
	uint32_t material = 0;
};

struct GLTFNode
{
public:
	std::string NodeName;

	uint64_t ParentGameObjectID = 0;
	uint64_t ParentModelID = 0;
	int NodeID = -1;

	std::shared_ptr<GLTFNode> ParentMesh;
	std::vector<std::shared_ptr<GLTFNode>> ChildMeshList;
	std::shared_ptr<GLTFMaterial> Material = nullptr;

	std::vector<GLTFPrimitive> PrimitiveList;
	VulkanBuffer TransformBuffer;

	glm::mat4 ModelTransformMatrix = glm::mat4(1.0f);
	glm::mat4 NodeTransformMatrix = glm::mat4(1.0f);
	bool Visible = true;

	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);

	GLTFNode(){}
	~GLTFNode(){}
};

struct GLTFModelData
{
	std::vector<GLTFVertex> VertexList = std::vector<GLTFVertex>();
	std::vector<uint32_t> IndexList = std::vector<uint32_t>();
	std::vector<std::shared_ptr<GLTFNode>> NodeList = std::vector<std::shared_ptr<GLTFNode>>();
	std::vector<std::shared_ptr<Texture2D>> TextureList = std::vector<std::shared_ptr<Texture2D>>();
	std::vector<std::shared_ptr<GLTFMaterial>> MaterialList = std::vector<std::shared_ptr<GLTFMaterial>>();
};

class GLTFModel
{
private:
	uint32_t VertexCount;
	uint32_t IndexCount;
	uint32_t VertexStartIndex;
	uint32_t FirstIndex;

	std::vector<GLTFVertex> VertexList;
	std::vector<uint32_t> IndexList;
	std::vector<std::shared_ptr<GLTFNode>> NodeList;
	tinygltf::Model model;

	GLTFModelData data;

	void LoadTextureDetails(const tinygltf::Image tinygltfImage, TinyGltfTextureLoader& TextureLoader);
	void LoadSamplerDetails(const tinygltf::Sampler tinygltfSampler, TinyGltfTextureSamplerLoader SamplerLoader);
	void LoadMaterial(tinygltf::Model& model);
	void LoadTransform(tinygltf::Model& model, tinygltf::Mesh& mesh, tinygltf::Node& node);
	void LoadVertices(tinygltf::Model& model, tinygltf::Mesh& mesh, uint32_t x);
	void LoadIndices(tinygltf::Model& model, tinygltf::Mesh& mesh, uint32_t x);
	void LoadPrimitive(tinygltf::Mesh& mesh, uint32_t x);
	void LoadMesh(tinygltf::Model& model, tinygltf::Node& node, std::shared_ptr<GLTFNode> parentNode, int index);
	void Loader();

public: 
	GLTFModel();
	GLTFModel(const char* filename);
	~GLTFModel();

	GLTFModelData GetModelData() { return data; }
};

