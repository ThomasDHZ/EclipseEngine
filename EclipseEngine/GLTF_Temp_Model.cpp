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

	for (auto& node : gltfModelData.NodeList)
	{
		VulkanBuffer TransformBuffer;
		TransformBuffer.CreateBuffer(&TransformBuffer, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

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
}

	MeshPropertiesUniformBuffer.Update(meshProperties);

	Update(glm::mat4(1.0f));
	UpdateMeshPropertiesBuffer();
	UpdateTexturePropertiesBuffer();
	UpdateMaterialPropertiesBuffer();
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
	glm::mat4 MeshTransformMatrix = glm::mat4(1.0f);
	MeshTransformMatrix = glm::translate(MeshTransformMatrix, mesh->MeshPosition);
	MeshTransformMatrix = glm::rotate(MeshTransformMatrix, glm::radians(mesh->MeshRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	MeshTransformMatrix = glm::rotate(MeshTransformMatrix, glm::radians(mesh->MeshRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	MeshTransformMatrix = glm::rotate(MeshTransformMatrix, glm::radians(mesh->MeshRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	MeshTransformMatrix = glm::scale(MeshTransformMatrix, mesh->MeshScale);
	mesh->MeshTransform = MeshTransformMatrix;

	glm::mat4 GlobalTransform = ParentMatrix * mesh->MeshTransform;
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
	for (int x = 0; x < TextureList.size(); x++)
	{
		TextureList[x]->SetTextureBufferIndex(x);
	}
	for (auto& material : MaterialList)
	{
		material->MaterialBufferUpdate();
	}

	ModelTransformMatrix = glm::mat4(1.0f);
	ModelTransformMatrix = glm::translate(ModelTransformMatrix, ModelPosition);
	ModelTransformMatrix = glm::rotate(ModelTransformMatrix, glm::radians(ModelRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	ModelTransformMatrix = glm::rotate(ModelTransformMatrix, glm::radians(ModelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	ModelTransformMatrix = glm::rotate(ModelTransformMatrix, glm::radians(ModelRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	ModelTransformMatrix = glm::scale(ModelTransformMatrix, ModelScale);

	for (auto& mesh : MeshList)
	{
		UpdateNodeTransform(mesh, nullptr, ModelTransformMatrix);
		mesh->Update(GameObjectTransformMatrix, ModelTransformMatrix);
	}
}

void GLTF_Temp_Model::UpdateMeshPropertiesBuffer()
{
	VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
	MeshPropertiesmBufferBufferInfo.buffer = MeshPropertiesUniformBuffer.VulkanBufferData.Buffer;
	MeshPropertiesmBufferBufferInfo.offset = 0;
	MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
	MeshPropertiesBuffer = MeshPropertiesmBufferBufferInfo;
}

void GLTF_Temp_Model::UpdateTexturePropertiesBuffer()
{
	std::vector<VkDescriptorImageInfo> TextureDescriptorList;
	if (TextureList.size() == 0)
	{
		VkDescriptorImageInfo nullBuffer;
		nullBuffer.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		nullBuffer.imageView = VK_NULL_HANDLE;
		nullBuffer.sampler = VK_NULL_HANDLE;
		TextureDescriptorList.emplace_back(nullBuffer);
	}
	else
	{
		for (auto texture : TextureList)
		{
			VkDescriptorImageInfo TextureDescriptor{};
			TextureDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			TextureDescriptor.imageView = texture->GetView();
			TextureDescriptor.sampler = texture->GetSampler();
			TextureDescriptorList.emplace_back(TextureDescriptor);
		}
	}
	TexturePropertiesBuffer = TextureDescriptorList;
}

void GLTF_Temp_Model::UpdateMaterialPropertiesBuffer()
{
	std::vector<VkDescriptorBufferInfo> MaterialPropertiesBufferList{};
	for (int x = 0; x < MaterialList.size(); x++)
	{
		VkDescriptorBufferInfo materialPropertiesBufferBufferInfo = {};
		materialPropertiesBufferBufferInfo.buffer = MaterialList[x]->GetMaterialBuffer();
		materialPropertiesBufferBufferInfo.offset = 0;
		materialPropertiesBufferBufferInfo.range = VK_WHOLE_SIZE;
		MaterialPropertiesBufferList.emplace_back(materialPropertiesBufferBufferInfo);

		MaterialList[x]->SetBufferIndex(x);
	}

	MaterialPropertiesBuffer = MaterialPropertiesBufferList;
}

void GLTF_Temp_Model::Draw(VkCommandBuffer& commandBuffer)
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
