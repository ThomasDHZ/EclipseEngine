#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform3D : public Component
{
private:

protected:
 

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

    virtual nlohmann::json ToJson() override
    {
        nlohmann::json json = Component::ToJson();

        json["Position"] = { Position.x, Position.y, Position.z };
        json["Rotation"] = { Rotation.x, Rotation.y, Rotation.z };
        json["Scale"] = { Scale.x, Scale.y, Scale.z };

        return json;
    }
};

