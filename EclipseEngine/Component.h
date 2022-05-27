#pragma once
#include "VulkanRenderer.h"
#include <json.hpp>

enum ComponentType
{
	kNullComponent,
	kLineRenderer,
	kSpriteRenderer,
	kMeshRenderer,
	kTransform2D,
	kTransform3D
};

enum ComponentSubType
{
	kRenderedObject,
	kTransform,
};


class Component
{
private:
	static uint64_t ComponentIDCounter;

protected:
	uint64_t ComponentID;
	uint64_t ParentGameObjectID;
	ComponentType componentType;
	ComponentSubType componentSubType;

public:

	Component();
	Component(uint64_t GameObjectID, ComponentType type, ComponentSubType componentSubType);
	Component(const nlohmann::json& json);
	virtual ~Component();

	virtual void Update(float DeltaTime)
	{}
	virtual void Destroy()
	{}

	void GenerateID();

	ComponentType GetComponentType() { return componentType; }
	ComponentSubType GetComponentSubType() { return componentSubType; }
	uint64_t GetComponentID() { return ComponentID; }

	virtual void to_json(nlohmann::json& json)
	{
		JsonConverter::to_json(json["componentType"], componentType);
		JsonConverter::to_json(json["componentSubType"], componentSubType);
	}
};

