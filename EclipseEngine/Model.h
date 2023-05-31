#pragma once
#include <GLTFModel.h>
#include "Vertex.h"
#include "Mesh.h"
#include "GLTF_GraphicsDescriptors.h"

class Model
{
private:
	static uint64_t ModelIDCounter;

	uint64_t ParentGameObjectID = 0;
	uint64_t ModelID = 0;

	VulkanBuffer VertexBuffer;
	VulkanBuffer IndexBuffer;
	VulkanBuffer scratchBuffer;
	AccelerationStructureBuffer TopLevelAccelerationStructure;

	int AccelerationStructureInstanceCount;
	VkDeviceOrHostAddressConstKHR DeviceOrHostAddressConst;

	std::vector<VulkanBuffer> MeshPropertiesBufferList;

	std::vector<VkDescriptorBufferInfo> VertexPropertiesBuffer;
	std::vector<VkDescriptorBufferInfo> IndexPropertiesBuffer;
	std::vector<VkDescriptorBufferInfo> MeshPropertiesBuffer;
	std::vector<VkDescriptorImageInfo> TexturePropertiesBuffer;
	std::vector<VkDescriptorBufferInfo> MaterialPropertiesBuffer;

	glm::mat4 GameObjectTransformMatrix;
	glm::mat4 ModelTransformMatrix;

	void GenerateID();
	void RTXModelStartUp();
	void LoadMaterials(std::vector<GLTFMaterialLoader>& materialLoader);
	void LoadLights(GLTFModelData& lights);

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
	std::vector<std::shared_ptr<Mesh>> MeshList;
	std::vector<std::shared_ptr<Material>> MaterialList;
	glm::vec3 ModelPosition = glm::vec3(0.0f);
	glm::vec3 ModelRotation = glm::vec3(0.0f);
	glm::vec3 ModelScale = glm::vec3(1.0f);

	Model();
	~Model();

