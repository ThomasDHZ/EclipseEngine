#pragma once
#include "VulkanBuffer.h"

struct SkyBoxView
{
	alignas(16) glm::mat4 proj = glm::mat4(1.0f);
	alignas(16) glm::mat4 view = glm::mat4(1.0f);
};

struct GLTFSunLightBuffer {
	alignas(16) glm::vec3 diffuse = glm::vec3(0.0f);
	alignas(16) glm::vec3 position = glm::vec3(0.00001f);
	alignas(16) glm::mat4 LightSpaceMatrix = glm::mat4(1.0f);
	alignas(4) float intensity = 1.0f;
};

struct GLTFDirectionalLightBuffer {
	alignas(16) glm::vec3 diffuse = glm::vec3(0.0f);
	alignas(16) glm::vec3 direction = glm::vec3(0.00001f);
	alignas(16) glm::mat4 LightSpaceMatrix = glm::mat4(1.0f);
	alignas(4) float intensity = 1.0f;
};

struct GLTFPointLightBuffer {
	alignas(16) glm::vec3 diffuse = glm::vec3(0.0f);
	alignas(16) glm::vec3 position = glm::vec3(0.00001f);
	alignas(16) glm::mat4 LightSpaceMatrix = glm::mat4(1.0f);
	alignas(4) float intensity = 1.0f;
	alignas(4) float radius = 1.0f;;
};

struct GLTFSpotLightBuffer {
	alignas(16) glm::vec3 diffuse = glm::vec3(0.0f);
	alignas(16) glm::vec3 position = glm::vec3(0.00001f);
	alignas(16) glm::vec3 direction = glm::vec3(0.00001f);
	alignas(16) glm::mat4 LightSpaceMatrix = glm::mat4(1.0f);
	alignas(4) float intensity = 1.0f;
	alignas(4) float radius = 1.0f;
	alignas(4) float cutOff = glm::cos(glm::radians(12.5f));
	alignas(4) float outerCutOff = glm::cos(glm::radians(15.0f));
	alignas(4) float constant = 1.0f;
	alignas(4) float linear = 0.022f;
	alignas(4) float quadratic = 0.0019f;
};

struct IrradianceSkyboxSettings
{
	alignas(4) uint32_t CubeMapId = 0;
	alignas(4) float IrradianceSampleDelta = 0.075f;
};

struct PrefilterSkyboxSettings
{
	alignas(4) uint32_t CubeMapId = 0;
	alignas(4) uint32_t SkyboxSize = 0.0f;
	alignas(4) float roughness = 0.0f;
};

struct BakeSceneProperties
{
	alignas(4)  uint32_t Counter = 0;
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

struct SceneProperties
{
	alignas(4)  uint32_t MeshIndex = 0;
	alignas(4)  uint32_t PrimitiveIndex = 0;
	alignas(4)  uint32_t MaterialIndex = 0;
	alignas(4)  uint32_t ReflectionIndex = 0;
	alignas(16) glm::mat4 proj = glm::mat4(1.0f);
	alignas(16) glm::mat4 view = glm::mat4(1.0f);
	alignas(16) glm::vec3 CameraPos = glm::vec3(0.0f);
	alignas(16) glm::vec3 MeshColorID = glm::vec3(0.0f);
	alignas(16) glm::vec3 AmbientLight = glm::vec3(0.1f);
	alignas(4)  uint32_t SunLightCount;
	alignas(4)  uint32_t DirectionalLightCount;
	alignas(4)  uint32_t PointLightCount;
	alignas(4)  uint32_t SpotLightCount;
	alignas(4)  float Timer = 0.0f;
	alignas(4)  float PBRMaxMipLevel = 0.0f;
	alignas(4)  uint32_t frame;
	alignas(4)  int MaxReflectCount;
};

struct GaussianBlurSettings
{
	alignas(4) float blurScale = 1.0f;
	alignas(4) float blurStrength = 1.0f;
};

struct BloomIndexSettings
{
	alignas(4) uint32_t TextureCount = 0;
};

struct BillboardSceneProperties : public SceneProperties
{
};

struct LightViewSceneData
{
	alignas(4) uint32_t MeshIndex;
	alignas(4) uint32_t MaterialIndex;
	alignas(4) uint32_t LightIndex;
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
	alignas(4) uint32_t VertexBufferIndex = 0;
	alignas(4) uint32_t IndiceBufferIndex = 0;
	alignas(4) uint32_t MaterialBufferIndex = 0;
	alignas(4) uint32_t AlbedoMap = -1;
	alignas(4) uint32_t MetallicRoughnessMap = -1;
	alignas(4) uint32_t AmbientOcclusionMap = -1;
	alignas(4) uint32_t NormalMap = -1;
	alignas(4) uint32_t DepthMap = -1;
	alignas(4) uint32_t AlphaMap = -1;
	alignas(4) uint32_t EmissionMap = -1;
	alignas(4) uint32_t SkyBoxIndex = -1;
	alignas(16) glm::mat4 MeshTransform = glm::mat4(1.0f);
	alignas(8) glm::vec2 UVOffset = glm::vec2(0.0f);
	alignas(8) glm::vec2 UVScale = glm::vec2(1.0f);
	alignas(8) glm::vec2 UVFlip = glm::vec2(0.0f);
	alignas(4) int SelectedObjectBufferIndex = 0;
	alignas(4) float heightScale = 0.1f;
	alignas(4) float minLayers = 8;
	alignas(4) float maxLayers = 32;
};

struct InstanceMeshProperties
{
	alignas(4) uint32_t MaterialBufferIndex;
};

template <class T>
class UniformBuffer
{
private:


public:
	VulkanBuffer VulkanBufferData;
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
		VulkanBufferData.DestroyBuffer();
	}

	VulkanBuffer GetVulkanBufferData() { return VulkanBufferData; }
	VulkanBuffer* GetVulkanBufferDataPtr() { return &VulkanBufferData; }
};

typedef UniformBuffer<MeshProperties> MeshPropertiesUniformBuffer;
typedef UniformBuffer<CubeMapSampler> CubeMapSamplerBuffer;