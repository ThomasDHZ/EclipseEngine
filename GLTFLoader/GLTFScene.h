#pragma once
#include "GLTFFileLoader.h"
#include "GLTFModel.h"

class GLTFScene
{
private:
	uint32_t VertexSize;
	VulkanBuffer VertexBuffer;
	VulkanBuffer IndexBuffer;

	void ModelNodes(tinygltf::Node& node);
	void LoadMesh(tinygltf::Mesh& mesh);

public:
	tinygltf::Model model;

	GLTFScene(const char* filename, uint32_t vertexSize);
	~GLTFScene();
};

