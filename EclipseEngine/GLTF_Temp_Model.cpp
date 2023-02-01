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
	UpdateTexturePropertiesBuffer();
	UpdateMaterialPropertiesBuffer();

	VkSampler Sampler = nullptr;
	VkSamplerCreateInfo TextureImageSamplerInfo = {};
	TextureImageSamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	TextureImageSamplerInfo.magFilter = VK_FILTER_NEAREST;
	TextureImageSamplerInfo.minFilter = VK_FILTER_NEAREST;
	TextureImageSamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	TextureImageSamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	TextureImageSamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	TextureImageSamplerInfo.anisotropyEnable = VK_TRUE;
	TextureImageSamplerInfo.maxAnisotropy = 16.0f;
	TextureImageSamplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	TextureImageSamplerInfo.unnormalizedCoordinates = VK_FALSE;
	TextureImageSamplerInfo.compareEnable = VK_FALSE;
	TextureImageSamplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	TextureImageSamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	TextureImageSamplerInfo.minLod = 0;
	TextureImageSamplerInfo.maxLod = 0;
	TextureImageSamplerInfo.mipLodBias = 0;

	if (vkCreateSampler(VulkanRenderer::GetDevice(), &TextureImageSamplerInfo, nullptr, &Sampler))
	{
		throw std::runtime_error("Failed to create Sampler.");
	}

	for (int x = 0; x < MaterialList.size(); x++)
	{
		VkDescriptorImageInfo albedoTextureDescriptor{};
		if (MaterialList[x]->AlbedoMap != nullptr)
		{
			auto albedoTexture = MaterialList[x]->AlbedoMap;
			albedoTextureDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			albedoTextureDescriptor.imageView = albedoTexture->GetView();
			albedoTextureDescriptor.sampler = albedoTexture->GetSampler();
		}
		else
		{
			albedoTextureDescriptor.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			albedoTextureDescriptor.imageView = VK_NULL_HANDLE;
			albedoTextureDescriptor.sampler = Sampler;
		}

		VkDescriptorImageInfo normalTextureDescriptor{};
		if (MaterialList[x]->NormalMap != nullptr)
		{
			auto normalTexture = MaterialList[x]->NormalMap;
			normalTextureDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			normalTextureDescriptor.imageView = normalTexture->GetView();
			normalTextureDescriptor.sampler = normalTexture->GetSampler();
		}
		else
		{
			normalTextureDescriptor.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			normalTextureDescriptor.imageView = VK_NULL_HANDLE;
			normalTextureDescriptor.sampler = Sampler;
		}

		VkDescriptorImageInfo MetallicRoughnessTextureDescriptor{};
		if (MaterialList[x]->MetallicRoughnessMap != nullptr)
		{
			auto MetallicRoughnessTexture = MaterialList[x]->MetallicRoughnessMap;
			MetallicRoughnessTextureDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			MetallicRoughnessTextureDescriptor.imageView = MetallicRoughnessTexture->GetView();
			MetallicRoughnessTextureDescriptor.sampler = MetallicRoughnessTexture->GetSampler();
		}
		else
		{
			MetallicRoughnessTextureDescriptor.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			MetallicRoughnessTextureDescriptor.imageView = VK_NULL_HANDLE;
			MetallicRoughnessTextureDescriptor.sampler = Sampler;
		}

		VkDescriptorImageInfo ambientOcclusionTextureDescriptor{};
		if (MaterialList[x]->AmbientOcclusionMap != nullptr)
		{
			auto AmbientOcclusionTexture = MaterialList[x]->AmbientOcclusionMap;
			ambientOcclusionTextureDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			ambientOcclusionTextureDescriptor.imageView = AmbientOcclusionTexture->GetView();
			ambientOcclusionTextureDescriptor.sampler = AmbientOcclusionTexture->GetSampler();
		}
		else
		{
			ambientOcclusionTextureDescriptor.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			ambientOcclusionTextureDescriptor.imageView = VK_NULL_HANDLE;
			ambientOcclusionTextureDescriptor.sampler = Sampler;
		}

		VkDescriptorImageInfo alphaTextureDescriptor{};
		if (MaterialList[x]->AlphaMap != nullptr)
		{
			auto AlphaTexture = MaterialList[x]->AlphaMap;

			alphaTextureDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			alphaTextureDescriptor.imageView = AlphaTexture->GetView();
			alphaTextureDescriptor.sampler = AlphaTexture->GetSampler();
		}
		else
		{
			alphaTextureDescriptor.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			alphaTextureDescriptor.imageView = VK_NULL_HANDLE;
			alphaTextureDescriptor.sampler = Sampler;
		}

		std::vector<VkDescriptorBufferInfo> MeshPropertiesBufferList = GetMeshPropertiesBuffer();
		std::vector<VkDescriptorBufferInfo> MeshTransformBufferList = GetTransformMatrixBuffer();
		std::vector<VkDescriptorBufferInfo> MaterialBufferList = GetMaterialPropertiesBuffer();
		std::vector<VkDescriptorBufferInfo> DirectionalLightBufferInfoList = LightManager::GetDirectionalLightBuffer();
		std::vector<VkDescriptorBufferInfo> PointLightBufferInfoList = LightManager::GetPointLightBuffer();
		std::vector<VkDescriptorBufferInfo> SpotLightBufferInfoList = LightManager::GetSpotLightBuffer();

		std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
		GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, MeshPropertiesBufferList);
		GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 1, MeshTransformBufferList);
		GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 2, albedoTextureDescriptor);
		GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 3, normalTextureDescriptor);
		GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 4, albedoTextureDescriptor);
		GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 5, normalTextureDescriptor);
		GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 6, albedoTextureDescriptor);
		GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 7, DirectionalLightBufferInfoList);
		GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 8, PointLightBufferInfoList);
		GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 9, SpotLightBufferInfoList);
		descripterSetList.emplace_back(GLTF_GraphicsDescriptors::SubmitDescriptorSet(DescriptorBindingList));
	}
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
	//	MeshPropertiesList[x].MaterialBufferIndex = MeshList[0]->ChildMeshList[x]->Material->GetMaterialBufferIndex();
		MeshPropertiesBufferList[x].CopyBufferToMemory(&MeshPropertiesList[x], sizeof(MeshProperties));
	}

	//for (int x = 0; x < TextureList.size(); x++)
	//{
	//	TextureList[x]->SetTextureBufferIndex(x);
	//}
	//for (auto& material : MaterialList)
	//{
	//	material->MaterialBufferUpdate();
	//}

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
	UpdateMeshPropertiesBuffer();
	UpdateMeshTransformBuffer();
}

