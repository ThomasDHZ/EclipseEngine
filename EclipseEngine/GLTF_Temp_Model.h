#pragma once
#include <GLTFModel.h>
#include "Vertex.h"
#include "Mesh3D.h"
#include "Temp_GLTFMesh.h"
#include "GLTF_GraphicsDescriptors.h"

class GLTF_Temp_Model
{
private:
	static uint64_t ModelIDCounter;

	uint64_t ParentGameObjectID = 0;
	uint64_t ModelID = 0;

	std::string NodeName;

	std::vector<GLTFPrimitive> PrimitiveList;
	std::vector<MeshProperties> MeshPropertiesList;
	std::vector<std::shared_ptr<Texture2D>> TextureList;

	VulkanBuffer VertexBuffer;
	VulkanBuffer IndexBuffer;
	std::vector<VulkanBuffer> MeshPropertiesBufferList;

	std::vector<VkDescriptorBufferInfo> MeshPropertiesBuffer;
	std::vector<VkDescriptorImageInfo> TexturePropertiesBuffer;
	std::vector<VkDescriptorBufferInfo> MaterialPropertiesBuffer;

	glm::mat4 GameObjectTransformMatrix;
	glm::mat4 ModelTransformMatrix;

	void GenerateID();

	template <class T>
	std::vector<T> GetVertexData(std::vector<GLTFVertex> vertexList)
	{
		std::vector<T> VertexList;

		if constexpr (std::is_same_v<T, Vertex3D>)
		{
			for (GLTFVertex gltfVertex : vertexList)
			{
				Vertex3D vertex{};
				vertex.Position = gltfVertex.Position;
				vertex.Normal = gltfVertex.Normal;
				vertex.UV = gltfVertex.UV;
				vertex.Tangant = gltfVertex.Tangant;
				vertex.BiTangant = gltfVertex.BiTangant;
				vertex.Color = gltfVertex.Color;
				VertexList.emplace_back(vertex);
			}
		}
		if constexpr (std::is_same_v<T, Vertex2D>)
		{
			for (GLTFVertex gltfVertex : vertexList)
			{
				Vertex2D vertex{};
				vertex.Position = gltfVertex.Position;
				vertex.UV = gltfVertex.UV;
				vertex.Color = gltfVertex.Color;
				VertexList.emplace_back(vertex);
			}
		}
		if constexpr (std::is_same_v<T, LineVertex3D>)
		{
			for (GLTFVertex gltfVertex : vertexList)
			{
				LineVertex3D vertex{};
				vertex.pos = gltfVertex.Position;
				vertex.color = gltfVertex.Color;
				VertexList.emplace_back(vertex);
			}
		}
		if constexpr (std::is_same_v<T, LineVertex2D>)
		{
			for (GLTFVertex gltfVertex : vertexList)
			{
				LineVertex2D vertex{};
				vertex.pos = gltfVertex.Position;
				vertex.color = gltfVertex.Color;
				VertexList.emplace_back(vertex);
			}
		}
		return VertexList;
	}

public:
	std::vector<std::shared_ptr<Temp_GLTFMesh>> MeshList;
	std::vector<std::shared_ptr<GLTFMaterial>> MaterialList;
	glm::vec3 ModelPosition = glm::vec3(0.0f);
	glm::vec3 ModelRotation = glm::vec3(0.0f);
	glm::vec3 ModelScale = glm::vec3(1.0f);

	GLTF_Temp_Model();
	~GLTF_Temp_Model();

