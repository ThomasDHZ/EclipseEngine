#pragma once
#include "UniformBuffer.h"
#include "RenderedColorTexture.h"
#include "RenderedCubeMapTexture.h"
#include "EnvironmentTexture.h"
#include "SunLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "Skybox.h"
#include "Material.h"
#include "GameObject3D.h"
#include "SpriteGameObject2D.h"
#include "SpriteGameObject3D.h"
#include "LineGameObject2D.h"
#include "LineGameObject3D.h"
#include "GridGameObject2D.h"
#include "SquareGameObject2D.h"
#include "LevelGameObject.h"

class GLTFSceneManager
{
private:
	static float PBRCubeMapSize;
	static float PreRenderedMapSize;

	static std::vector<std::shared_ptr<Material>> MaterialList;

	static std::vector<std::shared_ptr<SunLight>> SunLightList;
	static std::vector<std::shared_ptr<DirectionalLight>> DirectionalLightList;
	static std::vector<std::shared_ptr<PointLight>> PointLightList;
	static std::vector<std::shared_ptr<SpotLight>> SpotLightList;
	
	std::vector<VkDescriptorImageInfo> TexturePropertiesBuffer;
	std::vector<VkDescriptorBufferInfo> MaterialPropertiesBuffer;

	static VkSampler NullSampler;
	static VkDescriptorImageInfo NullDescriptor;

	static std::shared_ptr<Texture> IsTexture2DLoaded(std::string name);
	static std::shared_ptr<Texture> IsTexture2DLoaded(const GLTFTextureLoader& textureLoader);

public:
	static bool WireframeModeFlag;
	static bool RaytraceModeFlag;

	static SceneProperties sceneProperites;

	static std::vector<std::shared_ptr<Texture>> TextureList;

	static std::vector<std::shared_ptr<GameObject>> GameObjectList;
	static std::shared_ptr<Camera> ActiveCamera;
	static std::shared_ptr<Skybox> SkyboxMesh;
	static std::shared_ptr<EnvironmentTexture>     EnvironmentTexture;
	static std::shared_ptr<RenderedColorTexture>   BRDFTexture;
	static std::vector<std::shared_ptr<RenderedCubeMapTexture>> IrradianceMapList;
	static std::vector<std::shared_ptr<RenderedCubeMapTexture>> PrefilterMapList;
	static std::shared_ptr<RenderedCubeMapTexture> CubeMap;
	
	static void AddLevelGameObject(const std::string& levelPath, int drawLayer);
	static void AddLevelGameObject(const std::string& levelName, glm::ivec2 tileSizeInPixels, glm::ivec2 levelBounds, std::string tileSetPath, int drawLayer);

