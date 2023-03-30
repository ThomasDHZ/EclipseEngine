#pragma once
#include "UniformBuffer.h"
#include "RenderedColorTexture.h"
#include "RenderedCubeMapTexture.h"
#include "GLTFSunLight.h"
#include "GLTFDirectionalLight.h"
#include "GLTFSpotLight.h"
#include "GLTFPointLight.h"
#include "Skybox.h"
#
class GLTFSceneManager
{
private:
	static float PBRCubeMapSize;
	static float PreRenderedMapSize;

	static std::vector<std::shared_ptr<GLTFSunLight>> SunLightList;
	static std::vector<std::shared_ptr<GLTFDirectionalLight>> DirectionalLightList;
	static std::vector<std::shared_ptr<GLTFPointLight>> PointLightList;
	static std::vector<std::shared_ptr<GLTFSpotLight>> SpotLightList;


public:
	static SceneProperties sceneProperites;

	static std::shared_ptr<Camera> ActiveCamera;
	static std::shared_ptr<Skybox> SkyboxMesh;
	static SkyBoxView CubeMapInfo;
	static std::shared_ptr<EnvironmentTexture>     EnvironmentTexture;
	static std::shared_ptr<RenderedColorTexture>   BRDFTexture;
	static std::shared_ptr<RenderedCubeMapTexture> IrradianceMap;
	static std::shared_ptr<RenderedCubeMapTexture> PrefilterMap;
	static std::shared_ptr<RenderedCubeMapTexture> CubeMap;

	static void AddSunLight(std::shared_ptr<GLTFSunLight> sunLight);
	static void AddDirectionalLight(std::shared_ptr<GLTFDirectionalLight> directionalLight);
	static void AddPointlLight(std::shared_ptr<GLTFPointLight> pointLight);
	static void AddSpotLight(std::shared_ptr<GLTFSpotLight> spotLight);
	
	static void StartUp();
	static void Update();
	static void Destroy();

	static VkDescriptorImageInfo GetBRDFMapDescriptor();
	static VkDescriptorImageInfo GetIrradianceMapDescriptor();
	static VkDescriptorImageInfo GetPrefilterMapDescriptor();
	static VkDescriptorImageInfo GetCubeMapDescriptor();

	static std::vector<VkDescriptorBufferInfo> GetSunLightPropertiesBuffer();
	static std::vector<VkDescriptorBufferInfo> GetDirectionalLightPropertiesBuffer();
	static std::vector<VkDescriptorBufferInfo> GetPointLightPropertiesBuffer();
	static std::vector<VkDescriptorBufferInfo> GetSpotLightPropertiesBuffer();

	static std::vector<std::shared_ptr<GLTFSunLight>> GetSunLights() { return SunLightList; }
	static std::vector<std::shared_ptr<GLTFDirectionalLight>> GetDirectionalLights() { return DirectionalLightList; }
	static std::vector<std::shared_ptr<GLTFPointLight>> GetPointLights() { return PointLightList; }
	static std::vector<std::shared_ptr<GLTFSpotLight>> GetSpotLights() { return SpotLightList; }
	static uint32_t GetSunLightCount() { return SunLightList.size(); }
	static uint32_t GetDirectionalCount() { return DirectionalLightList.size(); }
	static uint32_t GetPointLightCount() { return PointLightList.size(); }
	static uint32_t GetSpotLightCount() { return SpotLightList.size(); }
	static float GetPBRCubeMapSize() { return PBRCubeMapSize; }
	static float GetPreRenderedMapSize() { return PreRenderedMapSize; }
};

