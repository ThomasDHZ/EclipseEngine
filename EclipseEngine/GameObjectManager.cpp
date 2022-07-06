#include "GameObjectManager.h"
#include "SpriteRenderer.h"
#include "MeshRenderer.h"

 void GameObjectManager::AddGameObject(std::shared_ptr<GameObject> obj)
{
	objList.emplace_back(obj);
	VulkanRenderer::UpdateRendererFlag = true;
}

 void GameObjectManager::RemoveGameObject(std::shared_ptr<GameObject> obj)
{
	for (int x = objList.size() - 1; x > 0; x--)
	{
		if (objList[x] == obj)
		{
			obj->Destroy();
			objList.erase(objList.begin() + x);
			VulkanRenderer::UpdateRendererFlag = true;
			break;
		}
	}
}


 std::shared_ptr<GameObject> GameObjectManager::GetGameObjectByID(uint64_t ObjID)
{
	for (auto obj : objList)
	{
		if (obj->GetGameObjectID() == ObjID)
		{
			return obj;
		}
	}

	return nullptr;
}

 std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetGameObjectsByComponentType(ComponentType componentType)
{
	std::vector<std::shared_ptr<GameObject>> returnObjectList;
	for (auto obj : objList)
	{
		auto componentExists = obj->GetComponentByType(componentType);
		if (componentExists)
		{
			returnObjectList.emplace_back(obj);
		}
	}

	return returnObjectList;
}

 void GameObjectManager::Update(float DeltaTime)
{
	for (auto obj : objList)
	{
		obj->Update(DeltaTime);
	}
}

 void GameObjectManager::DrawDepthMesh(VkCommandBuffer& commandBuffer, std::shared_ptr<GraphicsPipeline> pipeline, std::shared_ptr<Mesh> mesh, DirectionalLightProjection& directionalLightProjection)
 {
	 MeshRendererManager::DrawDepthMesh(commandBuffer, pipeline, mesh, directionalLightProjection);
 }

 void GameObjectManager::DrawMesh(VkCommandBuffer& commandBuffer, std::shared_ptr<GraphicsPipeline> pipeline, std::shared_ptr<Mesh> mesh, SceneProperties& sceneProperties)
 {
	 MeshRendererManager::DrawMesh(commandBuffer, pipeline, mesh, sceneProperties);
 }

 void GameObjectManager::DrawLine(VkCommandBuffer& commandBuffer, std::shared_ptr<GraphicsPipeline> pipeline, std::shared_ptr<Mesh> mesh, SceneProperties& sceneProperties)
 {
	 MeshRendererManager::DrawMesh(commandBuffer, pipeline, mesh, sceneProperties);
 }

 std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetGameObjectList()
{
	return objList;
}

 void GameObjectManager::LoadGameObjects(nlohmann::json& json)
{
	for (int x = 0; x < json["ObjectList"].size(); x++)
	{
		std::shared_ptr<GameObject> obj = std::make_shared<GameObject>(GameObject(json.at("ObjectList")[x]));
		objList.emplace_back(obj);
	}
	VulkanRenderer::UpdateRendererFlag = true;
}

 nlohmann::json GameObjectManager::SaveGameObjects()
{
	nlohmann::json json;
	for (int x = 0; x < objList.size(); x++)
	{
		objList[x]->to_json(json[x]);
	}
	return json;
}

 void GameObjectManager::Destroy()
{
	for (int x = objList.size() - 1; x >= 0; x--)
	{
		objList[x]->Destroy();
		objList.erase(objList.begin() + x);
	}
}

 void GameObjectManager::DestroyScene()
{
	for (int x = objList.size() - 1; x >= 0; x--)
	{
		objList[x]->Destroy();
		objList.erase(objList.begin() + x);
	}
}

 void GameObjectManager::DestoryGameObject(uint64_t GameObjectID)
{
	std::shared_ptr<GameObject> gameObject = GetGameObjectByID(GameObjectID);
	gameObject->Destroy();
	for (int x = objList.size() - 1; x >= 0; x--)
	{
		if (gameObject == objList[x])
		{
			objList.erase(objList.begin() + x);
			break;
		}
	}
}