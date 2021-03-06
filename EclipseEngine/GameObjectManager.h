#pragma once
#include "GameObject.h"
#include "MaterialManager.h"
#include "GraphicsPipeline.h"
#include <algorithm>

class GameObjectManager
{
private:
	static std::vector<std::shared_ptr<GameObject>> objList;
public:
	
	static void AddGameObject(std::shared_ptr<GameObject> obj);
	static void RemoveGameObject(std::shared_ptr<GameObject> obj);

	static void LoadGameObjects(nlohmann::json& json);
	static nlohmann::json SaveGameObjects();

	static void Update(float DeltaTime);
	static void DrawDepthMesh(VkCommandBuffer& commandBuffer, std::shared_ptr<GraphicsPipeline> pipeline, std::shared_ptr<Mesh> mesh, DirectionalLightProjection& directionalLightProjection);
	static void DrawMesh(VkCommandBuffer& commandBuffer, std::shared_ptr<GraphicsPipeline> pipeline, std::shared_ptr<Mesh> mesh, SceneProperties& sceneProperties);
	static void DrawLine(VkCommandBuffer& commandBuffer, std::shared_ptr<GraphicsPipeline> pipeline, std::shared_ptr<Mesh> mesh, SceneProperties& sceneProperties);
	static void Destroy();
	static void DestroyScene();
	static void DestoryGameObject(uint64_t GameObjectID);

	static std::shared_ptr<GameObject> GetGameObjectByID(uint64_t ObjID);
	static std::vector<std::shared_ptr<GameObject>> GetGameObjectsByComponentType(ComponentType componentType);
	static std::vector<std::shared_ptr<GameObject>> GetGameObjectList();
};


