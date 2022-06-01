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

	static uint64_t AddMaterial(const std::string materialName, MaterialProperties& MaterialInfo)
	{
		const std::shared_ptr<Material> material = std::make_shared<Material>(Material(materialName, MaterialInfo));
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

	static void AddMaterial(const nlohmann::json& json)
	{
		//uint64_t materialID = DoesMaterialExist(material);
		//{
		//	MaterialList.emplace_back(material);
		//}

		std::cout << json << std::endl;

		std::shared_ptr<Material> material = std::make_shared<Material>(Material(json));
		MaterialList.emplace_back(material);

		UpdateBufferIndex();
		VulkanRenderer::UpdateRendererFlag = true;
	}

	static void Update()
	{
		for (auto& material : MaterialList)
		{
			material->Update();
		}
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

	static void LoadMaterials(nlohmann::json& json)
	{
		auto a = json["MaterialList"].size();
		for (int x = 0; x < json["MaterialList"].size(); x++)
		{
			std::shared_ptr<Material> material = std::make_shared<Material>(Material(json.at("MaterialList")[x]));
			MaterialList.emplace_back(material);
		}

		UpdateBufferIndex();
		VulkanRenderer::UpdateRendererFlag = true;
	}

	static nlohmann::json SaveMaterials()
	{
		nlohmann::json json;
		for (int x = 0; x < MaterialList.size(); x++)
		{
			MaterialList[x]->to_json(json[x]);
		}
		return json;
	}

	static void LoadMaterial(const std::string FilePath)
	{
		std::string SceneInfo;
		std::ifstream SceneFile;
		SceneFile.open(FilePath);
		if (SceneFile.is_open())
		{
			while (!SceneFile.eof())
			{
				getline(SceneFile, SceneInfo);
			}
		}
		else std::cout << "Unable to open file";
		SceneFile.close();

		nlohmann::json jsonstring = nlohmann::json::parse(SceneInfo);
		AddMaterial(jsonstring);
	}

	static void SaveMaterial(std::shared_ptr<Material> material)
	{
		nlohmann::json json;

		material->to_json(json["Material"]);

		std::ofstream SaveFile("../Materials/" + material->GetMaterialName() + ".txt");
		if (SaveFile.is_open())
		{
			SaveFile << json;
			SaveFile.close();
		}
		else std::cout << "Unable to open file";
	}

	static void Destroy()
	{
		for (auto& material : MaterialList)
		{
			material->Destroy();
		}
	}
};