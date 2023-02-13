#include "GLTF_Temp_Model.h"
#include "SceneManager.h"

uint64_t GLTF_Temp_Model::ModelIDCounter = 0;

GLTF_Temp_Model::GLTF_Temp_Model()
{
}

GLTF_Temp_Model::GLTF_Temp_Model(const std::string FilePath, glm::mat4 GameObjectMatrix, uint32_t gameObjectID)
{
	GenerateID();

	ParentGameObjectID = gameObjectID;
	GameObjectTransformMatrix = glm::mat4(1.0f);

	GLTFModel model = GLTFModel(FilePath.c_str());
	GLTFModelData gltfModelData = model.GetModelData();

	std::vector<Vertex3D> VertexList;
	for (GLTFVertex gltfVertex : gltfModelData.VertexList)
	{
		Vertex3D vertex;
		vertex.Position = gltfVertex.Position;
		vertex.Normal = gltfVertex.Normal;
		vertex.UV = gltfVertex.UV;
		vertex.Tangant = gltfVertex.Tangant;
		vertex.BiTangant = gltfVertex.BiTangant;
		vertex.Color = gltfVertex.Color;
		VertexList.emplace_back(vertex);
	}
	std::vector<uint32_t> IndexList = gltfModelData.IndexList;

	TextureList = gltfModelData.TextureList;
	MaterialList = gltfModelData.MaterialList;

	VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(Vertex3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	
	glm::mat4 matrix = glm::mat4(1.0f);
	TransformMatrixList.emplace_back(matrix);

	MeshProperties meshProperties;
	MeshPropertiesList.emplace_back(meshProperties);

	VulkanBuffer TransformBuffer;
	TransformBuffer.CreateBuffer(&matrix, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	MeshTransformBufferList.emplace_back(TransformBuffer);

	VulkanBuffer MeshPropertiesBuffer;
	MeshPropertiesBuffer.CreateBuffer(&meshProperties, sizeof(MeshProperties), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	MeshPropertiesBufferList.emplace_back(MeshPropertiesBuffer);

	for (auto& node : gltfModelData.NodeList)
	{
		GLTFMeshLoader3D GltfMeshLoader;
		GltfMeshLoader.node = node;

		GltfMeshLoader.MeshType = MeshTypeEnum::kPolygon;
		GltfMeshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

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

		for (auto& childMesh : MeshList[0]->ChildMeshList)
		{
			glm::mat4 matrix = glm::mat4(1.0f);
			TransformMatrixList.emplace_back(matrix);

			MeshProperties meshProperties;
			MeshPropertiesList.emplace_back(meshProperties);

			VulkanBuffer TransformBuffer;
			TransformBuffer.CreateBuffer(&matrix, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			MeshTransformBufferList.emplace_back(TransformBuffer);

			VulkanBuffer MeshPropertiesBuffer;
			MeshPropertiesBuffer.CreateBuffer(&meshProperties, sizeof(MeshProperties), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			MeshPropertiesBufferList.emplace_back(MeshPropertiesBuffer);
		}
	}

	Update(glm::mat4(1.0f));
	UpdateMeshPropertiesBuffer();
	UpdateMeshTransformBuffer();
	UpdateDescriptorSets();
}

GLTF_Temp_Model::~GLTF_Temp_Model()
{
}

void GLTF_Temp_Model::GenerateID()
{
	ModelIDCounter++;
	ModelID = ModelIDCounter;
}

void GLTF_Temp_Model::UpdateNodeTransform(std::shared_ptr<Temp_GLTFMesh> mesh, std::shared_ptr<GLTFNode> node, const glm::mat4& ParentMatrix)
{
	glm::mat4 GlobalTransform = ParentMatrix * glm::mat4(1.0f);
	if (node == nullptr)
	{
		MeshTransformBufferList[0].CopyBufferToMemory(&GlobalTransform, sizeof(glm::mat4));
	}
	else
	{
		GlobalTransform = ParentMatrix * node->NodeTransformMatrix;
		MeshTransformBufferList[node->NodeID].CopyBufferToMemory(&GlobalTransform, sizeof(glm::mat4));
	}

	if (node == nullptr)
	{
		for (int x = 0; x < mesh->ChildMeshList.size() - 1; x++)
		{
			UpdateNodeTransform(mesh, mesh->ChildMeshList[x + 1], GlobalTransform);
		}
	}
}

void GLTF_Temp_Model::Update(const glm::mat4& GameObjectTransformMatrix)
{
	for (int x = 0; x < MeshList[0]->ChildMeshList.size(); x++)
	{
		MeshPropertiesBufferList[x].CopyBufferToMemory(&MeshPropertiesList[x], sizeof(MeshProperties));
	}

	ModelTransformMatrix = glm::mat4(1.0f);
	ModelTransformMatrix = glm::translate(ModelTransformMatrix, ModelPosition);
	ModelTransformMatrix = glm::rotate(ModelTransformMatrix, glm::radians(ModelRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	ModelTransformMatrix = glm::rotate(ModelTransformMatrix, glm::radians(ModelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	ModelTransformMatrix = glm::rotate(ModelTransformMatrix, glm::radians(ModelRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	ModelTransformMatrix = glm::scale(ModelTransformMatrix, ModelScale);

	for (auto& mesh : MeshList)
	{
		UpdateNodeTransform(mesh, nullptr, GameObjectTransformMatrix * ModelTransformMatrix);
		mesh->Update(GameObjectTransformMatrix, ModelTransformMatrix);
	}
}

void GLTF_Temp_Model::UpdateDescriptorSets()
{
	for (int x = 0; x < MaterialList.size(); x++)
	{
		std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
		GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, GetMeshPropertiesBuffer());
		GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 1, GetTransformMatrixBuffer());
		GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 2, MaterialList[x]->GetAlbedoMapDescriptor());
		GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 3, MaterialList[x]->GetNormalMapDescriptor());
		GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 4, MaterialList[x]->GetMetallicRoughnessMapDescriptor());
		GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 5, MaterialList[x]->GetAmbientOcclusionMapDescriptor());
		GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 6, MaterialList[x]->GetAlphaMapDescriptor());
		GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 7, MaterialList[x]->GetDepthMapDescriptor());
		GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 8, SceneManager::GetBRDFMapDescriptor());
		GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 9, SceneManager::GetIrradianceMapDescriptor());
		GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 10, SceneManager::GetPrefilterMapDescriptor());
		GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 11, LightManager::GetDirectionalLightBuffer());
		GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 12, LightManager::GetPointLightBuffer());
		GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 13, LightManager::GetSpotLightBuffer());
		descripterSetList.emplace_back(GLTF_GraphicsDescriptors::SubmitDescriptorSet(DescriptorBindingList));
	}
}

