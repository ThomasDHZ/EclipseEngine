#pragma once
#include "Component.h"
#include "ModelManager.h"

class ComponentRenderer : public Component
{
private:
protected:
    std::shared_ptr<Model> model;

public:
    ComponentRenderer();
    ComponentRenderer(uint64_t GameObjectID, ComponentType componentType);

    void Update(float DeltaTime) override;
    void UpdateMeshProperties();
    void Destroy() override;


    std::shared_ptr<Model> GetModel() { return model; }

    virtual nlohmann::json ToJson() override
    {
        nlohmann::json json = Component::ToJson();

      /*  json["Mesh"] = mesh->ToJson();*/

        return json;
    }
};

