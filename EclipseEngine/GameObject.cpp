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
	GenerateColorID();
	AddComponent(std::make_shared<LineRenderer>(LineRenderer(VertexList, GameObjectID)));
}

GameObject::GameObject(const std::string Name, glm::vec3 StartLine, glm::vec3 EndLine, int a)
{
	ObjectName = Name;
	GenerateID();
	GenerateColorID();
	AddComponent(std::make_shared<LineRenderer>(LineRenderer(StartLine, EndLine, GameObjectID)));
}

GameObject::GameObject(const std::string Name)
{
	ObjectName = Name;
	GenerateID();
	GenerateColorID();
}

GameObject::GameObject(const std::string Name, glm::vec2 position, uint32_t zIndex)
{
	ObjectName = Name;
	GenerateID();
	GenerateColorID();
	AddComponent(std::make_shared<SpriteRenderer>(SpriteRenderer(glm::vec3(position.x, position.y, zIndex), GameObjectID)));
}

GameObject::GameObject(const std::string Name, glm::vec2 position, glm::vec2 rotation, uint32_t zIndex)
{
	ObjectName = Name;
	GenerateID();
	GenerateColorID();
	AddComponent(std::make_shared<SpriteRenderer>(SpriteRenderer(glm::vec3(position.x, position.y, zIndex), glm::vec3(rotation.x, rotation.y, 0.0f), GameObjectID)));
}

GameObject::GameObject(const std::string Name, glm::vec2 position, glm::vec2 rotation, glm::vec2 scale, uint32_t zIndex)
{
	ObjectName = Name;
	GenerateID();
	GenerateColorID();
	AddComponent(std::make_shared<SpriteRenderer>(SpriteRenderer(glm::vec3(position.x, position.y, zIndex), glm::vec3(rotation.x, rotation.y, 0.0f), glm::vec3(scale.x, scale.y, 0.0f), GameObjectID)));
}

GameObject::GameObject(const std::string Name, glm::vec3 position)
{
	ObjectName = Name;
	GenerateID();
	GenerateColorID();
	AddComponent(std::make_shared<MeshRenderer>(MeshRenderer(position, GameObjectID)));
}

GameObject::GameObject(const std::string Name, glm::vec3 position, glm::vec3 rotation)
{
	ObjectName = Name;
	GenerateID();
	GenerateColorID();
	AddComponent(std::make_shared<MeshRenderer>(MeshRenderer(position, rotation, GameObjectID)));
}

GameObject::GameObject(const std::string Name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	ObjectName = Name;
	GenerateID();
	GenerateColorID();
	AddComponent(std::make_shared<MeshRenderer>(MeshRenderer(position, rotation, scale, GameObjectID)));
}

GameObject::GameObject(const std::string Name, const std::string filePath)
{
	ObjectName = Name;
	GenerateID();
	GenerateColorID();
	AddComponent(std::make_shared<MeshRenderer>(MeshRenderer(filePath, GameObjectID)));
}

GameObject::GameObject(nlohmann::json& json)
{
	std::cout << json << std::endl;

	json.at("ObjectName").get_to(ObjectName);

	for (int x = 0; x <= json.size(); x++)
	{
		ComponentType type = ComponentType::kNullComponent;
		json["ComponentList"][x].at("componentType").get_to(type);

		switch (type)
		{
		case ComponentType::kSpriteRenderer: {break; }
		case ComponentType::kMeshRenderer:
		{

			MeshRenderer a = MeshRenderer(json["ComponentList"][x]);
			break;
		}
		}
	}
	auto abs = 34;
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

void GameObject::Destory()
{
	for (auto& comp : ComponentList)
	{
		comp->Destroy();
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

void GameObject::GenerateColorID()
{
	const float red = (255.0f - (float)GameObjectID) / 255.0f;
	const float green = (255.0f - (float)GameObjectID) / 255.0f;
	const float blue = (255.0f - (float)GameObjectID) / 255.0f;

	GameObjectColorID = glm::vec3(red, green, blue);
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

