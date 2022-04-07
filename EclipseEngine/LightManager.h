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
};