void GLTF_Temp_Model::UpdateMeshPropertiesBuffer()
{
	std::vector<VkDescriptorBufferInfo> MeshPropertiesDescriptorList;
	for (auto meshProperties : MeshPropertiesBufferList)
	{
		VkDescriptorBufferInfo MeshPropertiesmBufferInfo = {};
		MeshPropertiesmBufferInfo.buffer = meshProperties.GetBuffer();
		MeshPropertiesmBufferInfo.offset = 0;
		MeshPropertiesmBufferInfo.range = VK_WHOLE_SIZE;
		MeshPropertiesDescriptorList.emplace_back(MeshPropertiesmBufferInfo);
	}
	MeshPropertiesBuffer = MeshPropertiesDescriptorList;
}

void GLTF_Temp_Model::UpdateMeshTransformBuffer()
{
	std::vector<VkDescriptorBufferInfo> TransformDescriptorList;
	for (auto transformMatrix : MeshTransformBufferList)
	{
		VkDescriptorBufferInfo transformMatrixPropertiesBufferInfo = {};
		transformMatrixPropertiesBufferInfo.buffer = transformMatrix.GetBuffer();
		transformMatrixPropertiesBufferInfo.offset = 0;
		transformMatrixPropertiesBufferInfo.range = VK_WHOLE_SIZE;
		TransformDescriptorList.emplace_back(transformMatrixPropertiesBufferInfo);
	}
	TransformMatrixBuffer = TransformDescriptorList;
}

void GLTF_Temp_Model::Draw(VkCommandBuffer& commandBuffer, VkPipelineLayout ShaderPipelineLayout)
{
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &VertexBuffer.Buffer, offsets);
	vkCmdBindIndexBuffer(commandBuffer, IndexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
	for (auto& mesh : MeshList)
	{
		for (auto& node : mesh->ChildMeshList)
		{
			if (node->Visible)
			{
				for (auto& primitve : node->PrimitiveList)
				{
					if (primitve.IndexCount > 0)
					{
						SceneManager::sceneProperites.MeshIndex = node->NodeID;
						SceneManager::sceneProperites.MeshColorID = Converter::PixelToVec3(mesh->GetMeshColorID());

						vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipelineLayout, 0, 1, &descripterSetList[node->NodeID], 0, nullptr);
						vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SceneProperties), &SceneManager::sceneProperites);
						vkCmdDrawIndexed(commandBuffer, primitve.IndexCount, 1, primitve.FirstIndex, 0, 0);
					}
				}
			}
		}
	}
}

void GLTF_Temp_Model::Destroy()
{
}
