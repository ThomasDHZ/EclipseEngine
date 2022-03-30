#pragma once
#include "VulkanRenderer.h"
#include "Mesh.h"
#include "GraphicsPipeline.h"

class MeshRendererManager
{
private:
	static std::vector<std::shared_ptr<Mesh>> MeshList;

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
		for (auto& mesh : MeshList)
		{
			mesh->UpdateMeshProperties();
		}
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

		for (int x = 0; x < MeshList.size(); x++)
		{
		
			ImGui::SliderFloat3(("Mesh position " + std::to_string(x)).c_str(), &MeshList[x]->GetMeshPosition()->x, 0.0f, 100.0f);
			ImGui::SliderFloat3(("Mesh rotation " + std::to_string(x)).c_str(), &MeshList[x]->GetMeshRotation()->x, 0.0f, 360.0f);
			ImGui::SliderFloat3(("Mesh scale " + std::to_string(x)).c_str(), &MeshList[x]->GetMeshScale()->x, 0.0f, 1.0f);
		}
	}

	static void DrawMesh(VkCommandBuffer& cmdBuffer, std::shared_ptr<GraphicsPipeline> pipeline, std::shared_ptr<Mesh> mesh, SceneProperties& sceneProperties)
	{
		sceneProperties.MeshIndex = mesh->GetMeshBufferIndex();
		vkCmdPushConstants(cmdBuffer, pipeline->GetShaderPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SceneProperties), &sceneProperties);
		mesh->Draw(cmdBuffer);
	}

	static void DrawLine(VkCommandBuffer& cmdBuffer, std::shared_ptr<GraphicsPipeline> pipeline, std::shared_ptr<Mesh> mesh, SceneProperties& sceneProperties)
	{
		sceneProperties.MeshIndex = mesh->GetMeshBufferIndex();
		vkCmdPushConstants(cmdBuffer, pipeline->GetShaderPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SceneProperties), &sceneProperties);
		mesh->Draw(cmdBuffer);
	}

	static std::vector<VkDescriptorBufferInfo> GetMeshPropertiesBuffer()
	{
		std::vector<VkDescriptorBufferInfo> MeshPropertiesBufferList{};

		if (VulkanRenderer::UpdateRendererFlag)
		{
			for (auto& mesh : MeshList)
			{
				mesh->GetMeshPropertiesBuffer(MeshPropertiesBufferList);
			}
		}
		else
		{
			std::cout << "Can't update IndexBuffers unless pipelines in the process of being rebuild." << std::endl;
		}
		return MeshPropertiesBufferList;
	}

	static std::vector<VkDescriptorBufferInfo> GetMeshVertexBuffer()
	{
		std::vector<VkDescriptorBufferInfo> VertexBufferList{};

		if (VulkanRenderer::UpdateRendererFlag)
		{
			for (auto& mesh : MeshList)
			{
				mesh->GetMeshVertexBuffer(VertexBufferList);
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
			for (auto& mesh : MeshList)
			{
				mesh->GetMeshIndexBuffer(IndexBufferList);
			}
		}
		else
		{
			std::cout << "Can't update IndexBuffers unless pipelines in the process of being rebuild." << std::endl;
		}
		return IndexBufferList;
	}

	static std::vector<std::shared_ptr<Mesh>> GetMeshList() { return MeshList; }
};

