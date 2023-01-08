#include "GLTFMesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.inl>

	GLTFMesh::GLTFMesh()
	{
		VertexType = kGLTFVertexUndefined;
		VertexSize = 0;
		VertexStart = 0;
		VertexCount = 0;
		IndexCount = 0;
	}

	GLTFMesh::GLTFMesh(GLTFVertexEnum vertexType)
	{
		VertexType = vertexType;
		VertexSize = 0;
		VertexStart = 0;
		VertexCount = 0;
		IndexCount = 0;
	}

	GLTFMesh::~GLTFMesh()
	{

	}

	void GLTFMesh::LoadTransform(tinygltf::Model& model, tinygltf::Mesh& mesh, tinygltf::Node& node)
	{
		if (node.translation.size() == 3) 
		{
			MeshMatrix = glm::translate(MeshMatrix, glm::vec3(glm::make_vec3(node.translation.data())));
		}
		if (node.rotation.size() == 4) 
		{
			//MeshMatrix *= glm::mat4(glm::make_quat(node.rotation.data()));
		}
		if (node.scale.size() == 3) 
		{
			MeshMatrix = glm::scale(MeshMatrix, glm::vec3(glm::make_vec3(node.scale.data())));
		}
		if (node.matrix.size() == 16) 
		{
			MeshMatrix = glm::make_mat4x4(node.matrix.data());
		};
	}

	void GLTFMesh::LoadVertices(tinygltf::Model& model, tinygltf::Mesh& mesh, uint32_t x)
	{
		const tinygltf::Primitive& glTFPrimitive = mesh.primitives[x];
		const float* PositionBuffer = nullptr;
		const float* NormalsBuffer = nullptr;
		const float* UVBuffer = nullptr;
		const float* ColorBuffer = nullptr;
		const float* TangantBuffer = nullptr;

		if (glTFPrimitive.attributes.find("POSITION") != glTFPrimitive.attributes.end()) {
			const tinygltf::Accessor& accessor = model.accessors[glTFPrimitive.attributes.find("POSITION")->second];
			const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
			PositionBuffer = reinterpret_cast<const float*>(&(model.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
			VertexCount = accessor.count;
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

		if (glTFPrimitive.attributes.find("COLOR_0") != glTFPrimitive.attributes.end()) {
			const tinygltf::Accessor& accessor = model.accessors[glTFPrimitive.attributes.find("COLOR_0")->second];
			const tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
			ColorBuffer = reinterpret_cast<const float*>(&(model.buffers[view.buffer].data[accessor.byteOffset + view.byteOffset]));
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
				std::vector<GLTFLineVertex2D> VertexList;
				for (uint32_t x = 0; x < VertexCount; x++)
				{
					GLTFLineVertex2D vertex;
					vertex.pos = glm::vec4(glm::make_vec3(&PositionBuffer[x * 3]), 1.0f);
					vertex.color = ColorBuffer ? glm::make_vec4(&ColorBuffer[x * 4]) : glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
					VertexList.emplace_back(vertex);
				}

				VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(GLTFLineVertex2D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
				break;
			}
			case GLTFVertexEnum::kGLTFLineVertex3D:
			{
				std::vector<GLTFLineVertex3D> VertexList;
				for (uint32_t x = 0; x < VertexCount; x++)
				{
					GLTFLineVertex3D vertex;
					vertex.pos = glm::vec4(glm::make_vec3(&PositionBuffer[x * 3]), 1.0f);
					vertex.color = ColorBuffer ? glm::make_vec4(&ColorBuffer[x * 4]) : glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
					VertexList.emplace_back(vertex);
				}

				VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(GLTFLineVertex3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
				break;
			}
			case GLTFVertexEnum::kGLTFVertex2D:
			{
				std::vector<GLTFVertex2D> VertexList;
				for (uint32_t x = 0; x < VertexCount; x++)
				{
					GLTFVertex2D vertex;
					vertex.Position = glm::vec4(glm::make_vec3(&PositionBuffer[x * 3]), 1.0f);
					vertex.UV = UVBuffer ? glm::make_vec2(&UVBuffer[x * 2]) : glm::vec3(0.0f);
					vertex.Color = ColorBuffer ? glm::make_vec4(&ColorBuffer[x * 4]) : glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
					VertexList.emplace_back(vertex);
				}

				VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(GLTFVertex2D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
				break;
			}
			case GLTFVertexEnum::kGLTFVertex3D:
			{
				std::vector<GLTFVertex3D> VertexList;
				for (uint32_t x = 0; x < VertexCount; x++)
				{
					GLTFVertex3D vertex;
					vertex.Position = glm::vec4(glm::make_vec3(&PositionBuffer[x * 3]), 1.0f);
					vertex.Normal = glm::normalize(glm::vec3(NormalsBuffer ? glm::make_vec3(&NormalsBuffer[x * 3]) : glm::vec3(0.0f)));
					vertex.UV = UVBuffer ? glm::make_vec2(&UVBuffer[x * 2]) : glm::vec3(0.0f);
					vertex.Color = ColorBuffer ? glm::make_vec4(&ColorBuffer[x * 4]) : glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
					vertex.Tangant = TangantBuffer ? glm::make_vec4(&TangantBuffer[x * 4]) : glm::vec4(0.0f);
					VertexList.emplace_back(vertex);
				}
				
				VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(GLTFVertex3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
				break;
			}
			case GLTFVertexEnum::kGLTFInstancedData3D:
			{
				std::vector<GLTFVertex3D> VertexList;
				std::vector<GLTFInstancedData3D> InstanceVertexList;
				for (uint32_t x = 0; x < VertexCount; x++)
				{
					GLTFVertex3D vertex;
					vertex.Position = glm::vec4(glm::make_vec3(&PositionBuffer[x * 3]), 1.0f);
					vertex.Normal = glm::normalize(glm::vec3(NormalsBuffer ? glm::make_vec3(&NormalsBuffer[x * 3]) : glm::vec3(0.0f)));
					vertex.UV = UVBuffer ? glm::make_vec2(&UVBuffer[x * 2]) : glm::vec3(0.0f);
					vertex.Color = ColorBuffer ? glm::make_vec4(&ColorBuffer[x * 4]) : glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
					vertex.Tangant = TangantBuffer ? glm::make_vec4(&TangantBuffer[x * 4]) : glm::vec4(0.0f);
					VertexList.emplace_back(vertex);

				/*	GLTFInstancedData3D instanceData;
					vertex.instanceData = glm::vec4(glm::make_vec3(&PositionBuffer[x * 3]), 1.0f);
					vertex.Normal = glm::normalize(glm::vec3(NormalsBuffer ? glm::make_vec3(&NormalsBuffer[x * 3]) : glm::vec3(0.0f)));
					vertex.UV = UVBuffer ? glm::make_vec2(&UVBuffer[x * 2]) : glm::vec3(0.0f);
					vertex.Color = ColorBuffer ? glm::make_vec4(&ColorBuffer[x * 4]) : glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
					vertex.Tangant = TangantBuffer ? glm::make_vec4(&TangantBuffer[x * 4]) : glm::vec4(0.0f);
					VertexList.emplace_back(vertex);*/
				}

				VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(GLTFVertex3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
				break;
			}
			default:
			{
				std::cout << "Texture type not found or undefined." << std::endl;
				break;
			}
		}
	}

	void GLTFMesh::LoadIndices(tinygltf::Model& model, tinygltf::Mesh& mesh, uint32_t x)
	{
		const tinygltf::Primitive& glTFPrimitive = mesh.primitives[x];
		uint32_t indexCount = 0;

		const tinygltf::Accessor& accessor = model.accessors[glTFPrimitive.indices];
		const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
		const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

		std::vector<uint32_t> IndexBufferList;
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
			IndexCount = static_cast<uint32_t>(IndexBufferList.size());
		}
	}

	void GLTFMesh::LoadMesh(tinygltf::Model& model, tinygltf::Mesh& mesh, tinygltf::Node& node)
	{
		LoadTransform(model, mesh, node);
		for (size_t x = 0; x < mesh.primitives.size(); x++)
		{
			LoadVertices(model, mesh, x);
			LoadIndices(model, mesh, x);
		}
	}