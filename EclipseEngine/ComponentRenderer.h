#pragma once
#include "Component.h"

class ComponentRenderer : public Component
{
private:

protected:

public:
    ComponentRenderer();
    ComponentRenderer(ComponentType componentType);

    void Update(float DeltaTime) override;
    void UpdateMeshProperties();
    void Destroy() override;

    virtual nlohmann::json ToJson() override
    {
        nlohmann::json json = Component::ToJson();

      /*  json["Mesh"] = mesh->ToJson();*/

        return json;
    }
};

