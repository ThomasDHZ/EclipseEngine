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

GameObject::GameObject(const std::string Name, glm::vec3 Position)
{
	GenerateID();
	ObjectName = Name;
	GameObjectPosition = Position;

	GameObjectTransform = glm::mat4(1.0f);
	GameObjectTransform = glm::translate(GameObjectTransform, GameObjectPosition);
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	GameObjectTransform = glm::scale(GameObjectTransform, GameObjectScale);
}

GameObject::GameObject(const std::string Name, glm::vec3 Position, glm::vec3 Rotation)
{
	GenerateID();
	ObjectName = Name;
	GameObjectPosition = Position;
	GameObjectRotation = Rotation;

	GameObjectTransform = glm::mat4(1.0f);
	GameObjectTransform = glm::translate(GameObjectTransform, GameObjectPosition);
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	GameObjectTransform = glm::scale(GameObjectTransform, GameObjectScale);
}

GameObject::GameObject(const std::string Name, glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale)
{
	GenerateID();
	ObjectName = Name;
	GameObjectPosition = Position;
	GameObjectRotation = Rotation;
	GameObjectScale = Scale;

	GameObjectTransform = glm::mat4(1.0f);
	GameObjectTransform = glm::translate(GameObjectTransform, GameObjectPosition);
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	GameObjectTransform = glm::scale(GameObjectTransform, GameObjectScale);
}

GameObject::GameObject(nlohmann::json& json)
{
	//GenerateID();
	//json.at("ObjectName").get_to(ObjectName);

	//for (int x = 0; x < json["ComponentList"].size(); x++)
	//{
	//	ComponentType type = ComponentType::kNullComponent;
	//	json["ComponentList"][x].at("componentType").get_to(type);

	//	switch (type)
	//	{
	//		case ComponentType::kSpriteRenderer: {break; }
	//		case ComponentType::kMeshRenderer: AddComponent(std::make_shared<MeshRenderer>(MeshRenderer(json["ComponentList"][x], GameObjectID))); break;
	//	}
	//}
}

GameObject::~GameObject()
{
}

void GameObject::Update(float DeltaTime)
{
	GameObjectTransform = glm::mat4(1.0f);
	GameObjectTransform = glm::translate(GameObjectTransform, GameObjectPosition);
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	GameObjectTransform = glm::scale(GameObjectTransform, GameObjectScale);

	for (auto& comp : ComponentList)
	{
		if (comp->GetComponentType() == ComponentType::kSpriteRenderer ||
			comp->GetComponentType() == ComponentType::kMeshRenderer)
		{
			if(comp->GetComponentType() == ComponentType::kSpriteRenderer)
			{
				const auto spriteRenderer = static_cast<SpriteRenderer*>(comp.get());
				spriteRenderer->Update(GameObjectTransform, DeltaTime);
			}
			else if(comp->GetComponentType() == ComponentType::kMeshRenderer)
			{
				const auto meshRenderer = static_cast<MeshRenderer*>(comp.get());
				meshRenderer->Update(GameObjectTransform, DeltaTime);
			}
		}
		else
		{
			comp->Update(DeltaTime);
		}
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

void GameObject::SetGameObjectPosition(float x, float y, float z)
{
	GameObjectPosition = glm::vec3(x, y, z);
}

void GameObject::SetGameObjectRotation(float x, float y, float z)
{
	GameObjectRotation = glm::vec3(x, y, z);
}

void GameObject::SetGameObjectScale(float x, float y, float z)
{
	GameObjectScale = glm::vec3(x, y, z);
}

void GameObject::SetGameObjectPosition(glm::vec3 gameObjectPosition)
{
	GameObjectPosition = gameObjectPosition;
}

void GameObject::SetGameObjectRotation(glm::vec3 gameObjectRotation)
{
	GameObjectRotation = gameObjectRotation;
}

void GameObject::SetGameObjectScale(glm::vec3 gameObjectScale)
{
	GameObjectScale = gameObjectScale;
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

