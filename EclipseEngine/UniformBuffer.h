#pragma once
#include "VulkanBuffer.h"
#include "Material.h"

struct DirectionalLightBuffer {
	alignas(16) glm::vec3 position = glm::vec3(0.0f);
	alignas(16) glm::vec3 direction = glm::vec3(0.0f);
	alignas(16) glm::vec3 ambient = glm::vec3(0.8f);
	alignas(16) glm::vec3 diffuse = glm::vec3(1.0f);
	alignas(16) glm::vec3 specular = glm::vec3(1.0f);
	alignas(16) glm::mat4 LightSpaceMatrix = glm::mat4(1.0f);
};

struct PointLightBuffer {
	alignas(16) glm::vec3 position = glm::vec3(0.0f);
	alignas(16) glm::vec3 ambient = glm::vec3(0.8f);
	alignas(16) glm::vec3 diffuse = glm::vec3(1.0f);
	alignas(16) glm::vec3 specular = glm::vec3(1.0f);
	alignas(4) float constant = 1.0f;
	alignas(4) float linear = 0.022f;
	alignas(4) float quadratic = 0.0019f;
	alignas(16) glm::mat4 LightSpaceMatrix = glm::mat4(1.0f);
};

struct SpotLightBuffer {
	alignas(16) glm::vec3 position = glm::vec3(0.0f);
	alignas(16) glm::vec3 direction = glm::vec3(0.0f);
	alignas(16) glm::vec3 ambient = glm::vec3(0.8f);
	alignas(16) glm::vec3 diffuse = glm::vec3(1.0f);
	alignas(16) glm::vec3 specular = glm::vec3(1.0f);

	alignas(4) float cutOff = glm::cos(glm::radians(12.5f));
	alignas(4) float outerCutOff = glm::cos(glm::radians(15.0f));
	alignas(4) float constant = 1.0f;
	alignas(4) float linear = 0.022f;
	alignas(4) float quadratic = 0.0019f;
	alignas(16) glm::mat4 LightSpaceMatrix = glm::mat4(1.0f);
};

struct SceneProperties
{
	alignas(4)  uint32_t MeshIndex = 0;
	alignas(16) glm::mat4 proj = glm::mat4(1.0f);
	alignas(16) glm::mat4 view = glm::mat4(1.0f);
	alignas(16) glm::vec3 CameraPos = glm::vec3(0.0f);
	alignas(16) glm::vec3 MeshColorID = glm::vec3(0.0f);
	alignas(4)  float Timer = 0.0f;
};

struct RayTraceSceneProperties
{
	alignas(16) glm::mat4 proj = glm::mat4(1.0f);
	alignas(16) glm::mat4 view = glm::mat4(1.0f);
	alignas(16) glm::vec3 CameraPos = glm::vec3(0.0f);
	alignas(4)  float Timer;
};

struct MeshProperties
{
	alignas(16) glm::mat4 MeshTransform = glm::mat4(1.0f);
	alignas(16) glm::mat4 ModelTransform = glm::mat4(1.0f);
	MaterialBufferData materialBufferData;
};

template <class T>
class UniformBuffer
{
private: 
	T UniformDataInfo;
	VulkanBuffer VulkanBufferData;

public:
	UniformBuffer()
	{
		VulkanBufferData.CreateBuffer(nullptr, sizeof(T), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}

	UniformBuffer(T UniformData)
	{
		VulkanBufferData.CreateBuffer(nullptr, sizeof(T), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		Update(UniformData);
	}

	void Update()
	{
		VulkanBufferData.CopyBufferToMemory(&UniformDataInfo, sizeof(T));
	}

	void Update(T UniformData)
	{
		UniformDataInfo = UniformData;
		VulkanBufferData.CopyBufferToMemory(&UniformDataInfo, sizeof(T));
	}

	void Destroy()
	{
		VulkanBufferData.DestoryBuffer();
	}

	VulkanBuffer GetVulkanBufferData() { return VulkanBufferData; }
};

typedef UniformBuffer<MeshProperties> MeshPropertiesUniformBuffer;