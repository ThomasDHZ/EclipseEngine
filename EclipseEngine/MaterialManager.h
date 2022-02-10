#pragma once
#include <vector>
#include <memory>
#include "Material.h"

class MaterialManager
{
private: 
	static std::vector<std::shared_ptr<Material>> MaterialList;

	static void UpdateBufferIndex()
	{
		for (int x = 0; x < MaterialList.size(); x++)
		{
			MaterialList[x]->SetBufferIndex(x);
		}
	}

public:
	static void StartUp()
	{
		AddMaterial("DefaultMaterial");
	}

	static uint64_t AddMaterial(const std::string materialName)
	{
		const std::shared_ptr<Material> material = std::make_shared<Material>(Material(materialName));
		MaterialList.emplace_back(material);

		UpdateBufferIndex();
		VulkanRenderer::UpdateRendererFlag = true;
		return material->GetMaterialID();
	}

	static uint64_t AddMaterial(const std::string materialName, MaterialProperties& MaterialInfo)
	{
		const std::shared_ptr<Material> material = std::make_shared<Material>(Material(materialName, MaterialInfo));
		MaterialList.emplace_back(material);

		UpdateBufferIndex();
		VulkanRenderer::UpdateRendererFlag = true;
		return material->GetMaterialID();
	}

	static std::vector<VkDescriptorBufferInfo>  GetMaterialemBufferList()
	{
		std::vector<VkDescriptorBufferInfo> MeshPropertiesmBufferList{};
		for (auto material : MaterialList)
		{
			VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
			MeshPropertiesmBufferBufferInfo.buffer = material->GetMaterialBuffer();
			MeshPropertiesmBufferBufferInfo.offset = 0;
			MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
			MeshPropertiesmBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

			material->SetBufferIndex(MeshPropertiesmBufferList.size());
		}

		return MeshPropertiesmBufferList;
	}

	static std::vector<std::shared_ptr<Material>> GetMaterialList()
	{
		return MaterialList;
	}

	static void Destroy()
	{
		for (auto& material : MaterialList)
		{
			material->Destroy();
		}
	}
};