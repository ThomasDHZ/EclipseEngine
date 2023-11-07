#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include "GLTFSceneManager.h"
#include "Camera.h"

#include "RenderedColorTexture.h"
#include "RenderedCubeMapTexture.h"

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

public:
	static SceneType sceneType;
	static GaussianBlurSettings bloomsettings;
	static bool EditorModeFlag;
	static bool	RayTracingActive;
	static bool	HybridRendererActive;
	static MusicPlayer musicPlayer;

	static VkSampler NullSampler;
	static VkDescriptorImageInfo nullBufferInfo;

	static void StartUp()
	{
		//musicPlayer.StartUp("../Music/AMBForst_Forest (ID 0100)_BSB.wav");
		//musicPlayer.Play();
	}

	static void Update()
	{
		//musicPlayer.UpdateBufferStream();
	}

	static void Destory()
	{
		//musicPlayer.Destroy();
	}

	static void ImGuiSceneHierarchy()
	{
		ImGui::Begin("Sound Window");
		if (ImGui::Button("Play"))
		{
			//musicPlayer.Play();
		}

		if (ImGui::Button("Pause"))
		{
			//musicPlayer.Pause();
		}

		if (ImGui::Button("Stop"))
		{
			//musicPlayer.Stop();
		}
		ImGui::End();
	}


	static void DestroyScene()
	{

	}

	static SceneType GetSceneType() { return sceneType; }
	static bool IsRayTracerActive() { return RayTracingActive; }
	static bool IsHybridRendererActive() { return HybridRendererActive; }
};

