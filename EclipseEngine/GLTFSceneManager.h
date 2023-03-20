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



	//static std::shared_ptr<SkyboxMesh> SkyboxMesh;

public:
	static SceneProperties sceneProperites;

	static std::vector<std::shared_ptr<GLTFSunLight>> SunlLightList;
	static std::vector<std::shared_ptr<GLTFDirectionalLight>> DirectionalLightList;
	static std::vector<std::shared_ptr<GLTFPointLight>> PointlLightList;
	static std::vector<std::shared_ptr<GLTFSpotLight>> SpotLightList;

	static std::vector<VkDescriptorBufferInfo> SunLightPropertiesBuffer;
	static std::vector<VkDescriptorBufferInfo> DirectionalLightPropertiesBuffer;
	static std::vector<VkDescriptorBufferInfo> PointLightPropertiesBuffer;
	static std::vector<VkDescriptorBufferInfo> SpotLightPropertiesBuffer;

	static std::shared_ptr<Skybox> skyboxMesh;
	static SkyBoxView CubeMapInfo;
	static std::shared_ptr<EnvironmentTexture>     EnvironmentTexture;
	static std::shared_ptr<RenderedColorTexture>   BRDFTexture;
	static std::shared_ptr<RenderedCubeMapTexture> IrradianceMap;
	static std::shared_ptr<RenderedCubeMapTexture> PrefilterMap;
	static std::shared_ptr<RenderedCubeMapTexture> CubeMap;
	//static void AddGameObject();
	static void AddSunLight(std::shared_ptr<GLTFSunLight> sunLight);
	static void AddDirectionalLight(std::shared_ptr<GLTFDirectionalLight> directionalLight);
	static void AddPointlLight(std::shared_ptr<GLTFPointLight> pointLight);
	static void AddSpotLight(std::shared_ptr<GLTFSpotLight> spotLight);
	static void Update();
	static VkDescriptorImageInfo GetBRDFMapDescriptor();
	static VkDescriptorImageInfo GetIrradianceMapDescriptor();
	static VkDescriptorImageInfo GetPrefilterMapDescriptor();
	static VkDescriptorImageInfo GetCubeMapDescriptor();
	//static void UpdateSunLightPropertiesBuffer();
	//static void UpdateDirectionalLightPropertiesBuffer();
	//static void UpdatePointLightPropertiesBuffer();
	//static void UpdateSpotLightPropertiesBuffer();

	//static std::vector<VkDescriptorBufferInfo> GetSunLightPropertiesBuffer() { return SunLightPropertiesBuffer; }
	//static std::vector<VkDescriptorBufferInfo> GetDirectionalLightPropertiesBuffer() { return DirectionalLightPropertiesBuffer; }
	//static std::vector<VkDescriptorBufferInfo> GetPointLightPropertiesBuffer() { return PointLightPropertiesBuffer; }
	//static std::vector<VkDescriptorBufferInfo> GetSpotLightPropertiesBuffer() { return SpotLightPropertiesBuffer; }
};

