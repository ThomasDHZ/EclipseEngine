#include "Component.h"

uint64_t Component::ComponentIDCounter = 0;

Component::Component()
{
	ComponentID = 0;
	componentType = kNullComponent;

}

Component::Component(ComponentType type)
{
	ComponentID = 0;
	componentType = type;
}

Component::Component(const nlohmann::json& json)
{
	FromJson(json);
}

Component::~Component()
{
}

void Component::GenerateID()
{
	ComponentIDCounter++;
	ComponentID = ComponentIDCounter;
}


