#include "LineMesh2D.h"

LineMesh2D::LineMesh2D()
{
}

LineMesh2D::LineMesh2D(std::vector<LineVertex2D>& lineVertexList, uint64_t parentGameObjectID)
{
	GenerateID();
	GenerateColorID();

	LineVertexList = lineVertexList;
	for (int x = 0; x <= LineVertexList.size(); x++)
	{
		LineIndexList.emplace_back(x);
	}

	ParentModelID = -1;
	ParentGameObjectID = parentGameObjectID;
	VertexCount = LineVertexList.size();
	IndexCount = LineIndexList.size();
	TriangleCount = static_cast<uint32_t>(LineIndexList.size()) / 3;
	BoneCount = 0;

	MeshType = MeshTypeEnum::kLine;
	material = MaterialManager::GetDefaultMaterial();

	VertexBuffer.CreateBuffer(LineVertexList.data(), LineVertexList.size() * sizeof(LineVertex2D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(LineIndexList.data(), LineIndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

LineMesh2D::LineMesh2D(glm::vec2& StartPoint, glm::vec2& EndPoint, uint64_t parentGameObjectID)
{
	GenerateID();
	GenerateColorID();

	LineVertexList =
	{
		{{StartPoint.x, StartPoint.y}, glm::vec4(1.0f)},
		{{EndPoint.x, EndPoint.y}, glm::vec4(1.0f)}
	};
	LineIndexList = {
		0, 1
	};

	ParentModelID = -1;
	ParentGameObjectID = parentGameObjectID;
	VertexCount = LineVertexList.size();
	IndexCount = LineIndexList.size();
	TriangleCount = static_cast<uint32_t>(LineIndexList.size()) / 3;
	BoneCount = 0;

	MeshType = MeshTypeEnum::kLine;
	material = MaterialManager::GetDefaultMaterial();

	VertexBuffer.CreateBuffer(LineVertexList.data(), LineVertexList.size() * sizeof(LineVertex2D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(LineIndexList.data(), LineIndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

}

LineMesh2D::LineMesh2D(glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec3& LineColor, uint64_t parentGameObjectID)
{
	GenerateID();
	GenerateColorID();

	LineVertexList =
	{
		{{StartPoint.x, StartPoint.y}, glm::vec4(LineColor.x, LineColor.y, LineColor.z, 1.0f)},
		{{EndPoint.x, EndPoint.y}, glm::vec4(LineColor.x, LineColor.y, LineColor.z, 1.0f)}
	};
	LineIndexList = {
		0, 1
	};

	ParentModelID = -1;
	ParentGameObjectID = parentGameObjectID;
	VertexCount = LineVertexList.size();
	IndexCount = LineIndexList.size();
	TriangleCount = static_cast<uint32_t>(LineIndexList.size()) / 3;
	BoneCount = 0;

	MeshType = MeshTypeEnum::kLine;
	material = MaterialManager::GetDefaultMaterial();

	VertexBuffer.CreateBuffer(LineVertexList.data(), LineVertexList.size() * sizeof(LineVertex2D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(LineIndexList.data(), LineIndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

}

LineMesh2D::LineMesh2D(glm::vec2& StartPoint, glm::vec2& EndPoint, glm::vec4& LineColor, uint64_t parentGameObjectID)
{
	GenerateID();
	GenerateColorID();

	LineVertexList =
	{
		{{StartPoint.x, StartPoint.y}, LineColor},
		{{EndPoint.x, EndPoint.y}, LineColor}
	};
	LineIndexList = {
		0, 1
	};

	ParentModelID = -1;
	ParentGameObjectID = parentGameObjectID;
	VertexCount = LineVertexList.size();
	IndexCount = LineIndexList.size();
	TriangleCount = static_cast<uint32_t>(LineIndexList.size()) / 3;
	BoneCount = 0;

	MeshType = MeshTypeEnum::kLine;
	material = MaterialManager::GetDefaultMaterial();

	VertexBuffer.CreateBuffer(LineVertexList.data(), LineVertexList.size() * sizeof(LineVertex2D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(LineIndexList.data(), LineIndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

LineMesh2D::LineMesh2D(int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, uint64_t parentGameObjectID)
{
	GenerateID();
	GenerateColorID();

	uint32_t indexCount = 0;
	for (int x = 0; x <= GridSizeX; x++)
	{
		glm::vec2 StartPoint = glm::vec2(0.0f, 0.0f);
		glm::vec2 EndPoint = glm::vec2(0.0f, (GridSizeY * GridSpacingY));
		LineVertexList.emplace_back(LineVertex2D({ StartPoint.x + (x * GridSpacingX), StartPoint.y }, { glm::vec4(1.0f) }));
		LineVertexList.emplace_back(LineVertex2D({ EndPoint.x + (x * GridSpacingX), EndPoint.y }, { glm::vec4(1.0f) }));
		LineIndexList.emplace_back(indexCount++);
		LineIndexList.emplace_back(indexCount++);
	}
	for (int y = 0; y <= GridSizeY; y++)
	{
		glm::vec2 StartPoint = glm::vec2(0.0f, 0.0f);
		glm::vec2 EndPoint = glm::vec2((GridSizeX * GridSpacingX), 0.0f);
		LineVertexList.emplace_back(LineVertex2D({ StartPoint.x, StartPoint.y + (y * GridSpacingY) }, { glm::vec4(1.0f) }));
		LineVertexList.emplace_back(LineVertex2D({ EndPoint.x , EndPoint.y + (y * GridSpacingY) }, { glm::vec4(1.0f) }));
		LineIndexList.emplace_back(indexCount++);
		LineIndexList.emplace_back(indexCount++);
	}

	ParentModelID = -1;
	ParentGameObjectID = parentGameObjectID;
	VertexCount = LineVertexList.size();
	IndexCount = LineIndexList.size();
	TriangleCount = static_cast<uint32_t>(LineIndexList.size()) / 3;
	BoneCount = 0;

	MeshType = MeshTypeEnum::kLine;
	material = MaterialManager::GetDefaultMaterial();

	VertexBuffer.CreateBuffer(LineVertexList.data(), LineVertexList.size() * sizeof(LineVertex2D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(LineIndexList.data(), LineIndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

}

LineMesh2D::LineMesh2D(int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, glm::vec3& LineColor, uint64_t parentGameObjectID)
{
	GenerateID();
	GenerateColorID();

	uint32_t indexCount = 0;
	for (int x = 0; x <= GridSizeX; x++)
	{
		glm::vec2 StartPoint = glm::vec2(0.0f, 0.0f);
		glm::vec2 EndPoint = glm::vec2(0.0f, (GridSizeY * GridSpacingY));
		LineVertexList.emplace_back(LineVertex2D({ StartPoint.x + (x * GridSpacingX), StartPoint.y }, { glm::vec4(LineColor.x, LineColor.y, LineColor.z, 1.0f) }));
		LineVertexList.emplace_back(LineVertex2D({ EndPoint.x + (x * GridSpacingX), EndPoint.y }, { glm::vec4(LineColor.x, LineColor.y, LineColor.z, 1.0f) }));
		LineIndexList.emplace_back(indexCount++);
		LineIndexList.emplace_back(indexCount++);
	}
	for (int y = 0; y <= GridSizeY; y++)
	{
		glm::vec2 StartPoint = glm::vec2(0.0f, 0.0f);
		glm::vec2 EndPoint = glm::vec2((GridSizeX * GridSpacingX), 0.0f);
		LineVertexList.emplace_back(LineVertex2D({ StartPoint.x, StartPoint.y + (y * GridSpacingY) }, { glm::vec4(LineColor.x, LineColor.y, LineColor.z, 1.0f) }));
		LineVertexList.emplace_back(LineVertex2D({ EndPoint.x , EndPoint.y + (y * GridSpacingY) }, { glm::vec4(LineColor.x, LineColor.y, LineColor.z, 1.0f) }));
		LineIndexList.emplace_back(indexCount++);
		LineIndexList.emplace_back(indexCount++);
	}

	ParentModelID = -1;
	ParentGameObjectID = parentGameObjectID;
	VertexCount = LineVertexList.size();
	IndexCount = LineIndexList.size();
	TriangleCount = static_cast<uint32_t>(LineIndexList.size()) / 3;
	BoneCount = 0;

	MeshType = MeshTypeEnum::kLine;

	material = MaterialManager::GetDefaultMaterial();

	glm::mat4 MeshTransform = glm::mat4(1.0f);
	MeshTransform = glm::transpose(MeshTransform);


	VertexBuffer.CreateBuffer(LineVertexList.data(), LineVertexList.size() * sizeof(LineVertex2D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(LineIndexList.data(), LineIndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

}

LineMesh2D::LineMesh2D(int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, glm::vec4& LineColor, uint64_t parentGameObjectID)
{
	GenerateID();
	GenerateColorID();

	uint32_t indexCount = 0;
	for (int x = 0; x <= GridSizeX; x++)
	{
		glm::vec2 StartPoint = glm::vec2(0.0f, 0.0f);
		glm::vec2 EndPoint = glm::vec2(0.0f, (GridSizeY * GridSpacingY));
		LineVertexList.emplace_back(LineVertex2D({ StartPoint.x + (x * GridSpacingX), StartPoint.y }, { LineColor }));
		LineVertexList.emplace_back(LineVertex2D({ EndPoint.x + (x * GridSpacingX), EndPoint.y }, { LineColor }));
		LineIndexList.emplace_back(indexCount++);
		LineIndexList.emplace_back(indexCount++);
	}
	for (int y = 0; y <= GridSizeY; y++)
	{
		glm::vec2 StartPoint = glm::vec2(0.0f, 0.0f);
		glm::vec2 EndPoint = glm::vec2((GridSizeX * GridSpacingX), 0.0f);
		LineVertexList.emplace_back(LineVertex2D({ StartPoint.x, StartPoint.y + (y * GridSpacingY) }, { LineColor }));
		LineVertexList.emplace_back(LineVertex2D({ EndPoint.x , EndPoint.y + (y * GridSpacingY) }, { LineColor }));
		LineIndexList.emplace_back(indexCount++);
		LineIndexList.emplace_back(indexCount++);
	}

	ParentModelID = -1;
	ParentGameObjectID = parentGameObjectID;
	VertexCount = LineVertexList.size();
	IndexCount = LineIndexList.size();
	TriangleCount = static_cast<uint32_t>(LineIndexList.size()) / 3;
	BoneCount = 0;

	MeshType = MeshTypeEnum::kLine;

	material = MaterialManager::GetDefaultMaterial();

	glm::mat4 MeshTransform = glm::mat4(1.0f);
	MeshTransform = glm::transpose(MeshTransform);


	VertexBuffer.CreateBuffer(LineVertexList.data(), LineVertexList.size() * sizeof(LineVertex2D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(LineIndexList.data(), LineIndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

LineMesh2D::~LineMesh2D()
{
}

void LineMesh2D::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	VertexBuffer.CopyBufferToMemory(LineVertexList.data(), LineVertexList.size() * sizeof(LineVertex2D));
	IndexBuffer.CopyBufferToMemory(LineIndexList.data(), LineIndexList.size() * sizeof(uint32_t));

	Mesh2D::Update(GameObjectMatrix, ModelMatrix);
}
