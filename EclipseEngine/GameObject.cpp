#include "GameObject.h"
#include "LineRenderer.h"
#include "SpriteRenderer.h"
#include "MeshRenderer.h"

uint64_t GameObject::GameObjectIDCounter = 0;

GameObject::GameObject()
{
}

GameObject::GameObject(const std::string Name, std::vector<LineVertex>& VertexList, int a)
{
	ObjectName = Name;
	GenerateID();
	AddComponent(std::make_shared<LineRenderer>(LineRenderer(VertexList, GameObjectID)));
}

GameObject::GameObject(const std::string Name, glm::vec3 StartLine, glm::vec3 EndLine, int a)
{
	ObjectName = Name;
	GenerateID();
	AddComponent(std::make_shared<LineRenderer>(LineRenderer(StartLine, EndLine, GameObjectID)));
}

GameObject::GameObject(const std::string Name)
{
	ObjectName = Name;
	GenerateID();
}

GameObject::GameObject(nlohmann::json& json)
{
	GenerateID();
	json.at("ObjectName").get_to(ObjectName);

	for (int x = 0; x < json["ComponentList"].size(); x++)
	{
		ComponentType type = ComponentType::kNullComponent;
		json["ComponentList"][x].at("componentType").get_to(type);

		switch (type)
		{
			case ComponentType::kSpriteRenderer: {break; }
			case ComponentType::kMeshRenderer: AddComponent(std::make_shared<MeshRenderer>(MeshRenderer(json["ComponentList"][x], GameObjectID))); break;
		}
	}
}

GameObject::~GameObject()
{
}

void GameObject::Update(float DeltaTime)
{
	for (auto& comp : ComponentList)
	{
		comp->Update(DeltaTime);
	}
}

void GameObject::Destroy()
{
	for (int x = ComponentList.size() - 1; x >= 0; x--)
	{
		ComponentList[x]->Destroy();
		ComponentList.erase(ComponentList.begin() + x);
	}
}

void GameObject::AddComponent(std::shared_ptr<Component> component)
{
	component->GenerateID();
	ComponentList.emplace_back(component);
}

void GameObject::RemoveComponent(std::shared_ptr<Component> component)
{
	for (int x = ComponentList.size() - 1; x > 0; x--)
	{
		if (component->GetComponentType() == ComponentList[x]->GetComponentType())
		{
			ComponentList[x]->Destroy();
			ComponentList.erase(ComponentList.begin() + x);
			break;
		}
	}
}

void GameObject::GenerateID()
{
	GameObjectIDCounter++;
	GameObjectID = GameObjectIDCounter;
}

std::shared_ptr<Component> GameObject::GetComponentByType(ComponentType componentType)
{
	for (auto& comp : ComponentList)
	{
		if (comp->GetComponentType() == componentType)
		{
			return comp;
		}
	}

	return nullptr;
}

std::shared_ptr<Component> GameObject::GetComponentBySubType(ComponentSubType componentSubType)
{
	for (auto& comp : ComponentList)
	{
		if (comp->GetComponentSubType() == componentSubType)
		{
			return comp;
		}
	}

	return nullptr;
}

std::shared_ptr<Component> GameObject::GetComponentByID(uint64_t ComponentID)
{
	for (auto& comp : ComponentList)
	{
		if (comp->GetComponentID() == ComponentID)
		{
			return comp;
		}
	}

	return nullptr;
}

