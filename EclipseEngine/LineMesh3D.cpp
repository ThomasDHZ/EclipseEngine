#include "LineMesh3D.h"

LineMesh3D::LineMesh3D(std::vector<LineVertex3D>& vertices)
{
	LineVertexList = vertices;
	for (int x = 0; x <= LineVertexList.size(); x++)
	{
		LineIndexList.emplace_back(x);
	}

	SetUpMesh();
}

LineMesh3D::LineMesh3D(const glm::vec3& StartPoint, const glm::vec3& EndPoint)
{
	LineVertexList =
	{
		{StartPoint, glm::vec4(1.0f)},
		{EndPoint, glm::vec4(1.0f)}
	};
	LineIndexList = {
		0, 1
	};

	SetUpMesh();
}

LineMesh3D::LineMesh3D(const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec3& Color)
{
	LineVertexList =
	{
		{StartPoint, glm::vec4(Color.x, Color.y, Color.z, 1.0f)},
		{EndPoint, glm::vec4(Color.x, Color.y, Color.z, 1.0f)}
	};
	LineIndexList = {
		0, 1
	};

	SetUpMesh();
}

LineMesh3D::LineMesh3D(const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& Color)
{
	LineVertexList =
	{
		{StartPoint, Color},
		{EndPoint, Color}
	};
	LineIndexList = {
		0, 1
	};

	SetUpMesh();
}

LineMesh3D::LineMesh3D(int GridSizeX, int GridSizeY, int GridSizeZ, float GridSpacingX, float GridSpacingY, float GridSpacingZ)
{

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

	SetUpMesh();
}

LineMesh3D::~LineMesh3D()
{
}

void LineMesh3D::SetUpMesh()
{
	GenerateID();
	GenerateColorID();

	MeshType = MeshTypeEnum::kMeshLine;
	//material = MaterialManager::GetDefaultMaterial();
	ParentModelID = -1;
	VertexCount = LineVertexList.size();
	IndexCount = LineIndexList.size();
	TriangleCount = 0;
	BoneCount = 0;

	VertexBuffer.CreateBuffer(LineVertexList.data(), LineVertexList.size() * sizeof(LineVertex3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(LineIndexList.data(), LineIndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	MeshRendererManager::AddMesh(std::make_shared<LineMesh3D>(*this));
}

void LineMesh3D::BuildGrid3D(glm::vec3 GridSize, glm::vec3 GridSpacing)
{
}

void LineMesh3D::Draw(VkCommandBuffer& commandBuffer)
{
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, VertexBuffer.GetBufferPtr(), offsets);
	if (IndexCount == 0)
	{
		vkCmdDraw(commandBuffer, VertexCount, 1, 0, 0);
	}
	else
	{
		vkCmdBindIndexBuffer(commandBuffer, IndexBuffer.GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(commandBuffer, IndexCount, 1, 0, 0, 0);
	}
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

void LineMesh3D::SetMeshPosition(const glm::vec3& position)
{
	MeshPosition = position;
}


void LineMesh3D::SetMeshRotation(const glm::vec3& rotation)
{
	MeshRotation = rotation;
}

void LineMesh3D::SetMeshScale(const glm::vec3& scale)
{
	MeshScale = scale;
}