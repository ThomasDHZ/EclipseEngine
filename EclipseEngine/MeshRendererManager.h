#pragma once
#include "VulkanRenderer.h"
#include "GraphicsPipeline.h"
#include "Mesh.h"

class MeshRendererManager
{
private:
	static std::vector<std::shared_ptr<Mesh>> MeshList;
	static std::shared_ptr<Mesh> ActiveMesh;
public:


	static void AddMesh(std::shared_ptr<Mesh> mesh)
	{
		MeshList.emplace_back(mesh);
	}

	//static std::shared_ptr<Level2D> AddMesh(std::shared_ptr<Level2D> level2D)
	//{
	//	MeshList.emplace_back(level2D->GetLevelMesh());
	//}

	static std::shared_ptr<Mesh> GetMeshByID(uint64_t meshID)
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

	static void Update()
	{
	//	SortByZIndex();
		//for (auto mesh : MeshList)
		//{
		//	mesh->Update(glm::mat4(1.0f), glm::mat4(1.0f));
		//}
	}

	//static void SortByZIndex()
	//{
	//	//std::sort(MeshList.begin(), MeshList.end(), Mesh::ZSorting{});
	//}

	//static void SortByRenderPipeline()
	//{
	//	//std::sort(MeshList.begin(), MeshList.end(), Mesh::MeshTypeSort{});
	//}

	static void GUIUpdate()
	{
		if (ActiveMesh != nullptr)
		{
			GetMeshMaterialDetails();
		}
	}

	static void GetMeshMaterialDetails()
	{
		ImGui::Begin("Selected Model");

		ImGui::LabelText("", "Transform");
		//ImGui::SliderFloat3("Mesh position ", &ActiveMesh->GetMeshPosition()->x, -100.0f, 100.0f);
		//ImGui::SliderFloat3("Mesh rotation ", &ActiveMesh->GetMeshRotation()->x, 0.0f, 360.0f);
		//ImGui::SliderFloat3("Mesh scale ", &ActiveMesh->GetMeshScale()->x, 0.0f, 1.0f);

		ImGui::NewLine();

		ImGui::LabelText("", "UVTransform");
		ImGui::SliderFloat2("UV Offset ", &ActiveMesh->GetUVOffset()->x, 0.0f, 1.0f);
		ImGui::SliderFloat2("UV scale ", &ActiveMesh->GetUVScale()->x, 0.0f, 10.0f);
		ImGui::SliderFloat2("Flip UV", &ActiveMesh->GetUVFlip()->x, 0.0f, 1.0f);

		ImGui::NewLine();

		ImGui::LabelText("", "Material");
		const auto material = ActiveMesh->GetMaterial();
		if (material)
		{
			if (material->GetMaterialType() == MaterialTypeEnum::kMaterialBlinnPhong)
			{
				if (material->GetDiffuseMap())
				{
					const auto texture = material->GetDiffuseMap();

					ImGui::LabelText("DiffuseTexture", "DiffuseTexture");
					ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
				}
				else
				{
					ImGui::SliderFloat3("Diffuse ", &material->materialTextureData.Diffuse.x, 0.0f, 1.0f);
				}

				if (material->GetSpecularMap())
				{
					const auto texture = material->GetSpecularMap();

					ImGui::LabelText("SpecularMap", "SpecularMap");
					ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
				}
				else
				{
					ImGui::SliderFloat3("Specular ", &material->materialTextureData.Specular.x, 0.0f, 1.0f);
				}
			}
			else
			{
				if (material->GetAlbedoMap())
				{
					const auto texture = material->GetAlbedoMap();

					ImGui::LabelText("Albedo Map", "Albedo Map");
					ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
				}
				else
				{
					ImGui::SliderFloat3("Albedo ", &material->materialTextureData.Albedo.x, 0.0f, 1.0f);
				}

				if (material->GetMetallicMap())
				{
					ImGui::LabelText("MetallicMap", "MetallicMap");
					const auto texture = material->GetMetallicMap();
					ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
				}
				else
				{
					ImGui::SliderFloat("Metallic ", &material->materialTextureData.Metallic, 0.0f, 1.0f);
				}

				if (material->GetRoughnessMap())
				{
					const auto texture = material->GetRoughnessMap();

					ImGui::LabelText("RoughnessMap", "RoughnessMap");
					ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
				}
				else
				{
					ImGui::SliderFloat("Roughness ", &material->materialTextureData.Roughness, 0.0f, 1.0f);
				}

				if (material->GetAmbientOcclusionMap())
				{
					const auto texture = material->GetAmbientOcclusionMap();

					ImGui::LabelText("AmbientOcclusionMap", "AmbientOcclusionMap");
					ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
				}
				else
				{
					ImGui::SliderFloat("AmbientOcclusion ", &material->materialTextureData.AmbientOcclusion, 0.0f, 1.0f);
					ImGui::Image(TextureManager::GetTexture2DList()[DefaultTextureID]->ImGuiDescriptorSet, ImVec2(100, 100));
				}
			}

			if (material->GetNormalMap())
			{
				const auto texture = material->GetNormalMap();

				ImGui::LabelText("NormalMap", "NormalMap");
				ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
			}
			else
			{
				ImGui::Image(TextureManager::GetTexture2DList()[DefaultTextureID]->ImGuiDescriptorSet, ImVec2(100, 100));
			}

			if (material->GetDepthMap())
			{
				const auto texture = material->GetDepthMap();

				ImGui::LabelText("DepthMap", "DepthMap");
				ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
			}
			else
			{
				ImGui::Image(TextureManager::GetTexture2DList()[DefaultTextureID]->ImGuiDescriptorSet, ImVec2(100, 100));
			}

			if (material->GetAlphaMap())
			{
				const auto texture = material->GetAlphaMap();

				ImGui::LabelText("AlphaMap", "AlphaMap");
				ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
			}
			else
			{
				ImGui::Image(TextureManager::GetTexture2DList()[DefaultTextureID]->ImGuiDescriptorSet, ImVec2(100, 100));
			}

			if (material->GetEmissionMap())
			{
				const auto texture = material->GetEmissionMap();

				ImGui::LabelText("EmissionMap", "EmissionMap");
				ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
			}
			else
			{
				ImGui::Image(TextureManager::GetTexture2DList()[DefaultTextureID]->ImGuiDescriptorSet, ImVec2(100, 100));
			}
		}

		ImGui::End();
	}

