#pragma once
#include "VulkanBuffer.h"
#include "Material.h"
struct SceneProperties
{
	alignas(4) uint32_t MeshIndex = 0;
	alignas(16) glm::mat4 proj = glm::mat4(1.0f);
	alignas(16) glm::mat4 view = glm::mat4(1.0f);
	alignas(16) glm::vec3 CameraPos = glm::vec3(0.0f);
	alignas(16) glm::vec3 MeshColor = glm::vec3(1.0f, 0.0f, 0.0f);
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
	}

	UniformBuffer(T UniformData)
	{
		Update(UniformData);
	}

	void Update(T UniformData)
	{
		if (VulkanBufferData.GetBuffer() == nullptr)
		{
			VulkanBufferData.CreateBuffer(nullptr, sizeof(T), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		}

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