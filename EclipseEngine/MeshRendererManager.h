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

	static void AddMesh(std::shared_ptr<Mesh> mesh);
	//static std::shared_ptr<Level2D> AddMesh(std::shared_ptr<Level2D> level2D);
	static std::shared_ptr<Mesh> GetMeshByID(uint64_t meshID);
	static void Update();
	//static void SortByZIndex();
	//static void SortByRenderPipeline();
	static void GUIUpdate();
	static void GetMeshMaterialDetails();
	static std::shared_ptr<Mesh> GetMeshByColorID(Pixel pixel);
	static Pixel GetGameObjectsColorID(uint64_t MeshID);
	static void SetSelectedMesh(std::shared_ptr<Mesh> mesh);
	static std::vector<VkDescriptorBufferInfo> GetMeshPropertiesBuffer()
	{
		std::vector<VkDescriptorBufferInfo> MeshPropertiesBufferList{};

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

		return MeshPropertiesBufferList;
	}
	static std::vector<VkDescriptorBufferInfo> GetMeshVertexBuffer();
	static std::vector<VkDescriptorBufferInfo> GetMeshIndexBuffer();
	static void DestroyScene();
	static void DestoryMesh(uint64_t MeshID);

	static std::shared_ptr<Mesh> GetSelectedMesh() { return ActiveMesh; }
	static std::vector<std::shared_ptr<Mesh>> GetMeshList() { return MeshList; }
};

