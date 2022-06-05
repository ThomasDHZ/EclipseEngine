#pragma once
#include "GameObject.h"
#include "MaterialManager.h"
#include <algorithm>

class GameObjectManager
{
private:
	static std::vector<std::shared_ptr<GameObject>> objList;
public:
	
	static void AddGameObject(std::shared_ptr<GameObject> obj)
	{
		objList.emplace_back(obj);
		VulkanRenderer::UpdateRendererFlag = true;
	}

	static void RemoveGameObject(std::shared_ptr<GameObject> obj)
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


	static std::shared_ptr<GameObject> GetGameObjectByID(uint64_t ObjID)
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

	static std::vector<std::shared_ptr<GameObject>> GetGameObjectsByComponentType(ComponentType componentType)
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
	
	static void Update(float DeltaTime)
	{
		for (auto obj : objList)
		{
			obj->Update(DeltaTime);
		}
	}

	static std::vector<std::shared_ptr<GameObject>> GetGameObjectList()
	{
		return objList;
	}

	static void LoadGameObjects(nlohmann::json& json)
	{
		for (int x = 0; x < json["ObjectList"].size(); x++)
		{
			std::shared_ptr<GameObject> obj = std::make_shared<GameObject>(GameObject(json.at("ObjectList")[x]));
			objList.emplace_back(obj);
		}
		VulkanRenderer::UpdateRendererFlag = true;
	}

	static nlohmann::json SaveGameObjects()
	{
		nlohmann::json json;
		for (int x = 0; x < objList.size(); x++)
		{
			objList[x]->to_json(json[x]);
		}
		return json;
	}

	static void Destroy()
	{
		for (int x = objList.size() - 1; x >= 0; x--)
		{
			objList[x]->Destroy();
			objList.erase(objList.begin() + x);
		}
	}

	static void DestroyScene()
	{
		for (int x = objList.size() - 1; x >= 0; x--)
		{
			objList[x]->Destroy();
			objList.erase(objList.begin() + x);
		}
	}

	static void DestoryGameObject(uint64_t GameObjectID)
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
};


