#include "MeshRendererManager.h"

std::vector<std::shared_ptr<Mesh>>  MeshRendererManager::MeshList;
std::shared_ptr<Mesh>  MeshRendererManager::ActiveMesh;

void MeshRendererManager::AddMesh(std::shared_ptr<Mesh> mesh)
{
	MeshList.emplace_back(mesh);
}

//std::shared_ptr<Level2D> MeshRendererManager::AddMesh(std::shared_ptr<Level2D> level2D)
//{
//	MeshList.emplace_back(level2D->GetLevelMesh());
//}

std::shared_ptr<Mesh> MeshRendererManager::GetMeshByID(uint64_t meshID)
{
	for (auto mesh : MeshList)
	{
		if (mesh->GetMeshID() == meshID)
		{
			return mesh;
		}
	}

	return nullptr;
}

void MeshRendererManager::Update()
{
	//	SortByZIndex();
		//for (auto mesh : MeshList)
		//{
		//	mesh->Update(glm::mat4(1.0f), glm::mat4(1.0f));
		//}
}

// void MeshRendererManager::SortByZIndex()
//{
//	//std::sort(MeshList.begin(), MeshList.end(), Mesh::ZSorting{});
//}

// void MeshRendererManagerSortByRenderPipeline()
//{
//	//std::sort(MeshList.begin(), MeshList.end(), Mesh::MeshTypeSort{});
//}

void MeshRendererManager::GUIUpdate()
{
	if (ActiveMesh != nullptr)
	{
		//GetMeshMaterialDetails();
	}
}

//void MeshRendererManager::GetMeshMaterialDetails()
//{
//	ImGui::Begin("Selected Model");
//
//
//	ImGui::NewLine();
//
//	ImGui::LabelText("", "UVTransform");
//	ImGui::SliderFloat2("UV Offset ", &ActiveMesh->GetUVOffset()->x, 0.0f, 1.0f);
//	ImGui::SliderFloat2("UV scale ", &ActiveMesh->GetUVScale()->x, 0.0f, 10.0f);
//	ImGui::SliderFloat2("Flip UV", &ActiveMesh->GetUVFlip()->x, 0.0f, 1.0f);
//
//	ImGui::NewLine();
//
//	ImGui::LabelText("", "Material");
//	const auto material = ActiveMesh->GetMaterial();
//	if (material)
//	{
//		ImGui::SliderFloat3("Reflectivness", &material->Reflectivness, 0.0f, 1.0f);
//
//		if (material->GetMaterialType() == MaterialTypeEnum::kMaterialBlinnPhong)
//		{
//			if (material->GetDiffuseMap())
//			{
//				const auto texture = material->GetDiffuseMap();
//
//				ImGui::LabelText("DiffuseTexture", "DiffuseTexture");
//				ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
//			}
//			else
//			{
//				ImGui::SliderFloat3("Diffuse ", &material->materialTextureData.Diffuse.x, 0.0f, 1.0f);
//			}
//
//			if (material->GetSpecularMap())
//			{
//				const auto texture = material->GetSpecularMap();
//
//				ImGui::LabelText("SpecularMap", "SpecularMap");
//				ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
//			}
//			else
//			{
//				ImGui::SliderFloat3("Specular ", &material->materialTextureData.Specular.x, 0.0f, 1.0f);
//			}
//		}
//		else
//		{
//			if (material->GetAlbedoMap())
//			{
//				const auto texture = material->GetAlbedoMap();
//
//				ImGui::LabelText("Albedo Map", "Albedo Map");
//				ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
//			}
//			else
//			{
//				ImGui::SliderFloat3("Albedo ", &material->materialTextureData.Albedo.x, 0.0f, 1.0f);
//			}
//
//			if (material->GetMetallicMap())
//			{
//				ImGui::LabelText("MetallicMap", "MetallicMap");
//				const auto texture = material->GetMetallicMap();
//				ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
//			}
//			else
//			{
//				ImGui::SliderFloat("Metallic ", &material->materialTextureData.Metallic, 0.0f, 1.0f);
//			}
//
//			if (material->GetRoughnessMap())
//			{
//				const auto texture = material->GetRoughnessMap();
//
//				ImGui::LabelText("RoughnessMap", "RoughnessMap");
//				ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
//			}
//			else
//			{
//				ImGui::SliderFloat("Roughness ", &material->materialTextureData.Roughness, 0.0f, 1.0f);
//			}
//
//			if (material->GetAmbientOcclusionMap())
//			{
//				const auto texture = material->GetAmbientOcclusionMap();
//
//				ImGui::LabelText("AmbientOcclusionMap", "AmbientOcclusionMap");
//				ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
//			}
//			else
//			{
//				ImGui::SliderFloat("AmbientOcclusion ", &material->materialTextureData.AmbientOcclusion, 0.0f, 1.0f);
//				ImGui::Image(TextureManager::GetTexture2DList()[DefaultTextureID]->ImGuiDescriptorSet, ImVec2(100, 100));
//			}
//		}
//
//		if (material->GetNormalMap())
//		{
//			const auto texture = material->GetNormalMap();
//
//			ImGui::LabelText("NormalMap", "NormalMap");
//			ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
//		}
//		else
//		{
//			ImGui::Image(TextureManager::GetTexture2DList()[DefaultTextureID]->ImGuiDescriptorSet, ImVec2(100, 100));
//		}
//
//		if (material->GetDepthMap())
//		{
//			const auto texture = material->GetDepthMap();
//
//			ImGui::LabelText("DepthMap", "DepthMap");
//			ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
//		}
//		else
//		{
//			ImGui::Image(TextureManager::GetTexture2DList()[DefaultTextureID]->ImGuiDescriptorSet, ImVec2(100, 100));
//		}
//
//		if (material->GetAlphaMap())
//		{
//			const auto texture = material->GetAlphaMap();
//
//			ImGui::LabelText("AlphaMap", "AlphaMap");
//			ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
//		}
//		else
//		{
//			ImGui::Image(TextureManager::GetTexture2DList()[DefaultTextureID]->ImGuiDescriptorSet, ImVec2(100, 100));
//		}
//
//		if (material->GetEmissionMap())
//		{
//			const auto texture = material->GetEmissionMap();
//
//			ImGui::LabelText("EmissionMap", "EmissionMap");
//			ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
//		}
//		else
//		{
//			ImGui::Image(TextureManager::GetTexture2DList()[DefaultTextureID]->ImGuiDescriptorSet, ImVec2(100, 100));
//		}
//	}
//
//	ImGui::End();
//}

