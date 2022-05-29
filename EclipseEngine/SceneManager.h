#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>

#include "RenderedColorTexture.h"
#include "RenderedCubeMapTexture.h"
#include "Skybox.h"

#include "GameObjectManager.h"
#include "LightManager.h"
#include "MaterialManager.h"

class SceneManager
{
private:
	static float PBRCubeMapSize;

	static std::shared_ptr<Skybox> SkyboxMesh;

public:
	static std::shared_ptr<RenderedColorTexture>   BRDFTexture;
	static std::shared_ptr<RenderedCubeMapTexture> IrradianceCubeMap;
	static std::shared_ptr<RenderedCubeMapTexture> PrefilterCubeMap;
	static std::shared_ptr<RenderedCubeMapTexture> CubeMap;

	static void StartUp()
	{
		SkyboxMesh = std::make_shared<Skybox>();
		SkyboxMesh->StartUp();
	}

	static void Destory()
	{
		SkyboxMesh->Destory();

		if (BRDFTexture != nullptr)
		{
			BRDFTexture->Destroy();
		}
		if (IrradianceCubeMap != nullptr)
		{
			IrradianceCubeMap->Destroy();
		}
		if (PrefilterCubeMap != nullptr)
		{
			PrefilterCubeMap->Destroy();
		}
		if (CubeMap != nullptr)
		{
			CubeMap->Destroy();
		}
	}

	static void SaveScene(const std::string FileName)
	{
		nlohmann::json json;

		json["GameObjectList"] = GameObjectManager::SaveGameObjects();
		json["MaterialList"] = MaterialManager::SaveMaterials();
		json["LightList"] = LightManager::SaveLights();

		std::ofstream SaveFile("../Scenes/example.txt");
		if (SaveFile.is_open())
		{
			SaveFile << json;
			SaveFile.close();
		}
		else std::cout << "Unable to open file";
	}

	static void LoadScene(const std::string FileName)
	{
		std::string SceneInfo;
		std::ifstream SceneFile;
		SceneFile.open(FileName);
		while (!SceneFile.eof())
		{
			getline(SceneFile, SceneInfo);
		}
		SceneFile.close();

		nlohmann::json jsonstring = nlohmann::json::parse(SceneInfo);

		MaterialManager::LoadMaterials(jsonstring);
		LightManager::LoadLights(jsonstring);

	}

	static std::shared_ptr<Skybox> GetSkyboxMesh() { return SkyboxMesh; };
	static float GetPBRCubeMapSize() { return PBRCubeMapSize; }
};