void GLTF_Temp_Model::UpdateMeshPropertiesBuffer()
{
	std::vector<VkDescriptorBufferInfo> MeshPropertiesDescriptorList;
	for (auto meshProperties : MeshPropertiesBufferList)
	{
		auto ab = meshProperties.DataView<MeshProperties>();

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
		auto ab = transformMatrix.DataView<glm::mat4>();

		VkDescriptorBufferInfo transformMatrixPropertiesBufferInfo = {};
		transformMatrixPropertiesBufferInfo.buffer = transformMatrix.GetBuffer();
		transformMatrixPropertiesBufferInfo.offset = 0;
		transformMatrixPropertiesBufferInfo.range = VK_WHOLE_SIZE;
		TransformDescriptorList.emplace_back(transformMatrixPropertiesBufferInfo);
	}
	TransformMatrixBuffer = TransformDescriptorList;
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
	//std::vector<VkDescriptorBufferInfo> MaterialPropertiesBufferList{};
	//for (int x = 0; x < MaterialList.size(); x++)
	//{
	//	auto ab = MaterialList[x]->MaterialBuffer.DataView<MaterialBufferData>();

	//	VkDescriptorBufferInfo materialPropertiesBufferBufferInfo = {};
	//	materialPropertiesBufferBufferInfo.buffer = MaterialList[x]->GetMaterialBuffer();
	//	materialPropertiesBufferBufferInfo.offset = 0;
	//	materialPropertiesBufferBufferInfo.range = VK_WHOLE_SIZE;
	//	MaterialPropertiesBufferList.emplace_back(materialPropertiesBufferBufferInfo);

	//	MaterialList[x]->SetBufferIndex(x);
	//}

	//MaterialPropertiesBuffer = MaterialPropertiesBufferList;
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
