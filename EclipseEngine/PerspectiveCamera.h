#pragma once
#include "Camera.h"

class PerspectiveCamera : public Camera
{
protected:
    float YAW = -90.0f;
    float PITCH = 0.0f;

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
    PerspectiveCamera(std::string Name, glm::vec2 ScreenSize, glm::vec3 position);
    PerspectiveCamera(std::string Name, glm::vec2 ScreenSize, glm::vec3 position, float pitch, float yaw);
    ~PerspectiveCamera();

    void Update(float deltaTime) override;
    void UpdateKeyboard(float deltaTime) override;
    void UpdateMouse(glm::vec2 mouseCoords, bool constrainPitch = true) override;
    virtual void MouseScroll(float yoffset);

    glm::vec3 GetFront() { return Front; }
};

