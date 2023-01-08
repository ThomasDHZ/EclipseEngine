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

	static void Update(float DeltaTime);
	static void Destroy();
	static void DestroyScene();
	static void DestoryGameObject(uint64_t GameObjectID);

	static std::shared_ptr<GameObject> GetGameObjectByID(uint64_t ObjID);
	static std::vector<std::shared_ptr<GameObject>> GetGameObjectsByComponentType(ComponentType componentType);
	static std::vector<std::shared_ptr<GameObject>> GetGameObjectList();
	static std::vector<std::shared_ptr<GameObject>> GetMeshRendererGameObjects();
	static std::vector<std::shared_ptr<GameObject>> GetModelRendererGameObjects();
	static std::vector<std::shared_ptr<GameObject>> GetSpriteRendererGameObjects();
	static std::vector<std::shared_ptr<GameObject>> GetkLineRenderer2DGameObjects();
	static std::vector<std::shared_ptr<GameObject>> GetkLineRenderer3DGameObjects();
	static std::vector<std::shared_ptr<GameObject>> GetkGridRenderer2DGameObjects();
	static std::vector<std::shared_ptr<GameObject>> GetkGridRenderer3DGameObjects();
};


