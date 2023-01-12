#include "GLTF_Temp_Model.h"

GLTF_Temp_Model::GLTF_Temp_Model()
{
}

GLTF_Temp_Model::GLTF_Temp_Model(const std::string FilePath)
{
	GLTFModel model = GLTFModel(FilePath.c_str());
	GLTFModelData gltfModelData = model.GetModelData();

	VertexCount = gltfModelData.VertexList.size();
	IndexCount = gltfModelData.IndexList.size();

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
	TransformMatrix = gltfModelData.TransformMatrix;
	PrimitiveList = gltfModelData.GLTFPrimitiveList;
	TextureList = gltfModelData.TextureList;
	MaterialList = gltfModelData.MaterialList;

	VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(Vertex3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformBuffer.CreateBuffer(&TransformMatrix, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	MeshPropertiesUniformBuffer.Update(meshProperties);

	Update(glm::mat4(1.0f));
	UpdateMeshPropertiesBuffer();
	UpdateTexturePropertiesBuffer();
	UpdateMaterialPropertiesBuffer();
}

GLTF_Temp_Model::~GLTF_Temp_Model()
{
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

	TransformMatrix = glm::mat4(1.0f);
	TransformMatrix = glm::translate(TransformMatrix, ModelPosition);
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(ModelRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(ModelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(ModelRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	TransformMatrix = glm::scale(TransformMatrix, ModelScale);

	meshProperties.MeshTransform = GameObjectTransformMatrix * TransformMatrix;
	MeshPropertiesUniformBuffer.Update(meshProperties);
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
	for (int x = 0; x <= PrimitiveList.size() -3; x++)
	{
		if (PrimitiveList[x].IndexCount > 0)
		{
			vkCmdDrawIndexed(commandBuffer, PrimitiveList[x].IndexCount, 1, PrimitiveList[x].FirstIndex, 0, 0);
		}
		else
		{
			vkCmdDraw(commandBuffer, VertexCount, 1, 0, 0);
		}
	}
}

void GLTF_Temp_Model::Destroy()
{
}