	template <class T>
	void LoadModel(const std::string FilePath, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
	{
		GenerateID();

		ParentGameObjectID = gameObjectID;
		GameObjectTransformMatrix = GameObjectMatrix;
		//ModelTransformMatrix = gltfModelData.NodeList[0]->ModelTransformMatrix;

		GLTFModel model = GLTFModel(FilePath.c_str());
		GLTFModelData gltfModelData = model.GetModelData();

		std::vector<T> VertexList = GetVertexData<T>(gltfModelData.VertexList);
		std::vector<uint32_t> IndexList = gltfModelData.IndexList;

		LoadMaterials(gltfModelData.MaterialList);
		LoadLights(gltfModelData);

		VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(T), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		IndexBuffer.CreateBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		for (auto& node : gltfModelData.NodeList)
		{
			GLTFMeshLoader3D GltfMeshLoader;
			GltfMeshLoader.node = node;
			GltfMeshLoader.ParentGameObjectID = ParentGameObjectID;
			GltfMeshLoader.ParentModelID = ModelID;
			GltfMeshLoader.NodeID = node->NodeID;
			GltfMeshLoader.VertexBuffer = VertexBuffer;
			GltfMeshLoader.IndexBuffer = IndexBuffer;
			GltfMeshLoader.VertexCount = VertexList.size();
			GltfMeshLoader.IndexCount = IndexList.size();
			GltfMeshLoader.BoneCount = 0;
			GltfMeshLoader.GameObjectTransform = GameObjectMatrix;
			GltfMeshLoader.ModelTransform = node->ModelTransformMatrix;
			GltfMeshLoader.gltfMaterialList = MaterialList;

			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(GltfMeshLoader));
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
	void LoadModel(const std::string FilePath, GLTFInstancingDataStruct& instanceData, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
	{
		GenerateID();

		ParentGameObjectID = gameObjectID;
		GameObjectTransformMatrix = GameObjectMatrix;
	//	ModelTransformMatrix = gltfModelData.NodeList[0]->ModelTransformMatrix;

		GLTFModel model = GLTFModel(FilePath.c_str());
		GLTFModelData gltfModelData = model.GetModelData();

		std::vector<T> VertexList = GetVertexData<T>(gltfModelData.VertexList);
		std::vector<uint32_t> IndexList = gltfModelData.IndexList;

		LoadMaterials(gltfModelData.MaterialList);
		LoadLights(gltfModelData);

		VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(T), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		IndexBuffer.CreateBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		for (auto& node : gltfModelData.NodeList)
		{
			GLTFMeshLoader3D GltfMeshLoader;
			GltfMeshLoader.node = node;
			GltfMeshLoader.ParentGameObjectID = ParentGameObjectID;
			GltfMeshLoader.ParentModelID = ModelID;
			GltfMeshLoader.NodeID = node->NodeID;
			GltfMeshLoader.VertexBuffer = VertexBuffer;
			GltfMeshLoader.IndexBuffer = IndexBuffer;
			GltfMeshLoader.VertexCount = VertexList.size();
			GltfMeshLoader.IndexCount = IndexList.size();
			GltfMeshLoader.BoneCount = 0;
			GltfMeshLoader.GameObjectTransform = GameObjectMatrix;
			GltfMeshLoader.ModelTransform = node->ModelTransformMatrix;
			GltfMeshLoader.gltfMaterialList = MaterialList;
			GltfMeshLoader.InstanceData = instanceData;

			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(GltfMeshLoader));
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
		GameObjectTransformMatrix = GameObjectMatrix;
		//ModelTransformMatrix = gltfModelData.NodeList[0]->ModelTransformMatrix;

		MaterialList.emplace_back(std::make_shared<Material>(Material("Line Material")));

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

		GLTFMeshLoader3D GltfMeshLoader;
		GltfMeshLoader.node = node;
		GltfMeshLoader.ParentGameObjectID = ParentGameObjectID;
		GltfMeshLoader.ParentModelID = ModelID;
		GltfMeshLoader.NodeID = node->NodeID;
		GltfMeshLoader.VertexBuffer = VertexBuffer;
		GltfMeshLoader.VertexCount = VertexList.size();
		GltfMeshLoader.IndexCount = 0;
		GltfMeshLoader.BoneCount = 0;
		GltfMeshLoader.GameObjectTransform = GameObjectMatrix;
		GltfMeshLoader.gltfMaterialList = MaterialList;
		
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(GltfMeshLoader));
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
		GameObjectTransformMatrix = GameObjectMatrix;
		//ModelTransformMatrix = gltfModelData.NodeList[0]->ModelTransformMatrix;

		MaterialList.emplace_back(std::make_shared<Material>(Material("Line Material")));

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

		GLTFMeshLoader3D GltfMeshLoader;
		GltfMeshLoader.node = node;
		GltfMeshLoader.ParentGameObjectID = ParentGameObjectID;
		GltfMeshLoader.ParentModelID = ModelID;
		GltfMeshLoader.NodeID = node->NodeID;
		GltfMeshLoader.VertexBuffer = VertexBuffer;
		GltfMeshLoader.IndexBuffer = IndexBuffer;
		GltfMeshLoader.VertexCount = VertexList.size();
		GltfMeshLoader.IndexCount = IndexList.size();
		GltfMeshLoader.BoneCount = 0;
		GltfMeshLoader.GameObjectTransform = GameObjectMatrix;
		GltfMeshLoader.gltfMaterialList = MaterialList;

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(GltfMeshLoader));
		MeshList.emplace_back(mesh);

		Update(GameObjectMatrix);
		UpdateMeshPropertiesBuffer();
		for (auto& mesh : MeshList)
		{
			mesh->UpdateMeshTransformBuffer();
		}
	}

	template <class T>
	void LoadModel(const std::string& NodeName, const std::vector<T>& vertexList, const std::vector<uint32_t>& indexList, const std::shared_ptr<Material> material, glm::mat4& GameObjectMatrix, uint32_t gameObjectID)
	{
		GenerateID();

		ParentGameObjectID = gameObjectID;
		GameObjectTransformMatrix = GameObjectMatrix;
	//	ModelTransformMatrix = gltfModelData.NodeList[0]->ModelTransformMatrix;

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

		GLTFMeshLoader3D GltfMeshLoader;
		GltfMeshLoader.node = node;
		GltfMeshLoader.ParentGameObjectID = ParentGameObjectID;
		GltfMeshLoader.ParentModelID = ModelID;
		GltfMeshLoader.NodeID = node->NodeID;
		GltfMeshLoader.VertexBuffer = VertexBuffer;
		GltfMeshLoader.IndexBuffer = IndexBuffer;
		GltfMeshLoader.VertexCount = VertexList.size();
		GltfMeshLoader.IndexCount = IndexList.size();
		GltfMeshLoader.BoneCount = 0;
		GltfMeshLoader.GameObjectTransform = GameObjectMatrix;
		GltfMeshLoader.gltfMaterialList = MaterialList;

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(GltfMeshLoader));
		MeshList.emplace_back(mesh);

		Update(GameObjectMatrix);
		UpdateMeshPropertiesBuffer();
		for (auto& mesh : MeshList)
		{
			mesh->UpdateMeshTransformBuffer();
		}
	}

