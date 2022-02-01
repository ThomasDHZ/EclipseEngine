#pragma once
#include "VulkanRenderer.h"

enum ComponentType
{
	kComponent,
	kSpriteRenderer,
	kMeshRenderer
};
class Component
{
private:
	ComponentType componentType;
public:
	Component();
	Component(ComponentType type);
	virtual ~Component();

	virtual void Update(float DeltaTime);

	ComponentType GetComponentType() { return componentType; }
};

