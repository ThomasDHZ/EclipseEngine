#pragma once
#include "Component.h"
#include "Transform.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform2D : public Transform
{
private:

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
	uint32_t ZIndex;

    virtual nlohmann::json ToJson() override
    {
        nlohmann::json json = Component::ToJson();

        json["Position"] = { Position.x, Position.y };
        json["Rotation"] = { Rotation.x, Rotation.y };
        json["Scale"] = { Scale.x, Scale.y };
        json["ZIndex"] = ZIndex;

        return json;
    }
};

