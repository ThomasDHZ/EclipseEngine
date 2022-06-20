#pragma once
#include "VulkanRenderer.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class LightManager
{
private:
	static std::vector<std::shared_ptr<DirectionalLight>> DirectionalLightList;
	static std::vector<std::shared_ptr<PointLight>> PointLightList;
	static std::vector<std::shared_ptr<SpotLight>> SpotLightList;

public:
	static void StartUp()
	{
	}

	static void Update()
	{
		for (auto& directionalLight : DirectionalLightList)
		{
			directionalLight->Update();
		}
		for (auto& pointLight : PointLightList)
		{
			pointLight->Update();
		}
		for (auto& spotLight : SpotLightList)
		{
			spotLight->Update();
		}
	}

	static void GUIUpdate()
	{
		for (int x = 0; x < DirectionalLightList.size(); x++)
		{
			ImGui::SliderFloat3(("DLight position " + std::to_string(x)).c_str(), &DirectionalLightList[x]->GetPositionPtr()->x, -100.0f, 2000.0f);
			ImGui::SliderFloat3(("DLight direction " + std::to_string(x)).c_str(), &DirectionalLightList[x]->GetDirectionPtr()->x, -1.0f, 1.0f);
			ImGui::SliderFloat3(("DLight Diffuse " + std::to_string(x)).c_str(), &DirectionalLightList[x]->GetDiffusePtr()->x, 0.0f, 1.0f);
			ImGui::SliderFloat3(("DLight specular " + std::to_string(x)).c_str(), &DirectionalLightList[x]->GetSpecularPtr()->x, 0.0f, 1.0f);
		}
		for (int x = 0; x < PointLightList.size(); x++)
		{
			ImGui::SliderFloat3(("PLight position " + std::to_string(x)).c_str(), &PointLightList[x]->GetPositionPtr()->x, -100.0f, 2000.0f);
			ImGui::SliderFloat3(("PLight Diffuse " + std::to_string(x)).c_str(), &PointLightList[x]->GetDiffusePtr()->x, 0.0f, 1.0f);
			ImGui::SliderFloat3(("PLight specular " + std::to_string(x)).c_str(), &PointLightList[x]->GetSpecularPtr()->x, 0.0f, 1.0f);
		}
		for (int x = 0; x < SpotLightList.size(); x++)
		{
			ImGui::SliderFloat3(("SLight position " + std::to_string(x)).c_str(), &SpotLightList[x]->GetPositionPtr()->x, -100.0f, 2000.0f);
			ImGui::SliderFloat3(("SLight direction " + std::to_string(x)).c_str(), &SpotLightList[x]->GetDirectionPtr()->x, -1.0f, 1.0f);
			ImGui::SliderFloat3(("SLight Diffuse " + std::to_string(x)).c_str(), &SpotLightList[x]->GetDiffusePtr()->x, 0.0f, 1.0f);
			ImGui::SliderFloat3(("SLight specular " + std::to_string(x)).c_str(), &SpotLightList[x]->GetSpecularPtr()->x, 0.0f, 1.0f);
		}
	}

	static void Destory()
	{
		for (auto& directionalLight : DirectionalLightList)
		{
			directionalLight->Destroy();
		}
		for (auto& pointLight : PointLightList)
		{
			pointLight->Destroy();
		}
		for (auto& spotLight : SpotLightList)
		{
			spotLight->Destroy();
		}
	}

	static void AddDirectionalLight()
	{
		auto directionalLight = std::make_shared<DirectionalLight>();
		DirectionalLightList.emplace_back(directionalLight);
		VulkanRenderer::UpdateRendererFlag = true;
	}

	static void AddDirectionalLight(std::shared_ptr<DirectionalLight> light)
	{
		DirectionalLightList.emplace_back(light);
		VulkanRenderer::UpdateRendererFlag = true;
	}

	static void AddDirectionalLight(DirectionalLightBuffer light)
	{
		auto directionalLight = std::make_shared<DirectionalLight>(DirectionalLight(light));
		DirectionalLightList.emplace_back(directionalLight);
		VulkanRenderer::UpdateRendererFlag = true;
	}

	static void AddPointLight()
	{
		auto pointLight = std::make_shared<PointLight>();
		PointLightList.emplace_back(pointLight);
		VulkanRenderer::UpdateRendererFlag = true;
	}

	static void AddPointLight(std::shared_ptr<PointLight> light)
	{
		PointLightList.emplace_back(light);
		VulkanRenderer::UpdateRendererFlag = true;
	}

	static void AddPointLight(PointLightBuffer light)
	{
		auto pointLight = std::make_shared<PointLight>(PointLight(light));
		PointLightList.emplace_back(pointLight);
		VulkanRenderer::UpdateRendererFlag = true;
	}

	static void AddSpotLight()
	{
		auto spotLight = std::make_shared<SpotLight>();
		SpotLightList.emplace_back(spotLight);
		VulkanRenderer::UpdateRendererFlag = true;
	}

	static void AddSpotLight(std::shared_ptr<SpotLight> light)
	{
		SpotLightList.emplace_back(light);
		VulkanRenderer::UpdateRendererFlag = true;
	}

	static void AddSpotLight(SpotLightBuffer light)
	{
		auto spotLight = std::make_shared<SpotLight>(SpotLight(light));
		SpotLightList.emplace_back(spotLight);
		VulkanRenderer::UpdateRendererFlag = true;
	}

	static void DeleteDirectionalLight(uint32_t LightBufferIndex)
	{
		DirectionalLightList[LightBufferIndex]->Destroy();
		DirectionalLightList.erase(DirectionalLightList.begin() + LightBufferIndex);
		VulkanRenderer::UpdateRendererFlag = true;
	}

	static void DeletePointLight(uint32_t LightBufferIndex)
	{
		PointLightList[LightBufferIndex]->Destroy();
		PointLightList.erase(PointLightList.begin() + LightBufferIndex);
		VulkanRenderer::UpdateRendererFlag = true;
	}

	static void DeleteSpotLight(uint32_t LightBufferIndex)
	{
		SpotLightList[LightBufferIndex]->Destroy();
		SpotLightList.erase(SpotLightList.begin() + LightBufferIndex);
		VulkanRenderer::UpdateRendererFlag = true;
	}

	static std::vector<VkDescriptorBufferInfo> GetDirectionalLightBuffer()
	{
		std::vector<VkDescriptorBufferInfo> DirectionalLightBufferList{};

		if (VulkanRenderer::UpdateRendererFlag)
		{
			if (DirectionalLightList.size() == 0)
			{
				VkDescriptorBufferInfo nullBuffer;
				nullBuffer.buffer = VK_NULL_HANDLE;
				nullBuffer.offset = 0;
				nullBuffer.range = VK_WHOLE_SIZE;
				DirectionalLightBufferList.emplace_back(nullBuffer);
			}
			else
			{
				for (auto& light : DirectionalLightList)
				{
					light->GetLightPropertiesBuffer(DirectionalLightBufferList);
				}
			}
		}
		else
		{
			std::cout << "Can't update DirectionalLightBuffer unless pipelines in the process of being rebuild." << std::endl;
		}
		return DirectionalLightBufferList;
	}

	static std::vector<VkDescriptorBufferInfo> GetPointLightBuffer()
	{
		std::vector<VkDescriptorBufferInfo> PointLightBufferList{};

		if (VulkanRenderer::UpdateRendererFlag)
		{
			if (PointLightList.size() == 0)
			{
				VkDescriptorBufferInfo nullBuffer;
				nullBuffer.buffer = VK_NULL_HANDLE;
				nullBuffer.offset = 0;
				nullBuffer.range = VK_WHOLE_SIZE;
				PointLightBufferList.emplace_back(nullBuffer);
			}
			else
			{
				for (auto& light : PointLightList)
				{
					light->GetLightPropertiesBuffer(PointLightBufferList);
				}
			}
		}
		else
		{
			std::cout << "Can't update PointLightBuffer unless pipelines in the process of being rebuild." << std::endl;
		}
		return PointLightBufferList;
	}

	static std::vector<VkDescriptorBufferInfo> GetSpotLightBuffer()
	{
		std::vector<VkDescriptorBufferInfo> SpotLightBufferList{};

		if (VulkanRenderer::UpdateRendererFlag)
		{
			if (SpotLightList.size() == 0)
			{
				VkDescriptorBufferInfo nullBuffer;
				nullBuffer.buffer = VK_NULL_HANDLE;
				nullBuffer.offset = 0;
				nullBuffer.range = VK_WHOLE_SIZE;
				SpotLightBufferList.emplace_back(nullBuffer);
			}
			else
			{
				for (auto& light : SpotLightList)
				{
					light->GetLightPropertiesBuffer(SpotLightBufferList);
				}
			}
		}
		else
		{
			std::cout << "Can't update SpotLight unless pipelines in the process of being rebuild." << std::endl;
		}
		return SpotLightBufferList;
	}

	static void LoadLights(nlohmann::json& json)
	{
		for (int x = 0; x < json["LightList"]["DirectionalLightList"].size(); x++)
		{
			std::shared_ptr<DirectionalLight> directionalLight = std::make_shared<DirectionalLight>(DirectionalLight(json["LightList"].at("DirectionalLightList")[x]));
			DirectionalLightList.emplace_back(directionalLight);
		}
		for (int x = 0; x < json["LightList"]["PointLightList"].size(); x++)
		{
			std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>(PointLight(json["LightList"].at("PointLightList")[x]));
			PointLightList.emplace_back(pointLight);
		}
		for (int x = 0; x < json["LightList"]["SpotLightList"].size(); x++)
		{
			std::shared_ptr<SpotLight> spotLight = std::make_shared<SpotLight>(SpotLight(json["LightList"].at("SpotLightList")[x]));
			SpotLightList.emplace_back(spotLight);
		}

		VulkanRenderer::UpdateRendererFlag = true;
	}

	static nlohmann::json SaveLights()
	{
		nlohmann::json json;
		for (int x = 0; x < DirectionalLightList.size(); x++)
		{
			DirectionalLightList[x]->to_json(json["DirectionalLightList"][x]);
		}
		for (int x = 0; x < PointLightList.size(); x++)
		{
			PointLightList[x]->to_json(json["PointLightList"][x]);
		}
		for (int x = 0; x < SpotLightList.size(); x++)
		{
			SpotLightList[x]->to_json(json["SpotLightList"][x]);
		}
		return json;
	}

	static void DestroyScene()
	{
		for (int x = DirectionalLightList.size() - 1; x >= 0; x--)
		{
			DirectionalLightList[x]->Destroy();
			DirectionalLightList.erase(DirectionalLightList.begin() + x);
		}
		for (int x = PointLightList.size() - 1; x >= 0; x--)
		{
			PointLightList[x]->Destroy();
			PointLightList.erase(PointLightList.begin() + x);
		}
		for (int x = SpotLightList.size() - 1; x >= 0; x--)
		{
			SpotLightList[x]->Destroy();
			SpotLightList.erase(SpotLightList.begin() + x);
		}
	}

	static std::vector<std::shared_ptr<DirectionalLight>> GetDirectionalLights() { return DirectionalLightList; }
	static std::vector<std::shared_ptr<PointLight>> GetPointLights() { return PointLightList; }
	static std::vector<std::shared_ptr<SpotLight>> GetSpotLLights() { return SpotLightList; }
	static uint32_t GetDirectionalLightCount() { return DirectionalLightList.size(); }
	static uint32_t GetPointLightCount() { return PointLightList.size(); }
	static uint32_t GetSpotLightCount() { return SpotLightList.size(); }
};

