#pragma once
#include "Camera.h"

class PerspectiveCamera : public Camera
{
protected:
    float YAW = -90.0f;
    float PITCH = 0.0f;
    float ZNear = 0.1f;
    float ZFar = 10000.0f;
    float Yaw;
    float Pitch;

    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float MovementSpeed;
    float MouseSensitivity;

public:
    PerspectiveCamera();
    PerspectiveCamera(std::string Name, const glm::vec2& ScreenSize, const glm::vec3& position);
    PerspectiveCamera(std::string Name, const glm::vec2& ScreenSize, const glm::vec3& position, float pitch, float yaw);
    ~PerspectiveCamera();

    void Update(float deltaTime) override;
    void UpdateKeyboard(float deltaTime) override;
    void UpdateMouse() override;

    glm::vec3 GetFront() { return Front; }
};

