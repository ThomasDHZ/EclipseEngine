#pragma once
#include "UniformBuffer.h"
#include "RenderedColorTexture.h"
#include "RenderedCubeMapTexture.h"
#include "GLTFSunLight.h"
#include "GLTFDirectionalLight.h"
#include "GLTFSpotLight.h"
#include "GLTFPointLight.h"
#include "Skybox.h"
#include "GLTFMaterial.h"
#include "GameObject3D.h"

class GLTFSceneManager
{
private:
	static float PBRCubeMapSize;
	static float PreRenderedMapSize;
	static std::vector<std::shared_ptr<GameObject>> GameObjectList;
	static std::vector<std::shared_ptr<GLTFMaterial>> MaterialList;
	static std::vector<std::shared_ptr<Texture2D>> Texture2DList;
	static std::vector<std::shared_ptr<GLTFSunLight>> SunLightList;
	static std::vector<std::shared_ptr<GLTFDirectionalLight>> DirectionalLightList;
	static std::vector<std::shared_ptr<GLTFPointLight>> PointLightList;
	static std::vector<std::shared_ptr<GLTFSpotLight>> SpotLightList;

	static void UpdateBufferIndex();
	static std::shared_ptr<Texture2D> IsTexture2DLoaded(std::string name);
	static std::shared_ptr<Texture2D> IsTexture2DLoaded(const GLTFTextureLoader& textureLoader);

public:
	static bool WireframeModeFlag;
	static SceneProperties sceneProperites;

	static std::shared_ptr<Camera> ActiveCamera;
	static std::shared_ptr<Skybox> SkyboxMesh;
	static SkyBoxView CubeMapInfo;
	static std::shared_ptr<EnvironmentTexture>     EnvironmentTexture;
	static std::shared_ptr<RenderedColorTexture>   BRDFTexture;
	static std::shared_ptr<RenderedCubeMapTexture> IrradianceMap;
	static std::shared_ptr<RenderedCubeMapTexture> PrefilterMap;
	static std::shared_ptr<RenderedCubeMapTexture> CubeMap;

	template <class T>
	static void AddGameObject(const std::string Name, const std::string FilePath, GameObjectRenderType renderType)
	{
		GameObjectList.emplace_back(std::make_shared<GameObject3D>(GameObject3D(Name, renderType)));
		GameObjectList.back()->LoadRenderObject<T>(FilePath);
	}
	
	template <class T>
	static void AddGameObject(const std::string Name, const std::string FilePath, GameObjectRenderType renderType, const glm::vec3& position)
	{
		GameObjectList.emplace_back(std::make_shared<GameObject3D>(GameObject3D(Name, renderType)));
		GameObjectList.back()->LoadRenderObject<T>(FilePath);
	}
	
	template <class T>
	static void AddGameObject(const std::string Name, const std::string FilePath, GameObjectRenderType renderType, const glm::vec3& position, const glm::vec3& rotation)
	{
		GameObjectList.emplace_back(std::make_shared<GameObject3D>(GameObject3D(Name, renderType)));
		GameObjectList.back()->LoadRenderObject<T>(FilePath);
	}
	
	template <class T>
	static void AddGameObject(const std::string Name, const std::string FilePath, GameObjectRenderType renderType, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	{
		GameObjectList.emplace_back(std::make_shared<GameObject3D>(GameObject3D(Name, renderType)));
		GameObjectList.back()->LoadRenderObject<T>(FilePath);
	}

	static std::shared_ptr<Texture2D> LoadTexture2D(std::shared_ptr<Texture2D> texture);
	static std::shared_ptr<Texture2D> LoadTexture2D(GLTFTextureLoader& textureLoader);
	static void AddMaterial(const std::shared_ptr<GLTFMaterial> material);
	static void AddSunLight(std::shared_ptr<GLTFSunLight> sunLight);
	static void AddDirectionalLight(std::shared_ptr<GLTFDirectionalLight> directionalLight);
	static void AddPointLight(std::shared_ptr<GLTFPointLight> pointLight);
	static void AddSpotLight(std::shared_ptr<GLTFSpotLight> spotLight);
	static void AddSunLight(GLTFSunLightLoader& sunLight);
	static void AddDirectionalLight(GLTFDirectionalLightLoader& directionalLight);
	static void AddPointLight(GLTFPointLightLoader& pointLight);
	static void AddSpotLight(GLTFSpotLightLoader& spotLight);

	static void StartUp();
	static void Update();
	static void Destroy();

	static VkDescriptorImageInfo GetBRDFMapDescriptor();
	static VkDescriptorImageInfo GetIrradianceMapDescriptor();
	static VkDescriptorImageInfo GetPrefilterMapDescriptor();
	static VkDescriptorImageInfo GetCubeMapDescriptor();
	static VkDescriptorImageInfo GetEnvironmentMapDescriptor();

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

