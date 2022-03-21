#include "Transform.h"

Transform::Transform() : Component(ComponentType::kNullComponent)
{
}

Transform::Transform(ComponentType componentType) :Component(componentType, ComponentSubType::kTransform)
{
}

Transform::~Transform()
{
}
