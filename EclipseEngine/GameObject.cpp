#include "GameObject.h"

GameObject::GameObject()
{
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

std::shared_ptr<Component> GameObject::GetComponent(std::shared_ptr<Component> component)
{
	for (auto& comp : ComponentList)
	{
		if (component->GetComponentType() == comp->GetComponentType())
		{
			return comp;
		}
	}
}
