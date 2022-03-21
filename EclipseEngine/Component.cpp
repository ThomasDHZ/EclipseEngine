#include "Component.h"

uint64_t Component::ComponentIDCounter = 0;

Component::Component()
{
	ComponentID = 0;
	componentType = kNullComponent;

}

Component::Component(ComponentType type, ComponentSubType subtype)
{
	ComponentID = 0;
	componentType = type;
	componentSubType = subtype;
}

Component::Component(const nlohmann::json& json)
{
	json.at("componentType").get_to(componentType);
	json.at("componentSubType").get_to(componentSubType);
}

Component::~Component()
{
}

void Component::GenerateID()
{
	ComponentIDCounter++;
	ComponentID = ComponentIDCounter;
}


