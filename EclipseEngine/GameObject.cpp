#include "GameObject.h"
#include <glm/ext/matrix_transform.hpp>

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

GameObject::GameObject(const std::string Name, const std::string fileName)
{
	ObjectName = Name;
	//RenderType = renderType;
	GenerateID();
	
	model = std::make_shared<GLTF_Temp_Model>(GLTF_Temp_Model());
	model->LoadModel<Vertex3D>(fileName, ModelTypeEnum::kPolygon, GameObjectTransform, GameObjectID);
}

GameObject::GameObject(const std::string Name, const std::string fileName, const glm::vec3& Position)
{
	GenerateID();
	ObjectName = Name;
	//RenderType = renderType;

	GameObjectPosition = Position;

	GameObjectTransform = glm::mat4(1.0f);
	GameObjectTransform = glm::translate(GameObjectTransform, GameObjectPosition);
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	GameObjectTransform = glm::scale(GameObjectTransform, GameObjectScale);

	model = std::make_shared<GLTF_Temp_Model>(GLTF_Temp_Model());
	model->LoadModel<Vertex3D>(fileName, ModelTypeEnum::kPolygon, GameObjectTransform, GameObjectID);
}

GameObject::GameObject(const std::string Name, const std::string fileName, const glm::vec3& Position, const glm::vec3& Rotation)
{
	GenerateID();
	ObjectName = Name;
	//RenderType = renderType;

	GameObjectPosition = Position;
	GameObjectRotation = Rotation;

	GameObjectTransform = glm::mat4(1.0f);
	GameObjectTransform = glm::translate(GameObjectTransform, GameObjectPosition);
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	GameObjectTransform = glm::scale(GameObjectTransform, GameObjectScale);

	model = std::make_shared<GLTF_Temp_Model>(GLTF_Temp_Model());
	model->LoadModel<Vertex3D>(fileName, ModelTypeEnum::kPolygon, GameObjectTransform, GameObjectID);
}

GameObject::GameObject(const std::string Name, const std::string fileName, const glm::vec3& Position, const glm::vec3& Rotation, const glm::vec3& Scale)
{
	GenerateID();
	ObjectName = Name;
	//RenderType = renderType;

	GameObjectPosition = Position;
	GameObjectRotation = Rotation;
	GameObjectScale = Scale;

	GameObjectTransform = glm::mat4(1.0f);
	GameObjectTransform = glm::translate(GameObjectTransform, GameObjectPosition);
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	GameObjectTransform = glm::scale(GameObjectTransform, GameObjectScale);

	model = std::make_shared<GLTF_Temp_Model>(GLTF_Temp_Model());
	model->LoadModel<Vertex3D>(fileName, ModelTypeEnum::kPolygon, GameObjectTransform, GameObjectID);
}

GameObject::~GameObject()
{
}

void GameObject::Draw(VkCommandBuffer& commandBuffer, VkPipelineLayout shaderPipelineLayout)
{
	model->Draw(commandBuffer, shaderPipelineLayout);
}

void GameObject::Update(float DeltaTime)
{
	GameObjectTransform = glm::mat4(1.0f);
	GameObjectTransform = glm::translate(GameObjectTransform, GameObjectPosition);
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	GameObjectTransform = glm::rotate(GameObjectTransform, glm::radians(GameObjectRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	GameObjectTransform = glm::scale(GameObjectTransform, GameObjectScale);

	model->Update(GameObjectTransform);
}

void GameObject::Destroy()
{
	for (int x = ComponentList.size() - 1; x >= 0; x--)
	{
		ComponentList[x]->Destroy();
		ComponentList.erase(ComponentList.begin() + x);
	}
	model->Destroy();
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

