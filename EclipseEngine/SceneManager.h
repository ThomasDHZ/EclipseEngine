#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>

#include "Camera.h"

#include "RenderedColorTexture.h"
#include "RenderedCubeMapTexture.h"
#include "SkyboxMesh.h"

#include "GameObjectManager.h"
#include "LightManager.h"
#include "MaterialManager.h"
#include "TopLevelAccelerationStructureManager.h"
#include "ComponentRenderer.h"
#include "MusicPlayer.h"

enum SceneType
{
	kBlinnPhong,
	kPBR,
	kSprite2D
};

class SceneManager
{
private:

	static float PBRCubeMapSize;
	static float PreRenderedMapSize;

	static std::shared_ptr<SkyboxMesh> skyboxMesh;

public:
	static SceneType sceneType;
	static std::shared_ptr<Camera> activeCamera;

	static SceneProperties sceneProperites;
	static ConstSkyBoxView cubeMapInfo;
	static GaussianBlurSettings bloomsettings;
	static std::shared_ptr<EnvironmentTexture>     environmentTexture;
	static std::shared_ptr<RenderedColorTexture>   BRDFTexture;
	static std::shared_ptr<RenderedCubeMapTexture> CubeMap;
	static std::shared_ptr<CubeMapTexture> CubeMap2;
	static bool EditorModeFlag;
	static bool	RayTracingActive;
	static bool	HybridRendererActive;
	static MusicPlayer musicPlayer;

	static void StartUp()
	{
		musicPlayer.StartUp("../Music/AMBForst_Forest (ID 0100)_BSB.wav");
		musicPlayer.Play();

		skyboxMesh = std::make_shared<SkyboxMesh>();
		skyboxMesh->StartUp();
	}

	static void Update()
	{
		auto time = (float)glfwGetTime();
		GameObjectManager::Update(time);
		MeshRendererManager::Update();
		TopLevelAccelerationStructureManager::Update();
		LightManager::Update();

		activeCamera->Update(time);
		musicPlayer.UpdateBufferStream();

		sceneProperites.CameraPos = SceneManager::activeCamera->GetPosition();
		sceneProperites.view = SceneManager::activeCamera->GetViewMatrix();
		sceneProperites.proj = SceneManager::activeCamera->GetProjectionMatrix();
		sceneProperites.DirectionalLightCount = LightManager::GetDirectionalLightCount();
		sceneProperites.PointLightCount = LightManager::GetPointLightCount();
		sceneProperites.SpotLightCount = LightManager::GetSpotLightCount();
		sceneProperites.Timer = time;
		sceneProperites.frame++;
		if (sceneProperites.frame == UINT32_MAX)
		{
			sceneProperites.frame = 0;
		}
		sceneProperites.MaxReflectCount = 2;

		cubeMapInfo.view = glm::mat4(glm::mat3(SceneManager::activeCamera->GetViewMatrix()));
		cubeMapInfo.proj = glm::perspective(glm::radians(SceneManager::activeCamera->GetZoom()), VulkanRenderer::GetSwapChainResolution().width / (float)VulkanRenderer::GetSwapChainResolution().height, 0.1f, 100.0f);
		cubeMapInfo.proj[1][1] *= -1;
	}

	static void Destory()
	{
		skyboxMesh->Destroy();
		musicPlayer.Destroy();

		if (environmentTexture != nullptr)
		{
			environmentTexture->Destroy();
		}
		if (BRDFTexture != nullptr)
		{
			BRDFTexture->Destroy();
		}
		if (CubeMap != nullptr)
		{
			CubeMap->Destroy();
		}
	}

	static void ImGuiSceneHierarchy()
	{
		ImGui::Begin("Sound Window");
		if (ImGui::Button("Play"))
		{
			musicPlayer.Play();
		}

		if (ImGui::Button("Pause"))
		{
			musicPlayer.Pause();
		}

		if (ImGui::Button("Stop"))
		{
			musicPlayer.Stop();
		}
		ImGui::End();

		//ImGui::Begin("Scene");
	
		//for (int x = 0; x < GameObjectManager::GetGameObjectList().size(); x++)
		//{
		//	const std::shared_ptr<GameObject> obj = GameObjectManager::GetGameObjectList()[x];

		//	ImGui::PushID(x);
		//	bool TreeNodeOpen = ImGui::TreeNodeEx(obj->GetObjectName().c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth);
		//	ImGui::TreePop();

		//	if (TreeNodeOpen)
		//	{
		//		const auto component = obj->GetComponentBySubType(ComponentSubType::kRenderedObject);
		//		const auto componentRenderer = static_cast<ComponentRenderer*>(component.get());
		//		const auto model = componentRenderer->GetModel();
		//		if(componentRenderer)
		//		{
		//			//ImGui::PushID(0);
		//			bool ModelTreeNodeOpen = ImGui::TreeNodeEx(model->GetModelFilePath().c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth);
		//		//	ImGui::TreePop();
		//		}
		//		
		//	}
		//}
		//ImGui::End();
	}

	static void SaveScene(const std::string FileName)
	{
		nlohmann::json json;

		auto texturepath = environmentTexture->GetFilePath();
		JsonConverter::to_json(json["environmentTexture"], texturepath);
		JsonConverter::to_json(json["sceneType"], sceneType);

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

	static void SaveAsPrefab(std::shared_ptr<GameObject> obj)
	{
		nlohmann::json json;
		obj->SaveAsPrefab(json);
		std::ofstream SaveFile("../Obj/" + obj->GetObjectName() + ".txt");
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

		environmentTexture = std::make_shared<EnvironmentTexture>(jsonstring.at("environmentTexture"), VK_FORMAT_R32G32B32A32_SFLOAT);
		sceneType = jsonstring.at("sceneType");

		GameObjectManager::LoadGameObjects(jsonstring);
		//MaterialManager::LoadMaterials(jsonstring);
		LightManager::LoadLights(jsonstring);
	}

	static void LoadPrefab(const std::string FileName)
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
		
	}

	static void DestroyScene()
	{
		MaterialManager::DestroyScene();
		TextureManager::DestroyScene();
		GameObjectManager::DestroyScene();
		MeshRendererManager::DestroyScene();
		LightManager::DestroyScene();

		TextureManager::StartUp();
	}

	static SceneType GetSceneType() { return sceneType; }
	static std::shared_ptr<SkyboxMesh> GetSkyboxMesh() { return skyboxMesh; };
	static float GetPBRCubeMapSize() { return PBRCubeMapSize; }
	static float GetPreRenderedMapSize() { return PreRenderedMapSize; }
	static bool IsRayTracerActive() { return RayTracingActive; }
	static bool IsHybridRendererActive() { return HybridRendererActive; }
};

