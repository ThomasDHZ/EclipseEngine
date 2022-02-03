#pragma once
#include "Component.h"
#include "Mesh.h"

class ComponentRenderer : public Component
{
protected:
public:
	Mesh mesh;
	ComponentRenderer(ComponentType componentType);

	void Update(float DeltaTime) override;
	void Destroy() override;
};

