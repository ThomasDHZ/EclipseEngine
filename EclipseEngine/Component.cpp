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
	json.at("componentType").get_to(componentType);
}

Component::~Component()
{
}

void Component::GenerateID()
{
	ComponentIDCounter++;
	ComponentID = ComponentIDCounter;
}


