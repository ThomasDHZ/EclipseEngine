#pragma once
#include "VulkanRenderer.h"

enum ComponentType
{
	kNullComponent,
	kSpriteRenderer,
	kMeshRenderer,
	kTransform2D,
	kTransform3D
};
class Component
{
private:
	static uint64_t ComponentIDCounter;
	uint64_t GameObjectPtr;

	ComponentType componentType;
	uint64_t ComponentID;

public:
	Component();
	Component(ComponentType type);
	virtual ~Component();

	virtual void Update(float DeltaTime) = 0;
	virtual void Destroy() = 0;

	void GenerateID();

	ComponentType GetComponentType() { return componentType; }
	uint64_t GetComponentID() { return ComponentID; }
};