	template <class T>
	void LoadModel(const std::string FilePath, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
	{
		GenerateID();

		ParentGameObjectID = gameObjectID;
		GameObjectTransformMatrix = glm::mat4(1.0f);

		GLTFModel model = GLTFModel(FilePath.c_str());
		GLTFModelData gltfModelData = model.GetModelData();

		std::vector<T> VertexList = GetVertexData<T>(gltfModelData.VertexList);
		std::vector<uint32_t> IndexList = gltfModelData.IndexList;

		TextureList = gltfModelData.TextureList;
		MaterialList = gltfModelData.MaterialList;

		VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(T), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		IndexBuffer.CreateBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		for (auto& node : gltfModelData.NodeList)
		{
			GLTFMeshLoader3D GltfMeshLoader;
			GltfMeshLoader.node = node;

			GltfMeshLoader.ParentGameObjectID = ParentGameObjectID;
			GltfMeshLoader.ParentModelID = ModelID;
			GltfMeshLoader.NodeID = node->NodeID;

			GltfMeshLoader.VertexCount = VertexList.size();
			GltfMeshLoader.IndexCount = IndexList.size();
			GltfMeshLoader.BoneCount = 0;

			GltfMeshLoader.GameObjectTransform = GameObjectMatrix;
			GltfMeshLoader.ModelTransform = node->ModelTransformMatrix;

			std::shared_ptr<Temp_GLTFMesh> mesh = std::make_shared<Temp_GLTFMesh>(Temp_GLTFMesh(GltfMeshLoader));
			MeshList.emplace_back(mesh);
		}

		Update(GameObjectMatrix);
		UpdateMeshPropertiesBuffer();
		for (auto& mesh : MeshList)
		{
			mesh->UpdateMeshTransformBuffer();
		}
	}

	template <class T>
	void LoadModel(const std::string& NodeName, const std::vector<T>& vertexList, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
	{
		GenerateID();

		ParentGameObjectID = gameObjectID;
		GameObjectTransformMatrix = glm::mat4(1.0f);

		MaterialList.emplace_back(std::make_shared<GLTFMaterial>(GLTFMaterial("Line Material")));

		std::vector<T> VertexList = vertexList;
		VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(T), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		
		std::shared_ptr<GLTFNode> node = std::make_shared<GLTFNode>();
		node->NodeID = 0;
		node->ParentModelID = ModelID;
		node->ParentGameObjectID = gameObjectID;
		node->NodeName = NodeName;
		node->ModelTransformMatrix = ModelTransformMatrix;
		node->NodeTransformMatrix = glm::mat4(1.0f);
		node->Position = glm::vec3(0.0f);
		node->Rotation = glm::vec3(0.0f);
		node->Scale = glm::vec3(1.0f);
		node->Material = MaterialList[0];

		GLTFMeshLoader3D GltfMeshLoader;
		GltfMeshLoader.node = node;
		GltfMeshLoader.ParentGameObjectID = ParentGameObjectID;
		GltfMeshLoader.ParentModelID = ModelID;
		GltfMeshLoader.NodeID = node->NodeID;
		GltfMeshLoader.VertexCount = VertexList.size();
		GltfMeshLoader.IndexCount = 0;
		GltfMeshLoader.BoneCount = 0;
		GltfMeshLoader.GameObjectTransform = GameObjectMatrix;
		
		std::shared_ptr<Temp_GLTFMesh> mesh = std::make_shared<Temp_GLTFMesh>(Temp_GLTFMesh(GltfMeshLoader));
		MeshList.emplace_back(mesh);

		Update(GameObjectMatrix);
		UpdateMeshPropertiesBuffer();
		for (auto& mesh : MeshList)
		{
			mesh->UpdateMeshTransformBuffer();
		}
	}

	template <class T>
	void LoadModel(const std::string& NodeName, const std::vector<T>& vertexList, const std::vector<uint32_t>& indexList, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
	{
		GenerateID();

		ParentGameObjectID = gameObjectID;
		GameObjectTransformMatrix = glm::mat4(1.0f);

		MaterialList.emplace_back(std::make_shared<GLTFMaterial>(GLTFMaterial("Line Material")));

		std::vector<T> VertexList = vertexList;
		std::vector<uint32_t> IndexList = indexList;
		VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(T), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		IndexBuffer.CreateBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		std::shared_ptr<GLTFNode> node = std::make_shared<GLTFNode>();
		node->NodeID = 0;
		node->ParentModelID = ModelID;
		node->ParentGameObjectID = gameObjectID;
		node->NodeName = NodeName;
		node->ModelTransformMatrix = ModelTransformMatrix;
		node->NodeTransformMatrix = glm::mat4(1.0f);
		node->Position = glm::vec3(0.0f);
		node->Rotation = glm::vec3(0.0f);
		node->Scale = glm::vec3(1.0f);
		node->Material = MaterialList[0];

		GLTFMeshLoader3D GltfMeshLoader;
		GltfMeshLoader.node = node;
		GltfMeshLoader.ParentGameObjectID = ParentGameObjectID;
		GltfMeshLoader.ParentModelID = ModelID;
		GltfMeshLoader.NodeID = node->NodeID;
		GltfMeshLoader.VertexCount = VertexList.size();
		GltfMeshLoader.IndexCount = IndexList.size();
		GltfMeshLoader.BoneCount = 0;
		GltfMeshLoader.GameObjectTransform = GameObjectMatrix;

		std::shared_ptr<Temp_GLTFMesh> mesh = std::make_shared<Temp_GLTFMesh>(Temp_GLTFMesh(GltfMeshLoader));
		MeshList.emplace_back(mesh);

		Update(GameObjectMatrix);
		UpdateMeshPropertiesBuffer();
		for (auto& mesh : MeshList)
		{
			mesh->UpdateMeshTransformBuffer();
		}
	}

	template <class T>
	void LoadModel(const std::string& NodeName, const std::vector<T>& vertexList, const std::vector<uint32_t>& indexList, const std::shared_ptr<GLTFMaterial> material, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
	{
		GenerateID();

		ParentGameObjectID = gameObjectID;
		GameObjectTransformMatrix = glm::mat4(1.0f);

		MaterialList.emplace_back(material);

		std::vector<T> VertexList = vertexList;
		std::vector<uint32_t> IndexList = indexList;
		VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(T), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		IndexBuffer.CreateBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		std::shared_ptr<GLTFNode> node = std::make_shared<GLTFNode>();
		node->NodeID = 0;
		node->ParentModelID = ModelID;
		node->ParentGameObjectID = gameObjectID;
		node->NodeName = NodeName;
		node->ModelTransformMatrix = ModelTransformMatrix;
		node->NodeTransformMatrix = glm::mat4(1.0f);
		node->Position = glm::vec3(0.0f);
		node->Rotation = glm::vec3(0.0f);
		node->Scale = glm::vec3(1.0f);
		node->Material = MaterialList[0];

		GLTFMeshLoader3D GltfMeshLoader;
		GltfMeshLoader.node = node;
		GltfMeshLoader.ParentGameObjectID = ParentGameObjectID;
		GltfMeshLoader.ParentModelID = ModelID;
		GltfMeshLoader.NodeID = node->NodeID;
		GltfMeshLoader.VertexCount = VertexList.size();
		GltfMeshLoader.IndexCount = IndexList.size();
		GltfMeshLoader.BoneCount = 0;
		GltfMeshLoader.GameObjectTransform = GameObjectMatrix;

		std::shared_ptr<Temp_GLTFMesh> mesh = std::make_shared<Temp_GLTFMesh>(Temp_GLTFMesh(GltfMeshLoader));
		MeshList.emplace_back(mesh);

		Update(GameObjectMatrix);
		UpdateMeshPropertiesBuffer();
		for (auto& mesh : MeshList)
		{
			mesh->UpdateMeshTransformBuffer();
		}
	}

	void Update(const glm::mat4& GameObjectTransformMatrix);
	void UpdateMeshPropertiesBuffer();
	void Draw(VkCommandBuffer& commandBuffer, VkPipelineLayout ShaderPipelineLayout);
	void DrawSprite(VkCommandBuffer& commandBuffer, VkPipelineLayout shaderPipelineLayout);
	void DrawLine(VkCommandBuffer& commandBuffer, VkPipelineLayout ShaderPipelineLayout, VkDescriptorSet descriptorSet);
	void Destroy();

	std::vector<std::shared_ptr<Temp_GLTFMesh>> GetMeshList() { return MeshList; }
	std::vector<std::shared_ptr<GLTFMaterial>> GetMaterialList() { return MaterialList; }
	std::vector<VkDescriptorBufferInfo> GetMeshPropertiesBuffer() { return MeshPropertiesBuffer; }
	std::vector<VkDescriptorBufferInfo> GetTransformMatrixBuffer() { return MeshList[0]->TransformMatrixBuffer; }
	std::vector<VkDescriptorImageInfo> GetTexturePropertiesBuffer() { return TexturePropertiesBuffer; }
	std::vector<VkDescriptorBufferInfo> GetMaterialPropertiesBuffer() { return MaterialPropertiesBuffer; }
};
