#pragma once
#include "VulkanBuffer.h"
#include "Material.h"

struct DirectionalLightBuffer {
	alignas(16) glm::vec3 position = glm::vec3(0.0f);
	alignas(16) glm::vec3 direction = glm::vec3(0.00001f);
	alignas(16) glm::vec3 diffuse = glm::vec3(1.0f);
	alignas(16) glm::vec3 specular = glm::vec3(1.0f);
	alignas(16) glm::mat4 LightSpaceMatrix = glm::mat4(1.0f);
};

struct PointLightBuffer {
	alignas(16) glm::vec3 position = glm::vec3(0.0f);
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
	alignas(16) glm::vec3 diffuse = glm::vec3(1.0f);
	alignas(16) glm::vec3 specular = glm::vec3(1.0f);

	alignas(4) float cutOff = glm::cos(glm::radians(12.5f));
	alignas(4) float outerCutOff = glm::cos(glm::radians(15.0f));
	alignas(4) float constant = 1.0f;
	alignas(4) float linear = 0.022f;
	alignas(4) float quadratic = 0.0019f;
	alignas(16) glm::mat4 LightSpaceMatrix = glm::mat4(1.0f);
};

struct IrradianceSkyboxSettings
{
	alignas(4) float IrradianceSampleDelta = 0.075f;
};

struct PrefilterSkyboxSettings
{
	alignas(4) uint32_t SkyboxSize = 0.0f;
	alignas(4) float roughness = 0.0f;
};

struct SceneProperties
{
	alignas(4)  uint32_t MeshIndex = 0;
	alignas(16) glm::mat4 proj = glm::mat4(1.0f);
	alignas(16) glm::mat4 view = glm::mat4(1.0f);
	alignas(16) glm::vec3 CameraPos = glm::vec3(0.0f);
	alignas(16) glm::vec3 MeshColorID = glm::vec3(0.0f);
	alignas(16) glm::vec3 AmbientLight = glm::vec3(0.1f);
	alignas(4)  uint32_t DirectionalLightCount;
	alignas(4)  uint32_t PointLightCount;
	alignas(4)  uint32_t SpotLightCount;
	alignas(4)  float Timer = 0.0f;
	alignas(4)  float PBRMaxMipLevel = 0.0f;
};

struct BillboardSceneProperties : public SceneProperties
{
};

struct DirectionalLightProjection
{
	alignas(4) uint32_t MeshIndex;
	alignas(4) uint32_t LightIndex;
};

struct ConstSkyBoxView
{
	alignas(16) glm::mat4 proj = glm::mat4(1.0f);
	alignas(16) glm::mat4 view = glm::mat4(1.0f);
};

struct RayTraceSceneProperties
{
	alignas(16) glm::mat4 proj = glm::mat4(1.0f);
	alignas(16) glm::mat4 view = glm::mat4(1.0f);
	alignas(16) glm::vec3 CameraPos = glm::vec3(0.0f);
	alignas(4)  float Timer;
};

struct CubeMapSampler
{
	alignas(16) glm::mat4 CubeMapFaceMatrix[6];
};

struct MeshProperties
{
	MaterialBufferData materialBufferData;
	alignas(16) glm::mat4 MeshTransform = glm::mat4(1.0f);
	alignas(16) glm::mat4 MeshReflectionMatrix[6];
	alignas(8) glm::vec2 UVOffset = glm::vec2(0.0f);
	alignas(8) glm::vec2 UVScale = glm::vec2(1.0f);
	alignas(8) glm::vec2 UVFlip = glm::vec2(0.0f);
	alignas(4) int SelectedObjectBufferIndex = 0;
	alignas(4) float heightScale = 0.1f;
	alignas(4) float minLayers = 8;
	alignas(4) float maxLayers = 32;

	MeshProperties()
	{
		for (int x = 0; x <= 5; x++)
		{
			MeshReflectionMatrix[x] = glm::mat4(1.0f);
		}
	}
	void from_json(nlohmann::json& json)
	{
		materialBufferData.from_json(json["materialBufferData"]);
		JsonConverter::from_json(json["MeshTransform"], MeshTransform);
		JsonConverter::from_json(json["UVOffset"], UVOffset);
		JsonConverter::from_json(json["UVScale"], UVScale);
		JsonConverter::from_json(json["UVFlip"], UVFlip);
		JsonConverter::from_json(json["heightScale"], heightScale);
		JsonConverter::from_json(json["minLayers"], minLayers);
		JsonConverter::from_json(json["maxLayers"], maxLayers);
	}

	void to_json(nlohmann::json& json)
	{
		materialBufferData.to_json(json["materialBufferData"]);
		JsonConverter::to_json(json["MeshTransform"], MeshTransform);
		JsonConverter::to_json(json["UVOffset"], UVOffset);
		JsonConverter::to_json(json["UVScale"], UVScale);
		JsonConverter::to_json(json["UVFlip"], UVFlip);
		JsonConverter::to_json(json["heightScale"], heightScale);
		JsonConverter::to_json(json["minLayers"], minLayers);
		JsonConverter::to_json(json["maxLayers"], maxLayers);
	}
};

template <class T>
class UniformBuffer
{
private: 
	VulkanBuffer VulkanBufferData;

public:

	T UniformDataInfo;

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
typedef UniformBuffer<CubeMapSampler> CubeMapSamplerBuffer;