	void LoadSpriteMesh2D(const std::string& spriteName, std::shared_ptr<Material> material, glm::mat4& GameObjectMatrix, uint32_t gameObjectID);
	void LoadSpriteMesh3D(const std::string& spriteName, std::shared_ptr<Material> material, glm::mat4& GameObjectMatrix, uint32_t gameObjectID);

	void LoadLineMesh3D(const std::string& LineName, std::vector<LineVertex3D>& vertices, glm::mat4& GameObjectMatrix, uint32_t gameObjectID);
	void LoadLineMesh3D(const std::string& LineName, const glm::vec3& StartPoint, const glm::vec3& EndPoint, glm::mat4& GameObjectMatrix, uint32_t gameObjectID);
	void LoadLineMesh3D(const std::string& LineName, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec3& Color, glm::mat4& GameObjectMatrix, uint32_t gameObjectID);
	void LoadLineMesh3D(const std::string& LineName, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& Color, glm::mat4& GameObjectMatrix, uint32_t gameObjectID);
	void LoadLineMesh3D(const std::string& LineName, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& StartColor, const glm::vec4& EndColor, glm::mat4& GameObjectMatrix, uint32_t gameObjectID);
	void LoadLineMesh3D(const std::string& GridName, int GridSizeX, int GridSizeY, int GridSizeZ, float GridSpacingX, float GridSpacingY, float GridSpacingZ, glm::mat4& GameObjectMatrix, uint32_t gameObjectID);

	void Update(const glm::mat4& GameObjectTransformMatrix);
	void UpdateModelTopLevelAccelerationStructure(std::vector<VkAccelerationStructureInstanceKHR>& AccelerationStructureInstanceList, uint32_t customIndex);
	void UpdateMeshPropertiesBuffer();
	void Draw(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout ShaderPipelineLayout);
	void DrawMesh(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperties);
	void DrawReflectionMesh(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperties);
	void DrawInstancedMesh(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperties);
	void DrawSprite(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperties);
	void DrawLine(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorset, VkPipelineLayout shaderPipelineLayout, SceneProperties& sceneProperties);
	void Destroy();

	VkDescriptorBufferInfo UpdateVertexBuffer();
	VkDescriptorBufferInfo UpdateIndexBuffer();

	std::vector<std::shared_ptr<Mesh>> GetMeshList() { return MeshList; }
	std::vector<std::shared_ptr<Material>> GetMaterialList() { return MaterialList; }
	std::vector<VkDescriptorBufferInfo> GetMeshPropertiesBuffer() { return MeshPropertiesBuffer; }
	std::vector<VkDescriptorBufferInfo> GetTransformMatrixBuffer() { return MeshList[0]->TransformMatrixBuffer; }
	std::vector<VkDescriptorImageInfo> GetTexturePropertiesBuffer() { return TexturePropertiesBuffer; }
	std::vector<VkDescriptorBufferInfo> GetMaterialPropertiesBuffer() { return MaterialPropertiesBuffer; }
};
