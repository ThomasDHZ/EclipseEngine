#pragma once
#include <iostream>
#include <VulkanBuffer.h>
#include "GLTFVertex.h"
#include <tinygltf/tiny_gltf.h>
#include <type_traits>
#include <glm/gtc/matrix_transform.hpp>

struct GLTFPrimitive
{
	uint32_t FirstIndex;
	uint32_t IndexCount;
	int32_t MaterialIndex;
};

class GLTFMesh
{
private:
	GLTFVertexEnum VertexType;
	VulkanBuffer IndexBuffer;
	glm::mat4 MeshMatrix;

	uint32_t VertexSize;
	uint32_t VertexStart;
	uint32_t IndexCount;
	std::vector<GLTFPrimitive> GLTFPrimitiveList;

	void LoadTransform(tinygltf::Model& model, tinygltf::Mesh& mesh, tinygltf::Node& node);
	void LoadVertices(tinygltf::Model& model, tinygltf::Mesh& mesh, uint32_t x);
	void LoadIndices(tinygltf::Model& model, tinygltf::Mesh& mesh, uint32_t x);

public:
	uint32_t VertexCount;
	VulkanBuffer VertexBuffer;

	GLTFMesh();
	GLTFMesh(GLTFVertexEnum vertexType);
	~GLTFMesh();

	void LoadMesh(tinygltf::Model& model, tinygltf::Mesh& mesh, tinygltf::Node& node);
};

