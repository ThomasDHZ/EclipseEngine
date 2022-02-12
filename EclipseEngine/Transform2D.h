#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform2D : public Component
{
private:

    void FromJson(const nlohmann::json& json) override
    {
        json.at("ComponentID").get_to(ComponentID);
        json.at("componentType").get_to(componentType);
        json.at("Position")[0].get_to(Position.x);
        json.at("Position")[1].get_to(Position.y);
        json.at("Rotation")[0].get_to(Rotation.x);
        json.at("Rotation")[1].get_to(Rotation.y);
        json.at("Scale")[0].get_to(Scale.x);
        json.at("Scale")[1].get_to(Scale.y);
        json.at("ZIndex").get_to(ZIndex);
    }

public:
	Transform2D();
	Transform2D(glm::vec2 position, float zIndex);
	Transform2D(glm::vec2 position, glm::vec2 rotation, float zIndex);
	Transform2D(glm::vec2 position, glm::vec2 rotation, glm::vec2 scale, float zIndex);
    Transform2D(const nlohmann::json& json);

	virtual ~Transform2D();

	void Update(float DeltaTime) override;
	void Destroy() override;

	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);
	glm::mat4 Transform = glm::mat4(1.0f);
	uint32_t ZIndex;

    virtual void ToJson(nlohmann::json& j) override
    {
        j["ComponentID"] = ComponentID;
        j["Position"] = { Position.x, Position.y };
        j["Rotation"] = { Rotation.x, Rotation.y };
        j["Scale"] = { Scale.x, Scale.y };
        j["ZIndex"] = ZIndex;
    }
};

