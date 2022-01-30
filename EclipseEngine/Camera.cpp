#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(std::string Name, float width, float height, float zoom)
{
    CameraName = Name;
    Width = width;
    Height = height;
    Aspect = Width / Height;
    Zoom = zoom;

    Position = glm::vec3(0.0f);
    ViewScreenSize = glm::vec2(width, height);
    ProjectionMatrix = glm::ortho(-ViewScreenSize.x * Zoom, ViewScreenSize.x * Zoom, -ViewScreenSize.y * Zoom, ViewScreenSize.y * Zoom, -1.0f, 1.0f);
    ViewMatrix = glm::mat4(1.0f);
}

Camera::~Camera()
{
}

void Camera::Update(float deltaTime)
{
    ProcessKeyboard(deltaTime);

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), Position) * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0, 0, 1));
    ViewMatrix = glm::inverse(transform);

    const auto 	Aspect = Width/Height;
    ProjectionMatrix = glm::ortho(-Aspect * Zoom, Aspect * Zoom, -1.0f * Zoom, 1.0f * Zoom, -10.0f, 10.0f);
    ProjectionMatrix[1][1] *= -1;

    ViewScreenSize = glm::vec2((Aspect * Zoom) * 2, (1.0f * Zoom) * 2);
}

void Camera::UpdateScreenSize(int NewWidth, int NewHeight)
{
}

void Camera::ProcessKeyboard(float deltaTime)
{
    //float velocity = MovementSpeed * deltaTime;
    //if(Keyboard::IsKeyPressed(GLFW_KEY_E))
    //{
    //    Zoom += 5.0f * velocity;
    //}
    //if (Keyboard::IsKeyPressed(GLFW_KEY_E))
    //{
    //    Zoom -= 5.0f * velocity;
    //}
    //if (Keyboard::IsKeyPressed(GLFW_KEY_W))
    //{
    //    Position.y += 1.0f * velocity;
    //}
    //if (Keyboard::IsKeyPressed(GLFW_KEY_S))
    //{
    //    Position.y -= 1.0f * velocity;
    //}
    //if (Keyboard::IsKeyPressed(GLFW_KEY_A))
    //{
    //    Position.x -= 1.0f * velocity;
    //}

        Position.x += 0.01f ;
    
}

void Camera::SetPosition(const glm::vec2& position)
{
    Position = glm::vec3(position, Position.z);
}

void Camera::SetPosition(const glm::vec3& position)
{
    Position = position;
}

void Camera::SetPosition(float x, float y)
{
    Position = glm::vec3(x, y, Position.z);
}

void Camera::SetPosition(float x, float y, float z)
{
    Position = glm::vec3(x, y, z);
}

void Camera::SetZoom(float zoom)
{
    Zoom = zoom;
}

void Camera::UpdateScreenSize(glm::vec2& ScreenSize)
{
}
