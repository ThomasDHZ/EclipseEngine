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
#include "ModelManager.h"

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
		SkyboxMesh->Destroy();

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

		json["ObjectList"] = GameObjectManager::SaveGameObjects();
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
		DestroyScene();

		std::string SceneInfo;
		std::ifstream SceneFile;
		SceneFile.open(FileName);
		while (!SceneFile.eof())
		{
			getline(SceneFile, SceneInfo);
		}
		SceneFile.close();

		nlohmann::json jsonstring = nlohmann::json::parse(SceneInfo);

		GameObjectManager::LoadGameObjects(jsonstring);
		//MaterialManager::LoadMaterials(jsonstring);
		LightManager::LoadLights(jsonstring);

	}

	static void DestroyScene()
	{
		MaterialManager::DestroyScene();
		TextureManager::DestroyScene();
		GameObjectManager::DestroyScene();
		ModelManager::DestroyScene();
		MeshRendererManager::DestroyScene();
		LightManager::DestroyScene();
	}

	static std::shared_ptr<Skybox> GetSkyboxMesh() { return SkyboxMesh; };
	static float GetPBRCubeMapSize() { return PBRCubeMapSize; }
};

