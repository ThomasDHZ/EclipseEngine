#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera()
{
}

PerspectiveCamera::PerspectiveCamera(std::string Name, const glm::vec2& ScreenSize, const glm::vec3& position) : Camera(Name)
{
    Width = ScreenSize.x;
    Height = ScreenSize.y;
    Position = position;
    Up = glm::vec3(0.0f, 1.0f, 0.0f);
    Front = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraType = CameraType::Perspective_Camera;

    MovementSpeed = 0.005f;
    MouseSensitivity = 0.1f;

    Position = position;
    WorldUp = Up;

    Zoom = 45.0f;
    Yaw = -90.0f;
    Pitch = 0.0f;
}

PerspectiveCamera::PerspectiveCamera(std::string Name, const glm::vec2& ScreenSize, const glm::vec3& position, float pitch, float yaw) : Camera(Name)
{
    Width = ScreenSize.x;
    Height = ScreenSize.y;
    Position = position;
    Up = glm::vec3(0.0f, 1.0f, 0.0f);
    Front = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraType = CameraType::Perspective_Camera;

    MovementSpeed = 0.5f;
    MouseSensitivity = 0.1f;

    Position = position;
    WorldUp = Up;

    Zoom = 45.0f;
    Yaw = pitch;
    Pitch = yaw;
}

PerspectiveCamera::~PerspectiveCamera()
{
}

void PerspectiveCamera::UpdateKeyboard(float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (Keyboard::IsKeyPressed(GLFW_KEY_W))
    {
        Position += Front * velocity;
    }
    if (Keyboard::IsKeyPressed(GLFW_KEY_S))
    {
        Position -= Front * velocity;
    }
    if (Keyboard::IsKeyPressed(GLFW_KEY_A))
    {
        Position -= Right * velocity;
    }
    if (Keyboard::IsKeyPressed(GLFW_KEY_D))
    {
        Position += Right * velocity;
    }
}

void PerspectiveCamera::UpdateMouse()
{
    float xoffset = 0.0f;
    float yoffset = 0.0f;

    if (glfwGetMouseButton(Window::GetWindowPtr(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        double XPos = Mouse::GetMouseCoords().x;
        double YPos = Mouse::GetMouseCoords().y;

        xoffset = XPos - Mouse::GetLastMouseCoords().x;
        yoffset = Mouse::GetLastMouseCoords().y - YPos;

        Mouse::SetLastMouseCoords(glm::vec2(XPos, YPos));
    }

    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;


    if (Pitch > 89.9999f)
    {
        Pitch = 89.9999f;
    }
    if (Pitch < -89.9999f)
    {
        Pitch = -89.9999f;
    }

}

void PerspectiveCamera::Update(float deltaTime)
{
    UpdateKeyboard(deltaTime);
    UpdateMouse();
    UpdateMouseScroll();

    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));

    ViewMatrix = glm::lookAt(Position, Position + Front, Up);

    const auto 	Aspect = Width / Height;
    ProjectionMatrix = glm::perspective(glm::radians(Zoom), Aspect, ZNear, ZFar);
    ProjectionMatrix[1][1] *= -1;
}
