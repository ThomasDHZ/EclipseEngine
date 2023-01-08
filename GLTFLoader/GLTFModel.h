#pragma once
#include "GLTFFileLoader.h"
#include "GLTFMesh.h"
#include "GLTFTexture.h"
#include <Texture2D.h>
#include <Material.h>
#include <VRAMManager.h>

class GLTFModel
{
private:
	GLTFVertexEnum VertexType;
	tinygltf::Model model;

	std::vector<std::shared_ptr<Texture2D>> TextureList;
	std::vector<std::shared_ptr<Material>> MaterialList;
	std::vector<GLTFMesh> MeshList;
	glm::mat4 ModelMatrix;

	void LoadTextureDetails(const tinygltf::Image tinygltfImage, TinyGltfTextureLoader& TextureLoader);
	void LoadSamplerDetails(const tinygltf::Sampler tinygltfSampler, TinyGltfTextureSamplerLoader SamplerLoader);
	void LoadMaterials(tinygltf::Model& model);

public: 
	GLTFModel();
	GLTFModel(const char* filename, GLTFVertexEnum vertexType);
	~GLTFModel();

	void LoadModelNodes(tinygltf::Node& node);

	std::vector<GLTFMesh> GetGLTFMeshList() { return MeshList; };
};

