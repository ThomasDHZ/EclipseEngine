#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VulkanRenderer.h"

class Camera
{
protected:
    std::string CameraName;
    glm::vec3 Position;

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
    Camera(std::string Name, float width, float height, float zoom);
    ~Camera();

    void Update(float deltaTime);
    void UpdateScreenSize(int NewWidth, int NewHeight);
    void UpdateScreenSize(glm::vec2& ScreenSize);
    void ProcessKeyboard(float deltaTime);

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

