#pragma once
#include "GameObject.h"
#include "MaterialManager.h"

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
		std::vector<VkDescriptorBufferInfo> MeshPropertiesmBufferList{};

		std::sort(objList.begin(), objList.end(), GameObject::ZSorting{} );

		for (auto obj : objList)
		{
			obj->Update(DeltaTime);
			MaterialManager::Update(DeltaTime);

			auto spriteRenderer = obj->GetComponentByType(ComponentType::kSpriteRenderer);
			auto transform2D = obj->GetComponentByType(ComponentType::kTransform2D);
			if (spriteRenderer != nullptr &&
				transform2D != nullptr)
			{
				SpriteRenderer* sprite = static_cast<SpriteRenderer*>(spriteRenderer.get());
				Transform2D* transform = static_cast<Transform2D*>(transform2D.get());


				MeshProperties meshProps = {};
				meshProps.MeshTransform = transform->Transform;
				meshProps.materialBufferData = sprite->GetMaterial()->GetMaterialTextureData();
				sprite->UpdateMeshProperties(meshProps);
			}

			auto meshRenderer = obj->GetComponentByType(ComponentType::kMeshRenderer);
			auto transform3D = obj->GetComponentByType(ComponentType::kTransform3D);
			if (meshRenderer != nullptr &&
				transform3D != nullptr)
			{
				MeshRenderer* mesh = static_cast<MeshRenderer*>(meshRenderer.get());
				Transform2D* transform = static_cast<Transform2D*>(transform3D.get());

				MeshProperties meshProps = {};
				meshProps.MeshTransform = transform->Transform;
				meshProps.materialBufferData = mesh->GetMaterial()->GetMaterialTextureData();
				mesh->UpdateMeshProperties(meshProps);
			}
		}
	}

	static std::vector<VkDescriptorBufferInfo>  GetMeshPropertiesmBufferList()
	{
		std::vector<VkDescriptorBufferInfo> MeshPropertiesmBufferList{};
		for (auto obj : objList)
		{
			auto spriteRenderer = obj->GetComponentByType(ComponentType::kSpriteRenderer);
			if (spriteRenderer)
			{
				SpriteRenderer* sprite = static_cast<SpriteRenderer*>(spriteRenderer.get());

				MeshProperties meshProps = {};
				sprite->UpdateMeshProperties(meshProps);
				VkBuffer buffer = sprite->GetMeshPropertiesBuffer();

				VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
				MeshPropertiesmBufferBufferInfo.buffer = buffer;
				MeshPropertiesmBufferBufferInfo.offset = 0;
				MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
				MeshPropertiesmBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

				sprite->SetBufferIndex(MeshPropertiesmBufferList.size());
			}

			auto meshRenderer = obj->GetComponentByType(ComponentType::kMeshRenderer);
			if (meshRenderer)
			{
				MeshRenderer* mesh = static_cast<MeshRenderer*>(meshRenderer.get());

				MeshProperties meshProps = {};
				mesh->UpdateMeshProperties(meshProps);
				VkBuffer buffer = mesh->GetMeshPropertiesBuffer();

				VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
				MeshPropertiesmBufferBufferInfo.buffer = buffer;
				MeshPropertiesmBufferBufferInfo.offset = 0;
				MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
				MeshPropertiesmBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

				mesh->SetBufferIndex(MeshPropertiesmBufferList.size());
			}
		}

		return MeshPropertiesmBufferList;
	}

	static std::vector<std::shared_ptr<GameObject>> GetGameObjectList()
	{
		return objList;
	}

	static void Destory()
	{
		for (auto& obj : objList)
		{
			obj->Destory();
		}
	}
};


