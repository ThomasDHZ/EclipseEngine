#pragma once
#include "GLTFFileLoader.h"
#include <Texture2D.h>
#include <Material.h>
#include <VRAMManager.h>
#include "GLTFVertex.h"

struct GLTFPrimitive
{
	uint32_t FirstIndex = 0;
	uint32_t IndexCount = 0;
	uint32_t material = 0;
};

struct GLTFModelData
{
	std::vector<GLTFVertex> VertexList = std::vector<GLTFVertex>();
	std::vector<uint32_t> IndexList = std::vector<uint32_t>();
	glm::mat4 TransformMatrix = glm::mat4(1.0f);
	std::vector<GLTFPrimitive> GLTFPrimitiveList = std::vector<GLTFPrimitive>();
	std::vector<std::shared_ptr<Texture2D>> TextureList = std::vector<std::shared_ptr<Texture2D>>();
	std::vector<std::shared_ptr<Material>> MaterialList = std::vector<std::shared_ptr<Material>>();

	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);
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
	std::vector<GLTFPrimitive> GLTFPrimitiveList;
	glm::mat4 TransformMatrix;

	tinygltf::Model model;

	GLTFModelData data;

	void LoadTextureDetails(const tinygltf::Image tinygltfImage, TinyGltfTextureLoader& TextureLoader);
	void LoadSamplerDetails(const tinygltf::Sampler tinygltfSampler, TinyGltfTextureSamplerLoader SamplerLoader);
	void LoadMaterial(tinygltf::Model& model);
	void LoadTransform(tinygltf::Model& model, tinygltf::Mesh& mesh, tinygltf::Node& node);
	void LoadVertices(tinygltf::Model& model, tinygltf::Mesh& mesh, uint32_t x);
	void LoadIndices(tinygltf::Model& model, tinygltf::Mesh& mesh, uint32_t x);
	void LoadPrimitive(tinygltf::Mesh& mesh, uint32_t x);
	void LoadMesh(tinygltf::Model& model, tinygltf::Mesh& mesh, tinygltf::Node& node);
	void LoadModelNodes(tinygltf::Node& node);
	void Loader();

public: 
	GLTFModel();
	GLTFModel(const char* filename);
	~GLTFModel();

	GLTFModelData GetModelData() { return data; }
};
