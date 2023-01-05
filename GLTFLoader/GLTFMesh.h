#pragma once
#include <iostream>
#include <VulkanBuffer.h>
#include "GLTFVertex.h"
#include <glm/glm/gtc/type_ptr.inl>
#include <tinygltf/tiny_gltf.h>
#include <type_traits>

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
	VulkanBuffer VertexBuffer;
	VulkanBuffer IndexBuffer;
	glm::mat4 MeshMatrix;

	uint32_t VertexSize;
	uint32_t VertexStart;
	uint32_t IndexCount;
	std::vector<GLTFPrimitive> GLTFPrimitiveList;

	const float* PositionBuffer;
	const float* NormalsBuffer;
	const float* UVBuffer;
	const float* TangantBuffer;

	void LoadPrimitive(tinygltf::Model& model, tinygltf::Mesh& mesh, uint32_t x)
	{
	/*	GLTFPrimitive primitive = {};

		const tinygltf::Primitive& glTFPrimitive = mesh.primitives[x];
		if (glTFPrimitive.attributes.find("POSITION") != glTFPrimitive.attributes.end()) {
			const tinygltf::Accessor& accessor = model.accessors[glTFPrimitive.attributes.find("POSITION")->second];
			primitive. = accessor.count;
		}*/
	}

	void LoadVertices(tinygltf::Model& model, tinygltf::Mesh& mesh, uint32_t x)
	{
		const tinygltf::Primitive& glTFPrimitive = mesh.primitives[x];
		size_t vertexCount = 0;

		if (glTFPrimitive.attributes.find("POSITION") != glTFPrimitive.attributes.end()) {
			const tinygltf::Accessor& accessor = model.accessors[glTFPrimitive.attributes.find("POSITION")->second];
			const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
			PositionBuffer = reinterpret_cast<const float*>(&(model.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
			vertexCount = accessor.count;
		}

		if (glTFPrimitive.attributes.find("NORMAL") != glTFPrimitive.attributes.end()) {
			const tinygltf::Accessor& accessor = model.accessors[glTFPrimitive.attributes.find("NORMAL")->second];
			const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
			NormalsBuffer = reinterpret_cast<const float*>(&(model.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
		}

		if (glTFPrimitive.attributes.find("TEXCOORD_0") != glTFPrimitive.attributes.end()) {
			const tinygltf::Accessor& accessor = model.accessors[glTFPrimitive.attributes.find("TEXCOORD_0")->second];
			const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
			UVBuffer = reinterpret_cast<const float*>(&(model.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
		}

		if (glTFPrimitive.attributes.find("TANGENT") != glTFPrimitive.attributes.end()) {
			const tinygltf::Accessor& accessor = model.accessors[glTFPrimitive.attributes.find("TANGENT")->second];
			const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
			TangantBuffer = reinterpret_cast<const float*>(&(model.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
		}

		switch (VertexType)
		{
			case GLTFVertexEnum::kGLTFLineVertex2D:
			{
				break;
			}
			case GLTFVertexEnum::kGLTFLineVertex3D:
			{
				break;
			}
			case GLTFVertexEnum::kGLTFVertex2D:
			{
				std::vector<GLTFVertex2D> VertexList;
				for (uint32_t x = 0; x < vertexCount; x++)
				{
					GLTFVertex2D vertex;
					vertex.Position = glm::vec4(glm::make_vec3(&PositionBuffer[x * 3]), 1.0f);
					vertex.UV = UVBuffer ? glm::make_vec2(&UVBuffer[x * 2]) : glm::vec3(0.0f);
					vertex.Color = glm::vec3(1.0f);
					VertexList.emplace_back(vertex);
				}
				break;
			}
			case GLTFVertexEnum::kGLTFVertex3D:
			{
				std::vector<GLTFVertex3D> VertexList;
				for (uint32_t x = 0; x < vertexCount; x++)
				{
					GLTFVertex3D vertex;
					vertex.Position = glm::vec4(glm::make_vec3(&PositionBuffer[x * 3]), 1.0f);
					vertex.Normal = glm::normalize(glm::vec3(NormalsBuffer ? glm::make_vec3(&NormalsBuffer[x * 3]) : glm::vec3(0.0f)));
					vertex.UV = UVBuffer ? glm::make_vec2(&UVBuffer[x * 2]) : glm::vec3(0.0f);
					vertex.Color = glm::vec3(1.0f);
					vertex.Tangant = TangantBuffer ? glm::make_vec4(&TangantBuffer[x * 4]) : glm::vec4(0.0f);
					VertexList.emplace_back(vertex);
				}
				break;
			}
			case GLTFVertexEnum::kGLTFInstancedData3D:
			{
				break;
			}
			default:
			{
				break;
			}
		}
	}

	void LoadIndices(tinygltf::Model& model, tinygltf::Mesh& mesh, uint32_t x)
	{
		const tinygltf::Primitive& glTFPrimitive = mesh.primitives[x];
		uint32_t indexCount = 0;

		const tinygltf::Accessor& accessor = model.accessors[glTFPrimitive.indices];
		const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
		const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

		std::vector<uint32_t> indexBufferz;
		indexCount += static_cast<uint32_t>(accessor.count);

		switch (accessor.componentType)
		{
			case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT: {
				const uint32_t* indexBuffer = reinterpret_cast<const uint32_t*>(&buffer.data[accessor.byteOffset + bufferView.byteOffset]);
				//for (uint32_t x = 0; x < accessor.count; x++)
				//{
				//	/*auto index = reinterpret_cast<uint32_t*>(indexBuffer[x] + VertexStart);
				//	indexBufferz.emplace_back(&index);*/
				//}
				break;
			}
			case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT: {
				/*const uint16_t* indexBuffer = reinterpret_cast<const uint16_t*>(&buffer.data[accessor.byteOffset + bufferView.byteOffset]);
				for (uint32_t x = 0; x < accessor.count; x++)
				{
					auto index = reinterpret_cast<uint16_t*>(indexBuffer[x] + VertexStart);
					indexBufferz.emplace_back(&index);
				}*/
				break;
			}
			case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE: {
			/*	const uint8_t* indexBuffer = reinterpret_cast<const uint8_t*>(&buffer.data[accessor.byteOffset + bufferView.byteOffset]);
				for (uint32_t x = 0; x < accessor.count; x++)
				{
					auto index = reinterpret_cast<uint8_t*>(indexBuffer[x] + VertexStart);
					indexBufferz.emplace_back(&index);;
				}*/
				break;
			}
			default:
			{
				std::cout << "Index component type " << accessor.componentType << " not supported!" << std::endl;
				return;
			}
		}
	}

public:
	GLTFMesh()
	{
		VertexType = kGLTFVertexUndefined;
		VertexSize = 0;
		VertexStart = 0;
		IndexCount = 0;

		PositionBuffer = nullptr;
		NormalsBuffer = nullptr;
		UVBuffer = nullptr;
		TangantBuffer = nullptr;
	}

	GLTFMesh(GLTFVertexEnum vertexType)
	{
		VertexType = vertexType;
		VertexSize = 0;
		VertexStart = 0;
		IndexCount = 0;

		PositionBuffer = nullptr;
		NormalsBuffer = nullptr;
		UVBuffer = nullptr;
		TangantBuffer = nullptr;
	}

	~GLTFMesh()
	{

	}

	void LoadMesh(tinygltf::Model& model, tinygltf::Mesh& mesh)
	{
		for (size_t x = 0; x < mesh.primitives.size(); x++)
		{
			LoadVertices(model, mesh, x);
			LoadIndices(model, mesh, x);
		}
	}
};

