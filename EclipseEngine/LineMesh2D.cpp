#include "LineMesh2D.h"

LineMesh2D::LineMesh2D()
{
}

LineMesh2D::LineMesh2D(std::vector<LineVertex3D>& vertices, uint64_t parentGameObjectID)
{
	GenerateID();
	GenerateColorID();

	IndexList = {

	};

	ParentModelID = -1;
	ParentGameObjectID = parentGameObjectID;
	VertexCount = VertexList.size();
	IndexCount = IndexList.size();
	TriangleCount = static_cast<uint32_t>(IndexList.size()) / 3;
	BoneCount = 0;

	MeshType = MeshTypeEnum::kLine;

	material = MaterialManager::GetDefaultMaterial();

	glm::mat4 MeshTransform = glm::mat4(1.0f);
	MeshTransform = glm::transpose(MeshTransform);

	if (GraphicsDevice::IsRayTracingFeatureActive())
	{
		VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(LineVertex3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}
}

LineMesh2D::LineMesh2D(glm::vec2& StartPoint, glm::vec2& EndPoint, uint64_t parentGameObjectID)
{
	GenerateID();
	GenerateColorID();

	std::vector<LineVertex3D> VertexList2 = {
	{{StartPoint.x, StartPoint.y, 0.0f}, {1.0f, 0.0f, 0.0f}},
	{{EndPoint.x, EndPoint.y, 0.0f}, {0.0f, 1.0f, 0.0f}}
	};

	IndexList = {

	};

	ParentModelID = -1;
	ParentGameObjectID = parentGameObjectID;
	VertexCount = VertexList2.size();
	IndexCount = IndexList.size();
	TriangleCount = static_cast<uint32_t>(IndexList.size()) / 3;
	BoneCount = 0;

	MeshType = MeshTypeEnum::kLine;

	material = MaterialManager::GetDefaultMaterial();

	glm::mat4 MeshTransform = glm::mat4(1.0f);
	MeshTransform = glm::transpose(MeshTransform);

	if (GraphicsDevice::IsRayTracingFeatureActive())
	{
		VertexBuffer.CreateBuffer(VertexList2.data(), VertexList2.size() * sizeof(LineVertex3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}
}

LineMesh2D::~LineMesh2D()
{
}
