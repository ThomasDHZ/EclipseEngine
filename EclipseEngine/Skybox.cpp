#include "Skybox.h"
Skybox::Skybox() : Mesh()
{
}

Skybox::~Skybox()
{
}

void Skybox::StartUp()
{
	const std::vector<MeshVertex> SkyBoxVertices =
	{
		{{-1.0f,  1.0f, -1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{-1.0f, -1.0f, -1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{ 1.0f, -1.0f, -1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{ 1.0f, -1.0f, -1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{ 1.0f,  1.0f, -1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{-1.0f,  1.0f, -1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},

		{{-1.0f, -1.0f,  1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{-1.0f, -1.0f, -1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{-1.0f,  1.0f, -1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{-1.0f,  1.0f, -1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{-1.0f,  1.0f,  1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{-1.0f, -1.0f,  1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},

		{{ 1.0f, -1.0f, -1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{ 1.0f, -1.0f,  1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{ 1.0f,  1.0f,  1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{ 1.0f,  1.0f,  1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{ 1.0f,  1.0f, -1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{ 1.0f, -1.0f, -1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},

		{{-1.0f, -1.0f,  1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{-1.0f,  1.0f,  1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{ 1.0f,  1.0f,  1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{ 1.0f,  1.0f,  1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{ 1.0f, -1.0f,  1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{-1.0f, -1.0f,  1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},

		{{-1.0f,  1.0f, -1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{ 1.0f,  1.0f, -1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{ 1.0f,  1.0f,  1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{ 1.0f,  1.0f,  1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{-1.0f,  1.0f,  1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{-1.0f,  1.0f, -1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},

		{{-1.0f, -1.0f, -1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{-1.0f, -1.0f,  1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{ 1.0f, -1.0f, -1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{ 1.0f, -1.0f, -1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{-1.0f, -1.0f,  1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }},
		{{ 1.0f, -1.0f,  1.0f }, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }, {0.0f, 0.0f, 0.0f}, { 0.0f }}
	};

	std::vector<uint32_t> IndexPlaceholder = { 0xFFFFFFFF };


	GenerateID();

	VertexList = SkyBoxVertices;
	//IndexList = IndexPlaceholder;

	ParentModelID = -1;
	ParentGameObjectID = -1;
	VertexCount = VertexList.size();
	IndexCount = IndexList.size();
	TriangleCount = static_cast<uint32_t>(IndexList.size()) / 3;
	BoneCount = 0;
	MeshType = MeshTypeEnum::kPolygon;

	material = MaterialManager::GetDefaultMaterial();

	glm::mat4 MeshTransform = glm::mat4(1.0f);
	MeshTransform = glm::transpose(MeshTransform);

	VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(MeshVertex), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	//IndexBuffer.CreateBuffer(IndexPlaceholder.data(), IndexPlaceholder.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformBuffer.CreateBuffer(&MeshTransform, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformInverseBuffer.CreateBuffer(&MeshTransform, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}