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
				Transform3D* transform = static_cast<Transform3D*>(transform3D.get());

				MeshProperties meshProps = {};
				meshProps.MeshTransform = transform->Transform;
				meshProps.materialBufferData = mesh->GetMaterial()->GetMaterialTextureData();
				mesh->UpdateMeshProperties(meshProps);
			}

			auto lineRenderer = obj->GetComponentByType(ComponentType::kLineRenderer);
			if (lineRenderer != nullptr &&
				transform3D != nullptr)
			{
				LineRenderer* line = static_cast<LineRenderer*>(lineRenderer.get());
				Transform3D* transform = static_cast<Transform3D*>(transform3D.get());

				MeshProperties meshProps = {};
				meshProps.MeshTransform = transform->Transform;
				meshProps.materialBufferData = line->GetMaterial()->GetMaterialTextureData();
				line->UpdateMeshProperties(meshProps);
			}
		}
	}

	static std::vector<VkDescriptorBufferInfo>  GetMeshPropertiesBufferList()
	{
		std::vector<VkDescriptorBufferInfo> MeshPropertiesBufferList{};
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
				MeshPropertiesBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

				sprite->SetBufferIndex(MeshPropertiesBufferList.size() - 1);
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
				MeshPropertiesBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

				mesh->SetBufferIndex(MeshPropertiesBufferList.size() - 1);
			}

			auto lineRenderer = obj->GetComponentByType(ComponentType::kLineRenderer);
			if (lineRenderer)
			{
				SpriteRenderer* line = static_cast<SpriteRenderer*>(lineRenderer.get());

				MeshProperties meshProps = {};
				line->UpdateMeshProperties(meshProps);
				VkBuffer buffer = line->GetMeshPropertiesBuffer();

				VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
				MeshPropertiesmBufferBufferInfo.buffer = buffer;
				MeshPropertiesmBufferBufferInfo.offset = 0;
				MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
				MeshPropertiesBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

				line->SetBufferIndex(MeshPropertiesBufferList.size() - 1);
			}
		}

		return MeshPropertiesBufferList;
	}

	static std::vector<VkDescriptorBufferInfo>  GetMeshVertexBufferList()
	{
		std::vector<VkDescriptorBufferInfo> MeshVertexBufferList{};
		for (auto obj : objList)
		{
			auto spriteRenderer = obj->GetComponentByType(ComponentType::kSpriteRenderer);
			if (spriteRenderer)
			{
				SpriteRenderer* sprite = static_cast<SpriteRenderer*>(spriteRenderer.get());

				MeshProperties meshProps = {};
				sprite->UpdateMeshProperties(meshProps);
				VkBuffer buffer = sprite->GetMeshVertexBuffer();

				VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
				MeshPropertiesmBufferBufferInfo.buffer = buffer;
				MeshPropertiesmBufferBufferInfo.offset = 0;
				MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
				MeshVertexBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

				sprite->SetBufferIndex(MeshVertexBufferList.size() - 1);
			}

			auto meshRenderer = obj->GetComponentByType(ComponentType::kMeshRenderer);
			if (meshRenderer)
			{
				MeshRenderer* mesh = static_cast<MeshRenderer*>(meshRenderer.get());

				MeshProperties meshProps = {};
				mesh->UpdateMeshProperties(meshProps);
				VkBuffer buffer = mesh->GetMeshVertexBuffer();

				VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
				MeshPropertiesmBufferBufferInfo.buffer = buffer;
				MeshPropertiesmBufferBufferInfo.offset = 0;
				MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
				MeshVertexBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

				mesh->SetBufferIndex(MeshVertexBufferList.size() - 1);
			}

			auto lineRenderer = obj->GetComponentByType(ComponentType::kLineRenderer);
			if (lineRenderer)
			{
				LineRenderer* line = static_cast<LineRenderer*>(lineRenderer.get());

				MeshProperties meshProps = {};
				line->UpdateMeshProperties(meshProps);
				VkBuffer buffer = line->GetMeshVertexBuffer();

				VkDescriptorBufferInfo linePropertiesmBufferBufferInfo = {};
				linePropertiesmBufferBufferInfo.buffer = buffer;
				linePropertiesmBufferBufferInfo.offset = 0;
				linePropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
				MeshVertexBufferList.emplace_back(linePropertiesmBufferBufferInfo);

				line->SetBufferIndex(MeshVertexBufferList.size() - 1);
			}
		}

		return MeshVertexBufferList;
	}

	static std::vector<VkDescriptorBufferInfo>  GetMeshIndexBufferList()
	{
		std::vector<VkDescriptorBufferInfo> MeshIndexBufferList{};
		for (auto obj : objList)
		{
			auto spriteRenderer = obj->GetComponentByType(ComponentType::kSpriteRenderer);
			if (spriteRenderer)
			{
				SpriteRenderer* sprite = static_cast<SpriteRenderer*>(spriteRenderer.get());

				MeshProperties meshProps = {};
				sprite->UpdateMeshProperties(meshProps);
				VkBuffer buffer = sprite->GetMeshIndiceBuffer();

				VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
				MeshPropertiesmBufferBufferInfo.buffer = buffer;
				MeshPropertiesmBufferBufferInfo.offset = 0;
				MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
				MeshIndexBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

				sprite->SetBufferIndex(MeshIndexBufferList.size() - 1);
			}

			auto meshRenderer = obj->GetComponentByType(ComponentType::kMeshRenderer);
			if (meshRenderer)
			{
				MeshRenderer* mesh = static_cast<MeshRenderer*>(meshRenderer.get());

				MeshProperties meshProps = {};
				mesh->UpdateMeshProperties(meshProps);
				VkBuffer buffer = mesh->GetMeshIndiceBuffer();

				VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
				MeshPropertiesmBufferBufferInfo.buffer = buffer;
				MeshPropertiesmBufferBufferInfo.offset = 0;
				MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
				MeshIndexBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

				mesh->SetBufferIndex(MeshIndexBufferList.size() - 1);
			}

			auto lineRenderer = obj->GetComponentByType(ComponentType::kLineRenderer);
			if (lineRenderer)
			{
				LineRenderer* line = static_cast<LineRenderer*>(lineRenderer.get());

				MeshProperties meshProps = {};
				line->UpdateMeshProperties(meshProps);
				VkBuffer buffer = line->GetMeshIndiceBuffer();

				VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
				MeshPropertiesmBufferBufferInfo.buffer = buffer;
				MeshPropertiesmBufferBufferInfo.offset = 0;
				MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
				MeshIndexBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

				line->SetBufferIndex(MeshIndexBufferList.size() - 1);
			}
		}

		return MeshIndexBufferList;
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


