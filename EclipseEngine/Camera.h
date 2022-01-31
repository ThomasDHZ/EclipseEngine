#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VulkanRenderer.h"

enum CameraType
{
    Perspective_Camera,
    Orthographic_Camera
};

class Camera
{
protected:
    std::string CameraName;
    glm::vec3 Position;
    CameraType cameraType;

    float Width;
    float Height;
    float Aspect;

    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;
    glm::vec2 ViewScreenSize;

    float Zoom;
    float MovementSpeed = 5.0f;

public:
    Camera();
    Camera(std::string Name);
    ~Camera();

    virtual void Update(float deltaTime) = 0;
    virtual void UpdateKeyboard(float deltaTime) = 0;
    virtual void UpdateMouse(glm::vec2 mouseCoords, bool constrainPitch = true) = 0;

    void UpdateScreenSize(int NewWidth, int NewHeight);
    void UpdateScreenSize(glm::vec2& ScreenSize);

    void SetPosition(const glm::vec2& position);
    void SetPosition(const glm::vec3& position);
    void SetPosition(float x, float y);
    void SetPosition(float x, float y, float z);
    void SetZoom(float zoom);

    const glm::vec2& GetViewScreenSize() { return ViewScreenSize; }
    const glm::mat4& GetProjectionMatrix() { return ProjectionMatrix; }
    const glm::mat4& GetViewMatrix() { return ViewMatrix; }
    const glm::vec3& GetPosition() { return Position; }
    const float GetZoom() { return Zoom; }
};

