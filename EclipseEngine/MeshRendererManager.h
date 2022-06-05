#pragma once
#include "VulkanRenderer.h"
#include "GameObjectManager.h"
#include "GraphicsPipeline.h"

class MeshRendererManager
{
private:
	static std::vector<std::shared_ptr<Mesh>> MeshList;
	static std::shared_ptr<Mesh> ActiveMesh;
public:

	static std::shared_ptr<Mesh> AddMesh(glm::vec3& StartPoint, glm::vec3& EndPoint)
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(StartPoint, EndPoint));
		MeshList.emplace_back(mesh);

		return mesh;
	}

	static std::shared_ptr<Mesh> AddMesh(std::vector<LineVertex>& vertices)
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices));
		MeshList.emplace_back(mesh);

		return mesh;
	}

	static std::shared_ptr<Mesh> AddMesh(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices)
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices, indices));
		MeshList.emplace_back(mesh);

		return mesh;
	}

	static std::shared_ptr<Mesh> AddMesh(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices, std::shared_ptr<Material> materialPtr)
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertices, indices, materialPtr));
		MeshList.emplace_back(mesh);

		return mesh;
	}

	static std::shared_ptr<Mesh> AddMesh(MeshLoadingInfo meshLoader)
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(meshLoader));
		MeshList.emplace_back(mesh);

		return mesh;
	}

	static std::shared_ptr<Mesh> AddMesh(std::shared_ptr<Mesh> mesh)
	{
		MeshList.emplace_back(mesh);
		return mesh;
	}

	static void Update()
	{
		SortByZIndex();
	}

	static void SortByZIndex()
	{
		std::sort(MeshList.begin(), MeshList.end(), Mesh::ZSorting{});
	}

	static void SortByRenderPipeline()
	{
		std::sort(MeshList.begin(), MeshList.end(), Mesh::MeshTypeSort{});
	}

	static void GUIUpdate()
	{
		if (ActiveMesh != nullptr)
		{
			ImGui::LabelText("", "Transform");
			ImGui::SliderFloat3("Mesh position ", &ActiveMesh->GetMeshPosition()->x, -100.0f, 100.0f);
			ImGui::SliderFloat3("Mesh rotation ", &ActiveMesh->GetMeshRotation()->x, 0.0f, 360.0f);
			ImGui::SliderFloat3("Mesh scale ", &ActiveMesh->GetMeshScale()->x, 0.0f, 1.0f);
			
			ImGui::NewLine();

			ImGui::LabelText("", "UVTransform");
			ImGui::SliderFloat2("UV Offset ", &ActiveMesh->GetUVOffset()->x, 0.0f, 1.0f);
			ImGui::SliderFloat2("UV scale ", &ActiveMesh->GetUVScale()->x, 0.0f, 10.0f);
			ImGui::SliderFloat2("Flip UV", &ActiveMesh->GetUVFlip()->x, 0.0f, 1.0f);
		

			GetMeshMaterialDetails();
		}
	}

	static void GetMeshMaterialDetails()
	{
		ImGui::NewLine();

		ImGui::LabelText("", "Material");

		const auto material = ActiveMesh->GetMaterial();
		if (material)
		{
			ImGui::LabelText("DiffuseTexture", "DiffuseTexture");
			if (ActiveMesh->GetMaterial()->GetDiffuseMap())
			{
				const auto texture = ActiveMesh->GetMaterial()->GetDiffuseMap();
				ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
			}
			else
			{
				ImGui::Image(TextureManager::GetTexture2DList()[DefaultTextureID]->ImGuiDescriptorSet, ImVec2(100, 100));
			}

			ImGui::LabelText("SpecularMap", "SpecularMap");
			if (ActiveMesh->GetMaterial()->GetSpecularMap())
			{
				const auto texture = ActiveMesh->GetMaterial()->GetSpecularMap();
				ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
			}
			else
			{
				ImGui::Image(TextureManager::GetTexture2DList()[DefaultTextureID]->ImGuiDescriptorSet, ImVec2(100, 100));
			}

			ImGui::LabelText("Albedo Map", "Albedo Map");
			if (ActiveMesh->GetMaterial()->GetAlbedoMap())
			{
				const auto texture = ActiveMesh->GetMaterial()->GetAlbedoMap();
				ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
			}
			else
			{
				ImGui::Image(TextureManager::GetTexture2DList()[DefaultTextureID]->ImGuiDescriptorSet, ImVec2(100, 100));
			}

			ImGui::LabelText("MetallicMap", "MetallicMap");
			if (ActiveMesh->GetMaterial()->GetMetallicMap())
			{
				const auto texture = ActiveMesh->GetMaterial()->GetMetallicMap();
				ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
			}
			else
			{
				ImGui::Image(TextureManager::GetTexture2DList()[DefaultTextureID]->ImGuiDescriptorSet, ImVec2(100, 100));
			}

			ImGui::LabelText("RoughnessMap", "RoughnessMap");
			if (ActiveMesh->GetMaterial()->GetRoughnessMap())
			{
				const auto texture = ActiveMesh->GetMaterial()->GetRoughnessMap();
				ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
			}
			else
			{
				ImGui::Image(TextureManager::GetTexture2DList()[DefaultTextureID]->ImGuiDescriptorSet, ImVec2(100, 100));
			}

			ImGui::LabelText("AmbientOcclusionMap", "AmbientOcclusionMap");
			if (ActiveMesh->GetMaterial()->GetAmbientOcclusionMap())
			{
				const auto texture = ActiveMesh->GetMaterial()->GetAmbientOcclusionMap();
				ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
			}
			else
			{
				ImGui::Image(TextureManager::GetTexture2DList()[DefaultTextureID]->ImGuiDescriptorSet, ImVec2(100, 100));
			}

			ImGui::LabelText("NormalMap", "NormalMap");
			if (ActiveMesh->GetMaterial()->GetNormalMap())
			{
				const auto texture = ActiveMesh->GetMaterial()->GetNormalMap();
				ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
			}
			else
			{
				ImGui::Image(TextureManager::GetTexture2DList()[DefaultTextureID]->ImGuiDescriptorSet, ImVec2(100, 100));
			}

			ImGui::LabelText("DepthMap", "DepthMap");
			if (ActiveMesh->GetMaterial()->GetDepthMap())
			{
				const auto texture = ActiveMesh->GetMaterial()->GetDepthMap();
				ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
			}
			else
			{
				ImGui::Image(TextureManager::GetTexture2DList()[DefaultTextureID]->ImGuiDescriptorSet, ImVec2(100, 100));
			}

			ImGui::LabelText("AlphaMap", "AlphaMap");
			if (ActiveMesh->GetMaterial()->GetAlphaMap())
			{
				const auto texture = ActiveMesh->GetMaterial()->GetAlphaMap();
				ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
			}
			else
			{
				ImGui::Image(TextureManager::GetTexture2DList()[DefaultTextureID]->ImGuiDescriptorSet, ImVec2(100, 100));
			}

			ImGui::LabelText("EmissionMap", "EmissionMap");
			if (ActiveMesh->GetMaterial()->GetEmissionMap())
			{
				const auto texture = ActiveMesh->GetMaterial()->GetEmissionMap();
				ImGui::Image(texture->ImGuiDescriptorSet, ImVec2(100, 100));
			}
			else
			{
				ImGui::Image(TextureManager::GetTexture2DList()[DefaultTextureID]->ImGuiDescriptorSet, ImVec2(100, 100));
			}
		}
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


	static void DrawMesh(VkCommandBuffer& cmdBuffer, std::shared_ptr<GraphicsPipeline> pipeline, std::shared_ptr<Mesh> mesh, SceneProperties& sceneProperties)
	{
		sceneProperties.MeshIndex = mesh->GetMeshBufferIndex();
		sceneProperties.MeshColorID = Converter::PixelToVec3(mesh->GetMeshColorID());
		vkCmdPushConstants(cmdBuffer, pipeline->GetShaderPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SceneProperties), &sceneProperties);
		mesh->Draw(cmdBuffer);
	}

	static void DrawLine(VkCommandBuffer& cmdBuffer, std::shared_ptr<GraphicsPipeline> pipeline, std::shared_ptr<Mesh> mesh, SceneProperties& sceneProperties)
	{
		sceneProperties.MeshIndex = mesh->GetMeshBufferIndex();
		sceneProperties.MeshColorID = Converter::PixelToVec3(mesh->GetMeshColorID());
		vkCmdPushConstants(cmdBuffer, pipeline->GetShaderPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SceneProperties), &sceneProperties);
		mesh->Draw(cmdBuffer);
	}

	static void DrawSkybox(VkCommandBuffer& cmdBuffer, std::shared_ptr<GraphicsPipeline> pipeline, std::shared_ptr<Mesh> mesh)
	{
		mesh->Draw(cmdBuffer);
	}

	static void DrawSkybox(VkCommandBuffer& cmdBuffer, std::shared_ptr<GraphicsPipeline> pipeline, std::shared_ptr<Mesh> mesh, ConstSkyBoxView& skyboxView)
	{
		vkCmdPushConstants(cmdBuffer, pipeline->GetShaderPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(ConstSkyBoxView), &skyboxView);
		mesh->Draw(cmdBuffer);
	}

	static void DrawSkybox(VkCommandBuffer& cmdBuffer, std::shared_ptr<GraphicsPipeline> pipeline, std::shared_ptr<Mesh> mesh, PrefilterSkyboxSettings& prefilterSkyboxSettings)
	{
		vkCmdPushConstants(cmdBuffer, pipeline->GetShaderPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(prefilterSkyboxSettings), &prefilterSkyboxSettings);
		mesh->Draw(cmdBuffer);
	}


	static void SetSelectedMesh(std::shared_ptr<Mesh> mesh)
	{
		ActiveMesh = mesh;
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

	static std::shared_ptr<Mesh> GetSelectedMesh() { return ActiveMesh; }
	static std::vector<std::shared_ptr<Mesh>> GetMeshList() { return MeshList; }
};

