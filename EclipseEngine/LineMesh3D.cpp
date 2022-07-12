#include "LineMesh3D.h"

LineMesh3D::LineMesh3D(std::vector<LineVertex3D>& vertices)
{
	GenerateID();
	GenerateColorID();

	IndexList = {

	};

	ParentModelID = -1;
	ParentGameObjectID = -1;
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

LineMesh3D::LineMesh3D(glm::vec3& StartPoint, glm::vec3& EndPoint)
{
	GenerateID();
	GenerateColorID();

	std::vector<LineVertex3D> VertexList2 = {
	{{StartPoint}, {1.0f, 0.0f, 0.0f}},
	{{EndPoint}, {0.0f, 1.0f, 0.0f}}
	};

	IndexList = {

	};

	ParentModelID = -1;
	ParentGameObjectID = -1;
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

LineMesh3D::~LineMesh3D()
{
}

void LineMesh3D::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	glm::mat4 TransformMatrix = glm::mat4(1.0f);
	TransformMatrix = glm::translate(TransformMatrix, MeshPosition);
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(MeshRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(MeshRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(MeshRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	TransformMatrix = glm::scale(TransformMatrix, glm::vec3(MeshScale.x, MeshScale.y, 1.0f));

	if (SelectedMesh)
	{
		meshProperties.SelectedObjectBufferIndex = 1;
	}
	else
	{
		meshProperties.SelectedObjectBufferIndex = 0;
	}


	meshProperties.MeshTransform = TransformMatrix;
	meshProperties.ModelTransform = ModelMatrix;
	meshProperties.GameObjectTransform = GameObjectMatrix;
	meshProperties.materialBufferData = material->GetMaterialTextureData();
	MeshPropertiesBuffer.Update(meshProperties);
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