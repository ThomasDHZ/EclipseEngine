#pragma once
#include "VulkanBuffer.h"
#include "Material.h"
struct SceneProperties
{
	alignas(4) uint32_t MeshIndex;
	alignas(16) glm::mat4 proj;
	alignas(16) glm::mat4 view;
	alignas(16) glm::vec3 CameraPos;
	alignas(4)  float Timer;
};

struct MeshProperties
{
	MaterialBufferData materialBufferData;
	alignas(16) glm::mat4 MeshTransform = glm::mat4(1.0f);
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