#pragma once
#include "VulkanRenderer.h"
#include "Component.h"
#include "SpriteRenderer.h"
#include "MeshRenderer.h"
#include "Transform2D.h"
#include "Transform3D.h"

class GameObject
{
private:
	static uint64_t GameObjectIDCounter;

	std::string ObjectName;
	uint64_t GameObjectID;
	std::vector<std::shared_ptr<Component>> ComponentList;

public:

	GameObject();
	GameObject(const std::string Name, std::vector<LineVertex>& VertexList, int a);
	GameObject(const std::string Name, glm::vec3 StartLine, glm::vec3 EndLine, int a);
	GameObject(const std::string Name);
	GameObject(const std::string Name, glm::vec2 position, uint32_t zIndex);
	GameObject(const std::string Name, glm::vec2 position, glm::vec2 rotation, uint32_t zIndex);
	GameObject(const std::string Name, glm::vec2 position, glm::vec2 rotation, glm::vec2 scale, uint32_t zIndex);
	GameObject(const std::string Name, glm::vec3 position);
	GameObject(const std::string Name, glm::vec3 position, glm::vec3 rotation);
	GameObject(const std::string Name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	GameObject(nlohmann::json& json);
	virtual ~GameObject();

	virtual void Update(float DeltaTime);
	void Draw(VkCommandBuffer& commandBuffer);
	void Destory();

	void AddComponent(std::shared_ptr<Component> component);
	void RemoveComponent(std::shared_ptr<Component> component);

	void GenerateID();

	std::string GetObjectName() { return ObjectName; }
	uint64_t GetGameObjectID() { return GameObjectID; }

	std::shared_ptr<Component> GetComponentByType(ComponentType componentType);
	std::shared_ptr<Component> GetComponentByID(uint64_t ComponentID);
	void GetGameObjectPropertiesBuffer(std::vector<VkDescriptorBufferInfo>& MeshPropertiesBufferList);
	void GetMeshVertexBuffer(std::vector<VkDescriptorBufferInfo>& VertexBufferList);
	void GetMeshIndexBuffer(std::vector<VkDescriptorBufferInfo>& IndexBufferList);

	std::vector<std::shared_ptr<Component>> GetComponentList() { return ComponentList; };

	virtual nlohmann::json ToJson()
	{
		nlohmann::json json;

		json["ObjectName"] = ObjectName;
		for (int x = 0; x < ComponentList.size(); x++)
		{
			json["ComponentList"][x] = ComponentList[x]->ToJson();
		}
		std::cout << json << std::endl;

		return json;
	}

	class ZSorting
	{
	public:
		bool operator()(std::shared_ptr<GameObject> obj1, std::shared_ptr<GameObject> obj2)
		{
			if (obj1->GetComponentByType(ComponentType::kTransform3D) != nullptr &&
				obj2->GetComponentByType(ComponentType::kTransform3D) != nullptr)
			{
				Transform3D* transform3D1 = static_cast<Transform3D*>(obj1->GetComponentByType(ComponentType::kTransform3D).get());
				Transform3D* transform3D2 = static_cast<Transform3D*>(obj2->GetComponentByType(ComponentType::kTransform3D).get());

				return transform3D1->Position.z < transform3D2->Position.z;
			}

			if (obj1->GetComponentByType(ComponentType::kTransform2D) != nullptr &&
				obj2->GetComponentByType(ComponentType::kTransform2D) != nullptr)
			{
				Transform2D* transform2D1 = static_cast<Transform2D*>(obj1->GetComponentByType(ComponentType::kTransform2D).get());
				Transform2D* transform2D2 = static_cast<Transform2D*>(obj2->GetComponentByType(ComponentType::kTransform2D).get());

				return transform2D1->ZIndex < transform2D2->ZIndex;
			}

			return false;
		}
	};
};

