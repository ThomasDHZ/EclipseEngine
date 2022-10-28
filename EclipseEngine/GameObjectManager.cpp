#include "GameObjectManager.h"
#include "SpriteRenderer.h"
#include "ModelRenderer.h"


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

 std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetGameObjectList()
{
	return objList;
}

 std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetMeshRendererGameObjects()
 {
	 std::vector<std::shared_ptr<GameObject>> GameObjectList;
	 for (auto& obj : objList)
	 {
		 if (obj->GetRenderType() == GameObjectRenderType::kMeshRenderer)
		 {
			 GameObjectList.emplace_back(obj);
		 }
	 }
	 return GameObjectList;
 }

 std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetModelRendererGameObjects()
 {
	 std::vector<std::shared_ptr<GameObject>> GameObjectList;
	 for (auto& obj : objList)
	 {
		 if (obj->GetRenderType() == GameObjectRenderType::kModelRenderer)
		 {
			 GameObjectList.emplace_back(obj);
		 }
	 }
	 return GameObjectList;
 }

 std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetSpriteRendererGameObjects()
 {
	 std::vector<std::shared_ptr<GameObject>> GameObjectList;
	 for (auto& obj : objList)
	 {
		 if (obj->GetRenderType() == GameObjectRenderType::kSpriteRenderer)
		 {
			 GameObjectList.emplace_back(obj);
		 }
	 }
	 return GameObjectList;
 }

 std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetkLineRenderer2DGameObjects()
 {
	 std::vector<std::shared_ptr<GameObject>> GameObjectList;
	 for (auto& obj : objList)
	 {
		 if (obj->GetRenderType() == GameObjectRenderType::kLineRenderer2D)
		 {
			 GameObjectList.emplace_back(obj);
		 }
	 }
	 return GameObjectList;
 }

 std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetkLineRenderer3DGameObjects()
 {
	 std::vector<std::shared_ptr<GameObject>> GameObjectList;
	 for (auto& obj : objList)
	 {
		 if (obj->GetRenderType() == GameObjectRenderType::kLineRenderer3D)
		 {
			 GameObjectList.emplace_back(obj);
		 }
	 }
	 return GameObjectList;
 }

 std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetkGridRenderer2DGameObjects()
 {
	 std::vector<std::shared_ptr<GameObject>> GameObjectList;
	 for (auto& obj : objList)
	 {
		 if (obj->GetRenderType() == GameObjectRenderType::kGridRenderer2D)
		 {
			 GameObjectList.emplace_back(obj);
		 }
	 }
	 return GameObjectList;
 }

 std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetkGridRenderer3DGameObjects()
 {
	 std::vector<std::shared_ptr<GameObject>> GameObjectList;
	 for (auto& obj : objList)
	 {
		 if (obj->GetRenderType() == GameObjectRenderType::kGridRenderer3D)
		 {
			 GameObjectList.emplace_back(obj);
		 }
	 }
	 return GameObjectList;
 }

 void GameObjectManager::LoadGameObjects(nlohmann::json& json)
{
	for (int x = 0; x < json["ObjectList"].size(); x++)
	{
		std::shared_ptr<ModelRenderer> obj = std::make_shared<ModelRenderer>(ModelRenderer(json.at("ObjectList")[x]));
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