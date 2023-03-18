#pragma once
#include "UniformBuffer.h"
#include "RenderedColorTexture.h"
#include "RenderedCubeMapTexture.h"
#include "GLTFSunLight.h"
#include "GLTFDirectionalLight.h"
#include "GLTFSpotLight.h"
#include "GLTFPointLight.h"
#include "SkyboxMesh.h"
#include "GLTFSceneManager.h"

class GLTFSceneManager
{
private:
	static float PBRCubeMapSize;
	static float PreRenderedMapSize;

	//static std::vector<std::shared_ptr<GLTFSunLight>> SunLightList;
	//static std::vector<std::shared_ptr<GLTFDirectionalLight>> DirectionalLightList;
	//static std::vector<std::shared_ptr<GLTFPointLight>> PointLightList;
	//static std::vector<std::shared_ptr<GLTFSpotLight>> SpotLightList;

	//static std::vector<VkDescriptorBufferInfo> SunLightPropertiesBuffer;
	//static std::vector<VkDescriptorBufferInfo> DirectionalLightPropertiesBuffer;
	//static std::vector<VkDescriptorBufferInfo> PointLightPropertiesBuffer;
	//static std::vector<VkDescriptorBufferInfo> SpotLightPropertiesBuffer;

	//static std::shared_ptr<SkyboxMesh> SkyboxMesh;

public:
	static SceneProperties sceneProperites;

	static std::shared_ptr<SkyboxMesh> skyboxMesh;
	static ConstSkyBoxView CubeMapInfo;
	static std::shared_ptr<EnvironmentTexture>     EnvironmentTexture;
	static std::shared_ptr<RenderedColorTexture>   BRDFTexture;
	static std::shared_ptr<RenderedCubeMapTexture> IrradianceMap;
	static std::shared_ptr<RenderedCubeMapTexture> PrefilterMap;
	static std::shared_ptr<RenderedCubeMapTexture> CubeMap;
	//static void AddGameObject();
	static void AddSunLight(GLTFSunLight sunLight);
	static void AddDirectionalLight(GLTFDirectionalLight directionalLight);
	static void AddPointlLight(GLTFPointLight pointLight);
	static void AddSpotLight(GLTFSpotLight spotLight);
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

