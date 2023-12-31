#include "SpriteLayerMesh.h"
#include "RandomNumber.h"
#include <glm/ext/matrix_transform.hpp>

SpriteLayerMesh::SpriteLayerMesh()
{
}

SpriteLayerMesh::SpriteLayerMesh(const std::string& LevelName, std::vector<Sprite>& levelTiles, std::vector<std::shared_ptr<Material>> materialList, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelObjectID)
{
	GLTFInstancingDataStruct instance = {};
	std::vector<std::shared_ptr<Material>> instanceMaterialList;

	SpriteList = levelTiles;
	for (auto& sprite : SpriteList)
	{
		GLTFInstanceMeshDataStruct instanceMeshDataStruct = {};
		instanceMeshDataStruct.VertexData = sprite.GetSpriteVertexList();
		instanceMeshDataStruct.IndiceData = sprite.GetSpriteIndexList();
		instanceMeshDataStruct.InstancePosition = glm::vec3(float(sprite.GetSpritePosition().x * 1.0f), float(sprite.GetSpritePosition().y * 1.0f), 0.0f);
		instanceMeshDataStruct.MaterialBufferIndex = sprite.GetMaterial()->GetMaterialBufferIndex();

		instance.InstanceMeshDataList.emplace_back(instanceMeshDataStruct);
		instance.UVOffset.emplace_back(glm::vec2(0.0f, 0.0f));
		instance.PaletteIndex = 0;
	}

	MeshID = 0;
	ParentModelID = modelObjectID;
	ParentGameObjectID = gameObjectID;

	MeshName = LevelName;
	gltfMaterialList = materialList;

	VertexCount = SpriteList[0].GetSpriteVertexList().size();
	IndexCount = SpriteList[0].GetSpriteIndexList().size();
	BoneCount = 0;
	TriangleCount = 0;
	InstanceCount = instance.InstanceMeshDataList.size();

	GameObjectTransform = GameObjectMatrix;
	ModelTransform = ModelMatrix;
	MeshTransform = glm::mat4(1.0f);

	MeshPosition = glm::vec3(0.0f);
	MeshRotation = glm::vec3(0.0f);
	MeshScale = glm::vec3(1.0f);

	MeshPropertiesBuffer = VulkanBuffer(&meshProperties, sizeof(MeshProperties), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT |VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	VertexBuffer = std::make_shared<VulkanBuffer>(VulkanBuffer(SpriteList[0].GetSpriteVertexList().data(), SpriteList[0].GetSpriteVertexList().size() * sizeof(Vertex2D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT |VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	IndexBuffer = std::make_shared<VulkanBuffer>(VulkanBuffer(SpriteList[0].GetSpriteIndexList().data(), SpriteList[0].GetSpriteIndexList().size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT |VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	MeshTransformBuffer = VulkanBuffer(&MeshTransform, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT |VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	Instancing2DStartUp(instance);

	glm::mat4 MeshMatrix = glm::mat4(1.0f);
	MeshMatrix = glm::translate(MeshMatrix, MeshPosition);
	MeshMatrix = glm::rotate(MeshMatrix, glm::radians(MeshRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	MeshMatrix = glm::rotate(MeshMatrix, glm::radians(MeshRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	MeshMatrix = glm::rotate(MeshMatrix, glm::radians(MeshRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	MeshMatrix = glm::scale(MeshMatrix, MeshScale);

	UpdateNodeTransform(nullptr, GameObjectTransform * ModelTransform * MeshMatrix);
}

SpriteLayerMesh::~SpriteLayerMesh()
{
}

void SpriteLayerMesh::AddSprite(Sprite sprite)
{
	SpriteList.emplace_back(sprite);
}

void SpriteLayerMesh::AddSprite(std::vector<Sprite> sprite)
{
	SpriteList = sprite;
}

void SpriteLayerMesh::Instancing2DStartUp(GLTFInstancingDataStruct& instanceData)
{
	InstanceCount = instanceData.InstanceMeshDataList.size();
	InstanceData = instanceData;
	if (InstanceCount > 0)
	{
		for (int x = 0; x < instanceData.InstanceMeshDataList.size(); x++)
		{
			InstancedVertexData2D instanceVertexData;

			glm::mat4 TransformMatrix = glm::mat4(1.0f);
			TransformMatrix = glm::translate(TransformMatrix, glm::vec3(instanceData.InstanceMeshDataList[x].InstancePosition.x, instanceData.InstanceMeshDataList[x].InstancePosition.y, 0.0f));
			TransformMatrix = glm::rotate(TransformMatrix, glm::radians(instanceData.InstanceMeshDataList[x].InstanceRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			TransformMatrix = glm::rotate(TransformMatrix, glm::radians(instanceData.InstanceMeshDataList[x].InstanceRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			TransformMatrix = glm::rotate(TransformMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			TransformMatrix = glm::scale(TransformMatrix, glm::vec3(instanceData.InstanceMeshDataList[x].InstanceScale.x, instanceData.InstanceMeshDataList[x].InstanceScale.y, 0.0f));

			instanceVertexData.InstanceModel = TransformMatrix;
			instanceVertexData.UVOffset = instanceData.UVOffset[x];
			instanceVertexData.MaterialID = instanceData.InstanceMeshDataList[x].MaterialBufferIndex;
			instanceVertexData.FlipSprite = glm::vec2(0.0f, 0.0f);
			InstancedVertex2DDataList.emplace_back(instanceVertexData);
		}

		InstanceBuffer.CreateBuffer(InstancedVertex2DDataList.data(), InstancedVertex2DDataList.size() * sizeof(InstancedVertexData2D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT |VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}
}

void SpriteLayerMesh::Update(float DeltaTime, const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	Mesh::Update(DeltaTime, GameObjectMatrix, ModelMatrix);

	if (InstanceCount > 0)
	{
		for (int x = 0; x < InstancedVertex2DDataList.size(); x++)
		{
			SpriteList[x].Update(DeltaTime);
			InstancedVertex2DDataList[x].UVOffset = SpriteList[x].GetCurrentSpriteUV();

			//if (InstancedVertex2DDataList[x].PixelOffset < 5)
			//{
			//	InstancedVertex2DDataList[x].PixelOffset += 1;
			//}
			//else
			//{
			//	InstancedVertex2DDataList[x].PixelOffset = 0;
			//}
		}
		InstanceBuffer.UpdateBufferMemory(InstancedVertex2DDataList.data(), InstancedVertex2DDataList.size() * sizeof(InstancedVertexData2D));
	}
}

void SpriteLayerMesh::DrawLevelLayer(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperites)
{
	Mesh::DrawLevelLayer(commandBuffer, descriptorSet, shaderPipelineLayout, sceneProperites);
}