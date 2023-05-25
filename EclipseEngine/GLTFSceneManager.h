#pragma once
#include "UniformBuffer.h"
#include "RenderedColorTexture.h"
#include "RenderedCubeMapTexture.h"
#include "EnvironmentTexture.h"
#include "GLTFSunLight.h"
#include "GLTFDirectionalLight.h"
#include "GLTFSpotLight.h"
#include "GLTFPointLight.h"
#include "Skybox.h"
#include "Material.h"
#include "GameObject3D.h"
#include "SpriteGameObject2D.h"
#include "SpriteGameObject3D.h"
#include "LineGameObject3D.h"

class GLTFSceneManager
{
private:
	static float PBRCubeMapSize;
	static float PreRenderedMapSize;

	static std::vector<std::shared_ptr<Material>> MaterialList;
	static std::vector<std::shared_ptr<Texture>> TextureList;
	static std::vector<std::shared_ptr<GLTFSunLight>> SunLightList;
	static std::vector<std::shared_ptr<GLTFDirectionalLight>> DirectionalLightList;
	static std::vector<std::shared_ptr<GLTFPointLight>> PointLightList;
	static std::vector<std::shared_ptr<GLTFSpotLight>> SpotLightList;

	std::vector<VkDescriptorImageInfo> TexturePropertiesBuffer;
	std::vector<VkDescriptorBufferInfo> MaterialPropertiesBuffer;

	static std::shared_ptr<Texture> IsTexture2DLoaded(std::string name);
	static std::shared_ptr<Texture> IsTexture2DLoaded(const GLTFTextureLoader& textureLoader);

public:
	static bool WireframeModeFlag;
	static SceneProperties sceneProperites;

	static std::vector<std::shared_ptr<GameObject>> GameObjectList;
	static std::shared_ptr<Camera> ActiveCamera;
	static std::shared_ptr<Skybox> SkyboxMesh;
	static std::shared_ptr<EnvironmentTexture>     EnvironmentTexture;
	static std::shared_ptr<RenderedColorTexture>   BRDFTexture;
	static std::shared_ptr<RenderedCubeMapTexture> IrradianceMap;
	static std::shared_ptr<RenderedCubeMapTexture> PrefilterMap;
	static std::shared_ptr<RenderedCubeMapTexture> CubeMap;

	static void AddMeshGameObject3D(const std::string Name, const std::string FilePath);
	static void AddMeshGameObject3D(const std::string Name, const std::string FilePath, const glm::vec3& position);
	static void AddMeshGameObject3D(const std::string Name, const std::string FilePath, const glm::vec3& position, const glm::vec3& rotation);
	static void AddMeshGameObject3D(const std::string Name, const std::string FilePath, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
	static void AddMeshGameObject3D(const std::string Name, const std::string FilePath, GLTFInstancingDataStruct& instanceData, GameObjectRenderType renderType);
	static void AddMeshGameObject3D(const std::string Name, const std::string FilePath, GLTFInstancingDataStruct& instanceData, GameObjectRenderType renderType, const glm::vec3& position);
	static void AddMeshGameObject3D(const std::string Name, const std::string FilePath, GLTFInstancingDataStruct& instanceData, GameObjectRenderType renderType, const glm::vec3& position, const glm::vec3& rotation);
	static void AddMeshGameObject3D(const std::string Name, const std::string FilePath, GLTFInstancingDataStruct& instanceData, GameObjectRenderType renderType, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	static void AddSpriteGameObject2D(std::string Name, std::shared_ptr<Material> material);
	static void AddSpriteGameObject2D(std::string Name, std::shared_ptr<Material> material, const glm::vec2& position, int drawLayer);
	static void AddSpriteGameObject2D(std::string Name, std::shared_ptr<Material> material, const glm::vec2& position, const glm::vec2& rotation, int drawLayer);
	static void AddSpriteGameObject2D(std::string Name, std::shared_ptr<Material> material, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, int drawLayer);

	static void AddSpriteGameObject3D(std::string Name, std::shared_ptr<Material> material);
	static void AddSpriteGameObject3D(std::string Name, std::shared_ptr<Material> material, const glm::vec3& position);
	static void AddSpriteGameObject3D(std::string Name, std::shared_ptr<Material> material, const glm::vec3& position, const glm::vec3& rotation);
	static void AddSpriteGameObject3D(std::string Name, std::shared_ptr<Material> material, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	static void AddLineGameObject3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint);
	static void AddLineGameObject3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec3& position);
	static void AddLineGameObject3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec3& position, const glm::vec3& rotation);
	static void AddLineGameObject3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
	static void AddLineGameObject3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& Color);
	static void AddLineGameObject3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& Color, const glm::vec3& position);
	static void AddLineGameObject3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& Color, const glm::vec3& position, const glm::vec3& rotation);
	static void AddLineGameObject3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& Color, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
	static void AddLineGameObject3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& StartColor, const glm::vec4& EndColor);
	static void AddLineGameObject3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& StartColor, const glm::vec4& EndColor, const glm::vec3& position);
	static void AddLineGameObject3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& StartColor, const glm::vec4& EndColor, const glm::vec3& position, const glm::vec3& rotation);
	static void AddLineGameObject3D(const std::string Name, const glm::vec3& StartPoint, const glm::vec3& EndPoint, const glm::vec4& StartColor, const glm::vec4& EndColor, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
	static void AddLineGameObject3D(const std::string Name, std::vector<LineVertex3D> VertexList);
	static void AddLineGameObject3D(const std::string Name, std::vector<LineVertex3D> VertexList, glm::vec3 position);
	static void AddLineGameObject3D(const std::string Name, std::vector<LineVertex3D> VertexList, glm::vec3 position, glm::vec3 rotation);
	static void AddLineGameObject3D(const std::string Name, std::vector<LineVertex3D> VertexList, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	static std::shared_ptr<Texture> LoadTexture2D(std::shared_ptr<Texture> texture);
	static std::shared_ptr<Texture> LoadTexture2D(GLTFTextureLoader& textureLoader);
	static void AddMaterial(const std::shared_ptr<Material> material);
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

	static void UpdateBufferIndex();
	static VkDescriptorImageInfo GetBRDFMapDescriptor();
	static VkDescriptorImageInfo GetIrradianceMapDescriptor();
	static VkDescriptorImageInfo GetPrefilterMapDescriptor();
	static VkDescriptorImageInfo GetCubeMapDescriptor();
	static VkDescriptorImageInfo GetEnvironmentMapDescriptor();

	static std::vector<VkDescriptorBufferInfo> GetVertexPropertiesBuffer();
	static std::vector<VkDescriptorBufferInfo> GetIndexPropertiesBuffer();
	static std::vector<VkDescriptorBufferInfo> GetGameObjectPropertiesBuffer();
	static std::vector<VkDescriptorBufferInfo> GetGameObjectTransformBuffer();
	static std::vector<VkDescriptorImageInfo>  GetTexturePropertiesBuffer();
	static std::vector<VkDescriptorBufferInfo> GetMaterialPropertiesBuffer();
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

