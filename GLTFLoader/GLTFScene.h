#pragma once
#include <iostream>
#include <VulkanBuffer.h>

#include <tinygltf/tiny_gltf.h>

class GLTFModel
{
private:
	uint32_t VertexSize;
	VulkanBuffer VertexBuffer;
	VulkanBuffer IndexBuffer;

	void ModelNodes(tinygltf::Node& node);
	void ModelMesh(tinygltf::Mesh& mesh);

public:
	tinygltf::Model model;

	GLTFModel(const char* filename, uint32_t vertexSize);
	~GLTFModel();
};

