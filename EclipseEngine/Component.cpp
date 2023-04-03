#include "Component.h"

uint64_t Component::ComponentIDCounter = 0;

Component::Component()
{
	ComponentID = 0;
	ParentGameObjectID = 0;
	componentType = kNullComponent;
}

Component::Component(ComponentType type)
{
	ComponentID = 0;
	ParentGameObjectID = 0;
	componentType = type;
}

Component::Component(uint64_t GameObjectID, ComponentType type, ComponentSubType subtype)
{
	ComponentID = 0;
	ParentGameObjectID = GameObjectID;
	componentType = type;
	componentSubType = subtype;
}

Component::~Component()
{
}

void Component::GenerateID()
{
	ComponentIDCounter++;
	ComponentID = ComponentIDCounter;
}