	static std::shared_ptr<Mesh> GetMeshByColorID(Pixel pixel)
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

	static Pixel GetGameObjectsColorID(uint64_t MeshID)
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

	static void SetSelectedMesh(std::shared_ptr<Mesh> mesh)
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

	static std::vector<VkDescriptorBufferInfo> GetMeshPropertiesBuffer()
	{
		std::vector<VkDescriptorBufferInfo> MeshPropertiesBufferList{};

		if (VulkanRenderer::UpdateRendererFlag)
		{
			if (MeshList.size() == 0)
			{
				VkDescriptorBufferInfo nullBuffer;
				nullBuffer.buffer = VK_NULL_HANDLE;
				nullBuffer.offset = 0;
				nullBuffer.range = VK_WHOLE_SIZE;
				MeshPropertiesBufferList.emplace_back(nullBuffer);
			}
			else
			{
				for (auto& mesh : MeshList)
				{
					mesh->GetMeshPropertiesBuffer(MeshPropertiesBufferList);
				}
			}
		}
		else
		{
			std::cout << "Can't update MeshBuffers unless pipelines in the process of being rebuild." << std::endl;
		}
		return MeshPropertiesBufferList;
	}

	static std::vector<VkDescriptorBufferInfo> GetMeshVertexBuffer()
	{
		std::vector<VkDescriptorBufferInfo> VertexBufferList{};

		if (VulkanRenderer::UpdateRendererFlag)
		{
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
		}
		else
		{
			std::cout << "Can't update IndexBuffers unless pipelines in the process of being rebuild." << std::endl;
		}
		return VertexBufferList;
	}

	static std::vector<VkDescriptorBufferInfo> GetMeshIndexBuffer()
	{
		std::vector<VkDescriptorBufferInfo> IndexBufferList{};

		if (VulkanRenderer::UpdateRendererFlag)
		{
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
		}
		else
		{
			std::cout << "Can't update IndexBuffers unless pipelines in the process of being rebuild." << std::endl;
		}
		return IndexBufferList;
	}

	static void DestroyScene()
	{
		for (int x = MeshList.size() - 1; x >= 0; x--)
		{
			MeshList[x]->Destroy();
			MeshList.erase(MeshList.begin() + x);
		}
	}

	static void DestoryMesh(uint64_t MeshID)
	{
		std::shared_ptr<Mesh> mesh = GetMeshByID(MeshID);
		mesh->Destroy();
		MeshList.erase(MeshList.begin() + mesh->GetMeshBufferIndex());
	}

	static std::shared_ptr<Mesh> GetSelectedMesh() { return ActiveMesh; }
	static std::vector<std::shared_ptr<Mesh>> GetMeshList() { return MeshList; }
};

