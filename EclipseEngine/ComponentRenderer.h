#pragma once
#include "Component.h"
#include "ModelManager.h"

class ComponentRenderer : public Component
{
private:
protected:

public:
    ComponentRenderer();
    ComponentRenderer(uint64_t GameObjectID, ComponentType componentType);

    void Update(float DeltaTime) override;
    void UpdateMeshProperties();
    void Destroy() override;
};

