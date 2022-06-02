#pragma once
#include "Component.h"
#include "ModelManager.h"

class ComponentRenderer : public Component
{
private:
protected:
    std::shared_ptr<Model> model;

    void from_json(nlohmann::json& json)
    {
       Component::from_json(json);
       model = std::make_shared<Model>(Model(json.at("Model"), ParentGameObjectID));
       ModelManager::AddModel(model);
    }

public:
    ComponentRenderer();
    ComponentRenderer(uint64_t GameObjectID, ComponentType componentType);

    void Update(float DeltaTime) override;
    void UpdateMeshProperties();
    void Destroy() override;


    std::shared_ptr<Model> GetModel() { return model; }

    virtual void to_json(nlohmann::json& json) override
    {
        Component::to_json(json);
        model->to_json(json["Model"]);
    }
};

