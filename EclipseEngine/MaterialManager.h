#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>

#include "Material.h"

class MaterialManager
{
private: 
	static std::shared_ptr<Material> DefaultMaterial;
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
		uint64_t materialID = AddDefaultMaterial();
		DefaultMaterial = GetMaterialByID(materialID);
	}

	static uint64_t AddDefaultMaterial()
	{
		const std::shared_ptr<Material> material = std::make_shared<Material>(Material("DefaultMaterial"));
		MaterialList.emplace_back(material);

		UpdateBufferIndex();
		VulkanRenderer::UpdateRendererFlag = true;
		return material->GetMaterialID();
	}

	static uint64_t AddMaterial(const std::string materialName)
	{
		const std::shared_ptr<Material> material = std::make_shared<Material>(Material(materialName));
		MaterialList.emplace_back(material);

		UpdateBufferIndex();
		VulkanRenderer::UpdateRendererFlag = true;
		return material->GetMaterialID();
	}

	static uint64_t AddMaterial(const std::string materialName, MaterialTypeEnum materialType, MaterialProperties& MaterialInfo)
	{
		const std::shared_ptr<Material> material = std::make_shared<Material>(Material(materialName, materialType, MaterialInfo));
		MaterialList.emplace_back(material);

		UpdateBufferIndex();
		VulkanRenderer::UpdateRendererFlag = true;
		return material->GetMaterialID();
	}

	static uint64_t AddMaterial(std::shared_ptr<Material> material)
	{
	    uint64_t materialID = DoesMaterialExist(material);
		{
			MaterialList.emplace_back(material);
		}

		UpdateBufferIndex();
		VulkanRenderer::UpdateRendererFlag = true;
		return materialID;
	}

	static std::vector<VkDescriptorBufferInfo>  GetMaterialBufferList()
	{
		std::vector<VkDescriptorBufferInfo> MeshPropertiesmBufferList{};
		for (int x = 0; x < MaterialList.size(); x++)
		{
			VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
			MeshPropertiesmBufferBufferInfo.buffer = MaterialList[x]->GetMaterialBuffer();
			MeshPropertiesmBufferBufferInfo.offset = 0;
			MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
			MeshPropertiesmBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

			MaterialList[x]->SetBufferIndex(x);
		}

		return MeshPropertiesmBufferList;
	}

	static std::vector<std::shared_ptr<Material>> GetMaterialList()
	{
		return MaterialList;
	}

	static std::shared_ptr<Material> GetMaterialByID(uint64_t MaterialID)
	{
		uint64_t textureBufferIndex = 0;
		for (auto material : MaterialList)
		{
			if (material->GetMaterialID() == MaterialID)
			{
				return material;
			}
		}

		return nullptr;
	}

	static std::shared_ptr<Material> GetDefaultMaterial()
	{
		return DefaultMaterial;
	}

	static uint64_t DoesMaterialExist(std::shared_ptr<Material> material)
	{
		uint64_t materialID = 0;
		for (auto materialList : MaterialList)
		{
			if (materialList->GetMaterialID() == material->GetMaterialID())
			{
				return material->GetMaterialID();
			}
		}
		return 0;
	}

	static void Destroy()
	{
		for (auto& material : MaterialList)
		{
			material->Destroy();
		}
	}

	static void DestroyScene()
	{
		for (int x = MaterialList.size() - 1; x >= 0; x--)
		{
			MaterialList[x]->Destroy();
			MaterialList.erase(MaterialList.begin() + x);
		}
	}

	static void DestoryMaterial(uint64_t MaterialID)
	{
		std::shared_ptr<Material> material = GetMaterialByID(MaterialID);
		material->Destroy();
		MaterialList.erase(MaterialList.begin() + material->GetMaterialBufferIndex());
	}
};