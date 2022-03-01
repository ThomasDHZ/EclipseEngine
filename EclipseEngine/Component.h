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


class Component
{
private:
	static uint64_t ComponentIDCounter;

protected:
	uint64_t ComponentID;
	ComponentType componentType;

public:

	Component();
	Component(ComponentType type);
	Component(const nlohmann::json& json);
	virtual ~Component();

	virtual void Update(float DeltaTime)
	{}
	virtual void Destroy()
	{}

	void GenerateID();

	ComponentType GetComponentType() { return componentType; }
	uint64_t GetComponentID() { return ComponentID; }

	virtual nlohmann::json ToJson()
	{
		nlohmann::json json;

		json["componentType"] = componentType;

		return json;
	}
};

