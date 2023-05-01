#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include "GLTFSceneManager.h"
#include "Camera.h"

#include "RenderedColorTexture.h"
#include "RenderedCubeMapTexture.h"

#include "GameObjectManager.h"
#include "LightManager.h"
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

	//static float PBRCubeMapSize;
	//static float PreRenderedMapSize;


public:
	static SceneType sceneType;


	//static SceneProperties sceneProperites;
	//static SkyBoxView cubeMapInfo;
	static GaussianBlurSettings bloomsettings;
	//static std::shared_ptr<EnvironmentTexture>     environmentTexture;
	//static std::shared_ptr<RenderedColorTexture>   BRDFTexture;
	//static std::shared_ptr<RenderedCubeMapTexture> IrradianceMap;
	//static std::shared_ptr<RenderedCubeMapTexture> PrefilterMap;
	//static std::shared_ptr<RenderedCubeMapTexture> CubeMap;
	static bool EditorModeFlag;
	static bool	RayTracingActive;
	static bool	HybridRendererActive;
	static MusicPlayer musicPlayer;

	static VkSampler NullSampler;
	static VkDescriptorImageInfo nullBufferInfo;

	static void StartUp()
	{
		VkSamplerCreateInfo NullSamplerInfo = {};
		NullSamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		NullSamplerInfo.magFilter = VK_FILTER_NEAREST;
		NullSamplerInfo.minFilter = VK_FILTER_NEAREST;
		NullSamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		NullSamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		NullSamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		NullSamplerInfo.anisotropyEnable = VK_TRUE;
		NullSamplerInfo.maxAnisotropy = 16.0f;
		NullSamplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		NullSamplerInfo.unnormalizedCoordinates = VK_FALSE;
		NullSamplerInfo.compareEnable = VK_FALSE;
		NullSamplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		NullSamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		NullSamplerInfo.minLod = 0;
		NullSamplerInfo.maxLod = 0;
		NullSamplerInfo.mipLodBias = 0;
		if (vkCreateSampler(VulkanRenderer::GetDevice(), &NullSamplerInfo, nullptr, &NullSampler))
		{
			throw std::runtime_error("Failed to create Sampler.");
		}

		nullBufferInfo.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		nullBufferInfo.imageView = VK_NULL_HANDLE;
		nullBufferInfo.sampler = NullSampler;

		musicPlayer.StartUp("../Music/AMBForst_Forest (ID 0100)_BSB.wav");
		musicPlayer.Play();
	}

	static void Update()
	{
		GameObjectManager::Update(VulkanRenderer::GetFrameTimeDurationMilliseconds());
		MeshRendererManager::Update();
		//TopLevelAccelerationStructureManager::Update();
		LightManager::Update();

		GLTFSceneManager::ActiveCamera->Update(VulkanRenderer::GetFrameTimeDurationMilliseconds());
		musicPlayer.UpdateBufferStream();
	}

	static void Destory()
	{
		musicPlayer.Destroy();

		//if (environmentTexture != nullptr)
		//{
		//	environmentTexture->Destroy();
		//}
		//if (BRDFTexture != nullptr)
		//{
		//	BRDFTexture->Destroy();
		//}
		//if (CubeMap != nullptr)
		//{
		//	CubeMap->Destroy();
		//}
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

	/*static VkDescriptorImageInfo GetBRDFMapDescriptor()
	{
		if (BRDFTexture != nullptr)
		{
			VkDescriptorImageInfo BRDFMapDescriptor{};
			BRDFMapDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			BRDFMapDescriptor.imageView = BRDFTexture->GetView();
			BRDFMapDescriptor.sampler = BRDFTexture->GetSampler();
			return BRDFMapDescriptor;
		}

		return VulkanRenderer::GetNullDescriptor();
	}

	static VkDescriptorImageInfo GetIrradianceMapDescriptor()
	{
		if (IrradianceMap != nullptr)
		{
			VkDescriptorImageInfo IrradianceMapDescriptor{};
			IrradianceMapDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			IrradianceMapDescriptor.imageView = IrradianceMap->GetView();
			IrradianceMapDescriptor.sampler = IrradianceMap->GetSampler();
			return IrradianceMapDescriptor;
		}

		return VulkanRenderer::GetNullDescriptor();
	}

	static VkDescriptorImageInfo GetPrefilterMapDescriptor()
	{
		if (PrefilterMap != nullptr)
		{
			VkDescriptorImageInfo PrefilterMapDescriptor{};
			PrefilterMapDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			PrefilterMapDescriptor.imageView = PrefilterMap->GetView();
			PrefilterMapDescriptor.sampler = PrefilterMap->GetSampler();
			return PrefilterMapDescriptor;
		}

		return VulkanRenderer::GetNullDescriptor();
	}

	static VkDescriptorImageInfo GetCubeMapDescriptor()
	{
		if (CubeMap != nullptr)
		{
			VkDescriptorImageInfo CubeMapDescriptor{};
			CubeMapDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			CubeMapDescriptor.imageView = CubeMap->GetView();
			CubeMapDescriptor.sampler = CubeMap->GetSampler();
			return CubeMapDescriptor;
		}

		return VulkanRenderer::GetNullDescriptor();
	}*/

	static void DestroyScene()
	{
		GameObjectManager::DestroyScene();
		MeshRendererManager::DestroyScene();
		LightManager::DestroyScene();
	}

	static SceneType GetSceneType() { return sceneType; }
	//static float GetPBRCubeMapSize() { return PBRCubeMapSize; }
	//static float GetPreRenderedMapSize() { return PreRenderedMapSize; }
	static bool IsRayTracerActive() { return RayTracingActive; }
	static bool IsHybridRendererActive() { return HybridRendererActive; }
};

