#include "LineMesh3D.h"

void LineMesh3D::BuildGrid3D(glm::vec3 GridSize, glm::vec3 GridSpacing)
{
}

LineMesh3D::LineMesh3D(std::vector<LineVertex3D>& vertices)
{
	GenerateID();
	GenerateColorID();

	LineVertexList = vertices;
	for (int x = 0; x <= LineVertexList.size(); x++)
	{
		LineIndexList.emplace_back(x);
	}

	MeshType = MeshTypeEnum::kLine;
	material = MaterialManager::GetDefaultMaterial();
	MeshTransformMatrix = glm::mat4(1.0f);
	ParentModelID = -1;
	ParentGameObjectID = -1;
	VertexCount = LineVertexList.size();
	IndexCount = LineIndexList.size();
	TriangleCount = static_cast<uint32_t>(LineIndexList.size()) / 3;
	BoneCount = 0;

	if (GraphicsDevice::IsRayTracingFeatureActive())
	{
		VertexBuffer.CreateBuffer(LineVertexList.data(), LineVertexList.size() * sizeof(LineVertex3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		IndexBuffer.CreateBuffer(LineIndexList.data(), LineIndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		TransformBuffer.CreateBuffer(&MeshTransformMatrix, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}
}

LineMesh3D::LineMesh3D(glm::vec3& StartPoint, glm::vec3& EndPoint)
{
	GenerateID();
	GenerateColorID();

	LineVertexList =
	{
		{StartPoint, glm::vec4(1.0f)},
		{EndPoint, glm::vec4(1.0f)}
	};
	LineIndexList = {
		0, 1
	};

	MeshType = MeshTypeEnum::kLine;
	material = MaterialManager::GetDefaultMaterial();
	MeshTransformMatrix = glm::mat4(1.0f);
	ParentModelID = -1;
	ParentGameObjectID = -1;
	VertexCount = LineVertexList.size();
	IndexCount = LineIndexList.size();
	TriangleCount = static_cast<uint32_t>(LineIndexList.size()) / 3;
	BoneCount = 0;

	if (GraphicsDevice::IsRayTracingFeatureActive())
	{
		VertexBuffer.CreateBuffer(LineVertexList.data(), LineVertexList.size() * sizeof(LineVertex3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		IndexBuffer.CreateBuffer(LineIndexList.data(), LineIndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}
}

LineMesh3D::LineMesh3D(glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec3& Color)
{
	GenerateID();
	GenerateColorID();

	LineVertexList =
	{
		{StartPoint, glm::vec4(Color.x, Color.y, Color.z, 1.0f)},
		{EndPoint, glm::vec4(Color.x, Color.y, Color.z, 1.0f)}
	};
	LineIndexList = {
		0, 1
	};

	MeshType = MeshTypeEnum::kLine;
	material = MaterialManager::GetDefaultMaterial();
	MeshTransformMatrix = glm::mat4(1.0f);
	ParentModelID = -1;
	ParentGameObjectID = -1;
	VertexCount = LineVertexList.size();
	IndexCount = LineIndexList.size();
	TriangleCount = static_cast<uint32_t>(LineIndexList.size()) / 3;
	BoneCount = 0;

	if (GraphicsDevice::IsRayTracingFeatureActive())
	{
		VertexBuffer.CreateBuffer(LineVertexList.data(), LineVertexList.size() * sizeof(LineVertex3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		IndexBuffer.CreateBuffer(LineIndexList.data(), LineIndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		TransformBuffer.CreateBuffer(&MeshTransformMatrix, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}
}

LineMesh3D::LineMesh3D(glm::vec3& StartPoint, glm::vec3& EndPoint, glm::vec4& Color)
{
	GenerateID();
	GenerateColorID();

	LineVertexList =
	{
		{StartPoint, Color},
		{EndPoint, Color}
	};
	LineIndexList = {
		0, 1
	};

	MeshType = MeshTypeEnum::kLine;
	material = MaterialManager::GetDefaultMaterial();
	MeshTransformMatrix = glm::mat4(1.0f);
	ParentModelID = -1;
	ParentGameObjectID = -1;
	VertexCount = LineVertexList.size();
	IndexCount = LineIndexList.size();
	TriangleCount = static_cast<uint32_t>(LineIndexList.size()) / 3;
	BoneCount = 0;

	if (GraphicsDevice::IsRayTracingFeatureActive())
	{
		VertexBuffer.CreateBuffer(LineVertexList.data(), LineVertexList.size() * sizeof(LineVertex3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		IndexBuffer.CreateBuffer(LineIndexList.data(), LineIndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		TransformBuffer.CreateBuffer(&MeshTransformMatrix, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}
}

LineMesh3D::LineMesh3D(int GridSizeX, int GridSizeY, int GridSizeZ, float GridSpacingX, float GridSpacingY, float GridSpacingZ)
{
	GenerateID();
	GenerateColorID();

	uint32_t indexCount = 0;
	glm::vec3 StartPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 EndPoint = glm::vec3((GridSizeX * GridSpacingX), 0.0f, 0.0f);
	LineVertexList.emplace_back(LineVertex3D({ StartPoint.x , StartPoint.y, StartPoint.z }, { 1.0f, 0.0f, 0.0f, 1.0f }));
	LineVertexList.emplace_back(LineVertex3D({ EndPoint.x, EndPoint.y, EndPoint.z }, { 1.0f, 0.0f, 0.0f, 1.0f }));
	LineIndexList.emplace_back(indexCount++);
	LineIndexList.emplace_back(indexCount++);

	StartPoint = glm::vec3(0.0f, 0.0f, 0.0);
	EndPoint = glm::vec3(0.0f, (GridSizeY * GridSpacingY), 0.0f);
	LineVertexList.emplace_back(LineVertex3D({ StartPoint.x , StartPoint.y, StartPoint.z }, { 1.0f, 0.0f, 0.0f, 1.0f }));
	LineVertexList.emplace_back(LineVertex3D({ EndPoint.x, EndPoint.y, EndPoint.z }, { 1.0f, 0.0f, 0.0f, 1.0f }));
	LineIndexList.emplace_back(indexCount++);
	LineIndexList.emplace_back(indexCount++);

	StartPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	EndPoint = glm::vec3(0.0f, 0.0f, (GridSizeZ * GridSpacingZ));
	LineVertexList.emplace_back(LineVertex3D({ StartPoint.x , StartPoint.y, StartPoint.z }, { 1.0f, 0.0f, 0.0f, 1.0f }));
	LineVertexList.emplace_back(LineVertex3D({ EndPoint.x, EndPoint.y, EndPoint.z }, { 1.0f, 0.0f, 0.0f, 1.0f }));
	LineIndexList.emplace_back(indexCount++);
	LineIndexList.emplace_back(indexCount++);

	StartPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	EndPoint = glm::vec3(-(GridSizeX * GridSpacingX), 0.0f, 0.0f);
	LineVertexList.emplace_back(LineVertex3D({ StartPoint.x , StartPoint.y, StartPoint.z }, { 1.0f, 0.0f, 0.0f, 1.0f }));
	LineVertexList.emplace_back(LineVertex3D({ EndPoint.x, EndPoint.y, EndPoint.z }, { 1.0f, 0.0f, 0.0f, 1.0f }));
	LineIndexList.emplace_back(indexCount++);
	LineIndexList.emplace_back(indexCount++);

	StartPoint = glm::vec3(0.0f, 0.0f, 0.0);
	EndPoint = glm::vec3(0.0f, -(GridSizeY * GridSpacingY), 0.0f);
	LineVertexList.emplace_back(LineVertex3D({ StartPoint.x , StartPoint.y, StartPoint.z }, { 1.0f, 0.0f, 0.0f, 1.0f }));
	LineVertexList.emplace_back(LineVertex3D({ EndPoint.x, EndPoint.y, EndPoint.z }, { 1.0f, 0.0f, 0.0f, 1.0f }));
	LineIndexList.emplace_back(indexCount++);
	LineIndexList.emplace_back(indexCount++);

	StartPoint = glm::vec3(0.0f, 0.0f, -(GridSizeZ * GridSpacingZ));
	EndPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	LineVertexList.emplace_back(LineVertex3D({ StartPoint.x , StartPoint.y, StartPoint.z }, { 1.0f, 0.0f, 0.0f, 1.0f }));
	LineVertexList.emplace_back(LineVertex3D({ EndPoint.x, EndPoint.y, EndPoint.z }, { 1.0f, 0.0f, 0.0f, 1.0f }));
	LineIndexList.emplace_back(indexCount++);
	LineIndexList.emplace_back(indexCount++);


	ParentModelID = -1;
	VertexCount = LineVertexList.size();
	IndexCount = LineIndexList.size();
	TriangleCount = static_cast<uint32_t>(LineIndexList.size()) / 3;
	BoneCount = 0;

	MeshType = MeshTypeEnum::kLine;

	material = MaterialManager::GetDefaultMaterial();

	glm::mat4 MeshTransform = glm::mat4(1.0f);
	MeshTransform = glm::transpose(MeshTransform);


	VertexBuffer.CreateBuffer(LineVertexList.data(), LineVertexList.size() * sizeof(LineVertex3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(LineIndexList.data(), LineIndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformBuffer.CreateBuffer(&MeshTransformMatrix, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

}

LineMesh3D::~LineMesh3D()
{
}

void LineMesh3D::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	VertexBuffer.CopyBufferToMemory(LineVertexList.data(), LineVertexList.size() * sizeof(LineVertex3D));
	IndexBuffer.CopyBufferToMemory(LineIndexList.data(), LineIndexList.size() * sizeof(uint32_t));

	Mesh3D::Update(GameObjectMatrix, ModelMatrix);
}

void LineMesh3D::SetMeshPosition(float x, float y, float z)
{
	MeshPosition = glm::vec3(x, y, z);
}


void LineMesh3D::SetMeshRotation(float x, float y, float z)
{
	MeshRotation = glm::vec3(x, y, z);
}

void LineMesh3D::SetMeshScale(float x, float y, float z)
{
	MeshScale = glm::vec3(x, y, z);
}

void LineMesh3D::SetMeshPosition(glm::vec3 position)
{
	MeshPosition = position;
}


void LineMesh3D::SetMeshRotation(glm::vec3 rotation)
{
	MeshRotation = rotation;
}

void LineMesh3D::SetMeshScale(glm::vec3 scale)
{
	MeshScale = scale;
}