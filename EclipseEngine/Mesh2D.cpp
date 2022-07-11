#include "Mesh2D.h"
#include "Math.h"

Mesh2D::Mesh2D() : Mesh(MeshTypeEnum::kSprite, 0)
{
}

Mesh2D::Mesh2D(uint64_t parentGameObjectID) : Mesh(MeshTypeEnum::kSprite, parentGameObjectID)
{
	GenerateID();
	GenerateColorID();

	VertexList = SpriteVertexList;
	IndexList = SpriteIndexList;

	ParentModelID = -1;
	ParentGameObjectID = -1;
	VertexCount = VertexList.size();
	IndexCount = IndexList.size();

	material = MaterialManager::GetDefaultMaterial();

	MeshTransformMatrix = glm::mat4(1.0f);
	MeshTransformMatrix = glm::transpose(MeshTransformMatrix);

	VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(MeshVertex), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

Mesh2D::Mesh2D(std::shared_ptr<Material> materialPtr, uint64_t parentGameObjectID) : Mesh(MeshTypeEnum::kSprite, parentGameObjectID)
{
	GenerateID();
	GenerateColorID();

	VertexList = SpriteVertexList;
	IndexList = SpriteIndexList;

	ParentModelID = -1;
	ParentGameObjectID = -1;
	VertexCount = VertexList.size();
	IndexCount = IndexList.size();

	material = materialPtr;

	MeshTransformMatrix = glm::mat4(1.0f);
	MeshTransformMatrix = glm::transpose(MeshTransformMatrix);

	VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(MeshVertex), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

Mesh2D::~Mesh2D()
{
}

void Mesh2D::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	glm::mat4 TransformMatrix = glm::mat4(1.0f);
	TransformMatrix = glm::translate(GameObjectMatrix, glm::vec3(MeshPosition.x, MeshPosition.y, 0.0f));
	TransformMatrix = glm::rotate(GameObjectMatrix, glm::radians(MeshRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	TransformMatrix = glm::rotate(GameObjectMatrix, glm::radians(MeshRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	TransformMatrix = glm::scale(GameObjectMatrix, glm::vec3(MeshScale.x, MeshScale.y, 1.0f));

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

void Mesh2D::SetPosition(float x, float y)
{
	MeshPosition = glm::vec2(x, y);
}

void Mesh2D::SetPosition(glm::vec2 position)
{
	MeshPosition = position;
}

void Mesh2D::SetDepth(float Depth)
{
	MeshDepth = Depth;
}

void Mesh2D::SetRotation(float x, float y)
{
	MeshRotation = glm::vec2(x, y);
}

void Mesh2D::SetRotation(glm::vec2 rotation)
{
	MeshRotation = rotation;
}

void Mesh2D::SetScale(float x, float y)
{
	MeshScale = glm::vec2(x, y);
}

void Mesh2D::SetScale(glm::vec2 Scale)
{
	MeshScale = Scale;
}
