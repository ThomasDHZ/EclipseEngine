#include "GameObject.h"

uint64_t GameObject::GameObjectIDCounter = 0;

GameObject::GameObject()
{
}

GameObject::GameObject(const std::string Name)
{
	ObjectName = Name;
}

GameObject::GameObject(const std::string Name, glm::vec2 position, uint32_t zIndex)
{
	ObjectName = Name;
	AddComponent(std::make_shared<Transform2D>(Transform2D(position, zIndex)));
}

GameObject::GameObject(const std::string Name, glm::vec2 position, glm::vec2 rotation, uint32_t zIndex)
{
	ObjectName = Name;
	AddComponent(std::make_shared<Transform2D>(Transform2D(position, rotation, zIndex)));
}

GameObject::GameObject(const std::string Name, glm::vec2 position, glm::vec2 rotation, glm::vec2 scale, uint32_t zIndex)
{
	ObjectName = Name;
	AddComponent(std::make_shared<Transform2D>(Transform2D(position, rotation, scale, zIndex)));
}

GameObject::GameObject(const std::string Name, glm::vec3 position)
{
	ObjectName = Name;
	AddComponent(std::make_shared<Transform3D>(Transform3D(position)));
}

GameObject::GameObject(const std::string Name, glm::vec3 position, glm::vec3 rotation)
{
	ObjectName = Name;
	AddComponent(std::make_shared<Transform3D>(Transform3D(position, rotation)));
}

GameObject::GameObject(const std::string Name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	ObjectName = Name;
	AddComponent(std::make_shared<Transform3D>(Transform3D(position, rotation, scale)));
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

void GameObject::Draw(VkCommandBuffer& commandBuffer)
{
	for (auto& comp : ComponentList)
	{
		if (comp->GetComponentType() == ComponentType::kSpriteRenderer ||
			comp->GetComponentType() == ComponentType::kMeshRenderer)
		{
			if (comp->GetComponentType() == ComponentType::kSpriteRenderer)
			{
				auto spriteRenderer = static_cast<SpriteRenderer*>(comp.get());
				if (spriteRenderer)
				{
					spriteRenderer->Draw(commandBuffer);
				}
			}
			else if(comp->GetComponentType() == ComponentType::kMeshRenderer)
			{
				auto meshRenderer = static_cast<MeshRenderer*>(comp.get());
				if (meshRenderer)
				{
					meshRenderer->Draw(commandBuffer);
				}
			}
		}
	}
}

void GameObject::AddComponent(std::shared_ptr<Component> component)
{
	ComponentList.emplace_back(component);
}

void GameObject::RemoveComponent(std::shared_ptr<Component> component)
{
	for (int x = ComponentList.size() - 1; x > 0; x--)
	{
		if (component->GetComponentType() == ComponentList[x]->GetComponentType())
		{
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
