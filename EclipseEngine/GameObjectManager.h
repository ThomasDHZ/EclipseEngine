#pragma once
#include "GameObject.h"
#include "MaterialManager.h"
#include "LineRenderer.h"

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
				obj->Destory();
				objList.erase(objList.begin() + x);
				VulkanRenderer::UpdateRendererFlag = true;
				break;
			}
		}
	}


	static std::shared_ptr<GameObject> GetGameObjectsByID(uint64_t ObjID)
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
		std::sort(objList.begin(), objList.end(), GameObject::ZSorting{} );
		for (auto obj : objList)
		{
			MaterialManager::Update(DeltaTime);
			obj->Update(DeltaTime);
		}
	}

	static std::vector<std::shared_ptr<GameObject>> GetGameObjectList()
	{
		return objList;
	}

	static nlohmann::json SaveGameObjects()
	{
		nlohmann::json json;
		for (int x = 0; x < objList.size(); x++)
		{
			json[x] = objList[x]->ToJson();
		}
		return json;
	}

	static void Destory()
	{
		for (auto& obj : objList)
		{
			obj->Destory();
		}
	}
};


