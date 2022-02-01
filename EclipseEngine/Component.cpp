#include "Component.h"

Component::Component()
{
}

Component::Component(ComponentType type)
{
	componentType = type;
}

Component::~Component()
{
}

void Component::Update(float DeltaTime)
{
}

