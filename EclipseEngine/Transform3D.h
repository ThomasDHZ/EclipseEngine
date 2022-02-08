#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform3D : public Component
{
private:

protected:
    void FromJson(const nlohmann::json& json) override
    {
        json.at("ComponentID").get_to(ComponentID);
        json.at("Position")[0].get_to(Position.x);
        json.at("Position")[1].get_to(Position.y);
        json.at("Position")[2].get_to(Position.z);
        json.at("Rotation")[0].get_to(Rotation.x);
        json.at("Rotation")[1].get_to(Rotation.y);
        json.at("Rotation")[2].get_to(Rotation.z);
        json.at("Scale")[0].get_to(Scale.x);
        json.at("Scale")[1].get_to(Scale.y);
        json.at("Scale")[2].get_to(Scale.z);
    }

public:
	Transform3D();
	Transform3D(glm::vec3 position);
	Transform3D(glm::vec3 position, glm::vec3 rotation);
	Transform3D(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    Transform3D(const nlohmann::json& json);
	virtual ~Transform3D();

	void Update(float DeltaTime) override;
	void Destroy() override;

	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);
	glm::mat4 Transform = glm::mat4(1.0f);

    virtual void ToJson(nlohmann::json& j) override
    {
        j["ComponentID"] = ComponentID;
        j["Position"] = { Position.x, Position.y, Position.z };
        j["Rotation"] = { Rotation.x, Rotation.y, Rotation.z };
        j["Scale"] = { Scale.x, Scale.y, Scale.z };
    }
};

