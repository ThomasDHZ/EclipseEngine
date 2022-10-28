#include "GameObject.h"
#include "SpriteRenderer.h"

uint64_t GameObject::GameObjectIDCounter = 0;

GameObject::GameObject()
{
}

GameObject::GameObject(const std::string Name, GameObjectRenderType renderType)
{
	ObjectName = Name;
	RenderType = renderType;
	GenerateID();
}

GameObject::GameObject(const std::string Name, GameObjectRenderType renderType, const glm::vec3& Position)
{
	GenerateID();
	ObjectName = Name;
	RenderType = renderType;
	
	GameObjectPosition = Position;

	GameObjectTransform = glm::mat4(1.0f);
	GameObjectTransform = glm::translate(GameObjectTransform, GameObjectPosition);
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	GameObjectTransform = glm::scale(GameObjectTransform, GameObjectScale);
}

GameObject::GameObject(const std::string Name, GameObjectRenderType renderType, const glm::vec3& Position, const glm::vec3& Rotation)
{
	GenerateID();
	ObjectName = Name;
	RenderType = renderType;

	GameObjectPosition = Position;
	GameObjectRotation = Rotation;

	GameObjectTransform = glm::mat4(1.0f);
	GameObjectTransform = glm::translate(GameObjectTransform, GameObjectPosition);
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	GameObjectTransform = glm::scale(GameObjectTransform, GameObjectScale);
}

GameObject::GameObject(const std::string Name, GameObjectRenderType renderType, const glm::vec3& Position, const glm::vec3& Rotation, const glm::vec3& Scale)
{
	GenerateID();
	ObjectName = Name;
	RenderType = renderType;

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
	GenerateID();
	json.at("ObjectName").get_to(ObjectName);
	JsonConverter::from_json(json["GameObjectPosition"], GameObjectPosition);
	JsonConverter::from_json(json["GameObjectRotation"], GameObjectRotation);
	JsonConverter::from_json(json["GameObjectScale"], GameObjectScale);
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

void GameObject::LoadPrefab(nlohmann::json& json)
{
	ObjectName = json.at("ObjectName");
}

void GameObject::SaveAsPrefab(nlohmann::json& json)
{
	JsonConverter::to_json(json["ObjectName"], ObjectName);
}

void GameObject::Update(float DeltaTime)
{
	GameObjectTransform = glm::mat4(1.0f);
	GameObjectTransform = glm::translate(GameObjectTransform, GameObjectPosition);
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	GameObjectTransform = glm::scale(GameObjectTransform, GameObjectScale);
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

void GameObject::SetGameObjectPosition(const glm::vec3& gameObjectPosition)
{
	GameObjectPosition = gameObjectPosition;
}

void GameObject::SetGameObjectRotation(const glm::vec3& gameObjectRotation)
{
	GameObjectRotation = gameObjectRotation;
}

void GameObject::SetGameObjectScale(const glm::vec3& gameObjectScale)
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