std::shared_ptr<Mesh> MeshRendererManager::GetMeshByColorID(Pixel pixel)
{
	for (auto mesh : MeshList)
	{
		if (mesh->GetMeshColorID() == pixel)
		{
			return mesh;
		}
	}

	return nullptr;
}

Pixel MeshRendererManager::GetGameObjectsColorID(uint64_t MeshID)
{
	for (auto mesh : MeshList)
	{
		if (mesh->GetMeshID() == MeshID)
		{
			return mesh->GetMeshColorID();
		}
	}

	return Pixel();
}

void MeshRendererManager::SetSelectedMesh(std::shared_ptr<Mesh> mesh)
{
	if (mesh != nullptr)
	{
		if (ActiveMesh != nullptr)
		{
			ActiveMesh->SetSelectedMesh(false);
		}
		ActiveMesh = mesh;
		mesh->SetSelectedMesh(true);
	}
	else
	{
		ActiveMesh = nullptr;
	}
}

//std::vector<VkDescriptorBufferInfo> MeshRendererManager::GetMeshPropertiesBuffer()
//{
//	std::vector<VkDescriptorBufferInfo> MeshPropertiesBufferList{};
//
//	if (MeshList.size() == 0)
//	{
//		VkDescriptorBufferInfo nullBuffer;
//		nullBuffer.buffer = VK_NULL_HANDLE;
//		nullBuffer.offset = 0;
//		nullBuffer.range = VK_WHOLE_SIZE;
//		MeshPropertiesBufferList.emplace_back(nullBuffer);
//	}
//	else
//	{
//		for (auto& mesh : MeshList)
//		{
//			mesh->GetMeshPropertiesBuffer(MeshPropertiesBufferList);
//		}
//	}
//
//	return MeshPropertiesBufferList;
//}

std::vector<VkDescriptorBufferInfo> MeshRendererManager::GetMeshVertexBuffer()
{
	std::vector<VkDescriptorBufferInfo> VertexBufferList{};

	if (MeshList.size() == 0)
	{
		VkDescriptorBufferInfo nullBuffer;
		nullBuffer.buffer = VK_NULL_HANDLE;
		nullBuffer.offset = 0;
		nullBuffer.range = VK_WHOLE_SIZE;
		VertexBufferList.emplace_back(nullBuffer);
	}
	else
	{
		for (auto& mesh : MeshList)
		{
			mesh->GetMeshVertexBuffer(VertexBufferList);
		}
	}

	return VertexBufferList;
}

std::vector<VkDescriptorBufferInfo> MeshRendererManager::GetMeshIndexBuffer()
{
	std::vector<VkDescriptorBufferInfo> IndexBufferList{};


	if (MeshList.size() == 0)
	{
		VkDescriptorBufferInfo nullBuffer;
		nullBuffer.buffer = VK_NULL_HANDLE;
		nullBuffer.offset = 0;
		nullBuffer.range = VK_WHOLE_SIZE;
		IndexBufferList.emplace_back(nullBuffer);
	}
	else
	{
		for (auto& mesh : MeshList)
		{
			mesh->GetMeshIndexBuffer(IndexBufferList);
		}
	}
	return IndexBufferList;
}

void MeshRendererManager::DestroyScene()
{
	for (int x = MeshList.size() - 1; x >= 0; x--)
	{
		MeshList[x]->Destroy();
		MeshList.erase(MeshList.begin() + x);
	}
}

void MeshRendererManager::DestoryMesh(uint64_t MeshID)
{
	std::shared_ptr<Mesh> mesh = GetMeshByID(MeshID);
	mesh->Destroy();
	MeshList.erase(MeshList.begin() + mesh->GetMeshBufferIndex());
}
