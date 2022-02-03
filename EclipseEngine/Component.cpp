#include "Component.h"

uint64_t Component::ComponentIDCounter = 0;

Component::Component()
{
	GameObjectPtr = 0;
	ComponentID = 0;
	componentType = kNullComponent;

}

Component::Component(ComponentType type)
{
	//GameObjectPtr = gameObjectPtr;
	ComponentID = 0;
	componentType = type;
}

Component::~Component()
{
}

void Component::GenerateID()
{
	ComponentIDCounter++;
	ComponentID = ComponentIDCounter;
}