	static void AddMeshGameObject3D(const std::string Name, const std::string FilePath);
	static void AddMeshGameObject3D(const std::string Name, const std::string FilePath, const glm::vec3& position);
	static void AddMeshGameObject3D(const std::string Name, const std::string FilePath, const glm::vec3& position, const glm::vec3& rotation);
	static void AddMeshGameObject3D(const std::string Name, const std::string FilePath, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
	
	static void AddMeshGameObject3D(const std::string Name, const std::string FilePath, std::shared_ptr<Material> material);
	static void AddMeshGameObject3D(const std::string Name, const std::string FilePath, std::shared_ptr<Material> material, const glm::vec3& position);
	static void AddMeshGameObject3D(const std::string Name, const std::string FilePath, std::shared_ptr<Material> material, const glm::vec3& position, const glm::vec3& rotation);
	static void AddMeshGameObject3D(const std::string Name, const std::string FilePath, std::shared_ptr<Material> material, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	static void AddBillBoardGameObject3D(const std::string Name, const std::string FilePath);
	static void AddBillBoardGameObject3D(const std::string Name, const std::string FilePath, const glm::vec3& position);
	static void AddBillBoardGameObject3D(const std::string Name, const std::string FilePath, const glm::vec3& position, const glm::vec3& rotation);
	static void AddBillBoardGameObject3D(const std::string Name, const std::string FilePath, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	static void AddBillBoardGameObject3D(const std::string Name, const std::string FilePath, std::shared_ptr<Material> material);
	static void AddBillBoardGameObject3D(const std::string Name, const std::string FilePath, std::shared_ptr<Material> material, const glm::vec3& position);
	static void AddBillBoardGameObject3D(const std::string Name, const std::string FilePath, std::shared_ptr<Material> material, const glm::vec3& position, const glm::vec3& rotation);
	static void AddBillBoardGameObject3D(const std::string Name, const std::string FilePath, std::shared_ptr<Material> material, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	static void AddInstancedGameObject3D(const std::string Name, const std::string FilePath, GLTFInstancingDataStruct& instanceData);
	static void AddInstancedGameObject3D(const std::string Name, const std::string FilePath, GLTFInstancingDataStruct& instanceData, const glm::vec3& position);
	static void AddInstancedGameObject3D(const std::string Name, const std::string FilePath, GLTFInstancingDataStruct& instanceData, const glm::vec3& position, const glm::vec3& rotation);
	static void AddInstancedGameObject3D(const std::string Name, const std::string FilePath, GLTFInstancingDataStruct& instanceData, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	static void AddSpriteGameObject2D(std::string Name, std::shared_ptr<Material> material);
	static void AddSpriteGameObject2D(std::string Name, std::shared_ptr<Material> material, const glm::vec2& position, int drawLayer);
	static void AddSpriteGameObject2D(std::string Name, std::shared_ptr<Material> material, const glm::vec2& position, const glm::vec2& rotation, int drawLayer);
	static void AddSpriteGameObject2D(std::string Name, std::shared_ptr<Material> material, const glm::vec2& position, const glm::vec2& rotation, const glm::vec2& scale, int drawLayer);

	static void AddSpriteGameObject3D(std::string Name, std::shared_ptr<Material> material);
	static void AddSpriteGameObject3D(std::string Name, std::shared_ptr<Material> material, const glm::vec3& position);
	static void AddSpriteGameObject3D(std::string Name, std::shared_ptr<Material> material, const glm::vec3& position, const glm::vec3& rotation);
	static void AddSpriteGameObject3D(std::string Name, std::shared_ptr<Material> material, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	static void AddSquareGameObject2D(const std::string Name, float Size, const glm::vec4& Color, int drawLayer);
	static void AddSquareGameObject2D(const std::string Name, float Size, const glm::vec4& Color, const glm::vec3& position, int drawLayer);
	static void AddSquareGameObject2D(const std::string Name, float Size, const glm::vec4& Color, const glm::vec3& position, const glm::vec3& rotation, int drawLayer);
	static void AddSquareGameObject2D(const std::string Name, float Size, const glm::vec4& Color, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, int drawLayer);

	static void AddLineGameObject2D(const std::string Name, const glm::vec2& StartPoint, const glm::vec2& EndPoint, int drawLayer);
	static void AddLineGameObject2D(const std::string Name, const glm::vec2& StartPoint, const glm::vec2& EndPoint, const glm::vec3& position, int drawLayer);
	static void AddLineGameObject2D(const std::string Name, const glm::vec2& StartPoint, const glm::vec2& EndPoint, const glm::vec3& position, const glm::vec3& rotation, int drawLayer);
	static void AddLineGameObject2D(const std::string Name, const glm::vec2& StartPoint, const glm::vec2& EndPoint, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, int drawLayer);
	static void AddLineGameObject2D(const std::string Name, const glm::vec2& StartPoint, const glm::vec2& EndPoint, const glm::vec4& Color, int drawLayer);
	static void AddLineGameObject2D(const std::string Name, const glm::vec2& StartPoint, const glm::vec2& EndPoint, const glm::vec4& Color, const glm::vec3& position, int drawLayer);
	static void AddLineGameObject2D(const std::string Name, const glm::vec2& StartPoint, const glm::vec2& EndPoint, const glm::vec4& Color, const glm::vec3& position, const glm::vec3& rotation, int drawLayer);
	static void AddLineGameObject2D(const std::string Name, const glm::vec2& StartPoint, const glm::vec2& EndPoint, const glm::vec4& Color, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, int drawLayer);
	static void AddLineGameObject2D(const std::string Name, const glm::vec2& StartPoint, const glm::vec2& EndPoint, const glm::vec4& StartColor, const glm::vec4& EndColor, int drawLayer);
	static void AddLineGameObject2D(const std::string Name, const glm::vec2& StartPoint, const glm::vec2& EndPoint, const glm::vec4& StartColor, const glm::vec4& EndColor, const glm::vec3& position, int drawLayer);
	static void AddLineGameObject2D(const std::string Name, const glm::vec2& StartPoint, const glm::vec2& EndPoint, const glm::vec4& StartColor, const glm::vec4& EndColor, const glm::vec3& position, const glm::vec3& rotation, int drawLayer);
	static void AddLineGameObject2D(const std::string Name, const glm::vec2& StartPoint, const glm::vec2& EndPoint, const glm::vec4& StartColor, const glm::vec4& EndColor, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, int drawLayer);

	static void AddGridGameObject2D(const std::string& GridName, int GridSizeX, int GridSizeY, float GridSpacingX, float GridSpacingY, int drawLayer);

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
	static void LoadReflectionIrradianceTexture(std::shared_ptr<RenderedCubeMapTexture> irradianceTexture);
	static void LoadReflectionIrradianceTexture(std::vector<std::shared_ptr<RenderedCubeMapTexture>> irradianceTextureList);
	static void LoadReflectionPrefilterTexture(std::shared_ptr<RenderedCubeMapTexture> prefilterTexture);
	static void LoadReflectionPrefilterTexture(std::vector<std::shared_ptr<RenderedCubeMapTexture>> prefilterTextureList);
	static void AddMaterial(const std::shared_ptr<Material> material);
	static void AddSunLight(std::shared_ptr<SunLight> sunLight);
	static void AddDirectionalLight(std::shared_ptr<DirectionalLight> directionalLight);
	static void AddPointLight(std::shared_ptr<PointLight> pointLight);
	static void AddSpotLight(std::shared_ptr<SpotLight> spotLight);
	static void AddSunLight(GLTFSunLightLoader& sunLight);
	static void AddDirectionalLight(GLTFDirectionalLightLoader& directionalLight);
	static void AddPointLight(GLTFPointLightLoader& pointLight);
	static void AddSpotLight(GLTFSpotLightLoader& spotLight);

	static void StartUp();
	static void Update();
	static void Destroy();

	static void UpdateBufferIndex();
	static VkDescriptorImageInfo GetBRDFMapDescriptor();
	static std::vector<VkDescriptorImageInfo> GetIrradianceMapDescriptor();
	static std::vector<VkDescriptorImageInfo> GetPrefilterMapDescriptor();
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
	static std::vector<VkDescriptorBufferInfo> GetCubeMapSamplerBuffer();

	static std::vector<GameObject2D*> Get2DGameObjects();

	static std::vector<std::shared_ptr<Material>> GetMaterialList() { return MaterialList; }
	static std::vector<std::shared_ptr<LightBase>> GetAllLights();
	static std::vector<std::shared_ptr<SunLight>> GetSunLights() { return SunLightList; }
	static std::vector<std::shared_ptr<DirectionalLight>> GetDirectionalLights() { return DirectionalLightList; }
	static std::vector<std::shared_ptr<PointLight>> GetPointLights() { return PointLightList; }
	static std::vector<std::shared_ptr<SpotLight>> GetSpotLights() { return SpotLightList; }
	static uint32_t GetSunLightCount() { return SunLightList.size(); }
	static uint32_t GetDirectionalCount() { return DirectionalLightList.size(); }
	static uint32_t GetPointLightCount() { return PointLightList.size(); }
	static uint32_t GetSpotLightCount() { return SpotLightList.size(); }
	static float GetPBRCubeMapSize() { return PBRCubeMapSize; }
	static float GetPreRenderedMapSize() { return PreRenderedMapSize; }
	static VkSampler GetNullSampler() { return NullSampler; }
	static VkDescriptorImageInfo GetNullDescriptor() { return NullDescriptor; }
};

