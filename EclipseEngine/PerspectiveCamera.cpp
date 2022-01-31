#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera()
{
}

PerspectiveCamera::PerspectiveCamera(std::string Name, glm::vec2 ScreenSize, glm::vec3 position) : Camera(Name)
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
    Yaw = -90.0f;
    Pitch = 0.0f;
}

PerspectiveCamera::PerspectiveCamera(std::string Name, glm::vec2 ScreenSize, glm::vec3 position, float pitch, float yaw) : Camera(Name)
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
//    Position.y -= Right * velocity;
//}
//if (Keyboard::IsKeyPressed(GLFW_KEY_A))
//{
//    Position.x -= Right * velocity;
//}
//if (Keyboard::IsKeyPressed(GLFW_KEY_D))
//{
//    Position.x += 1.0f ;
//}
}

void PerspectiveCamera::UpdateMouse(glm::vec2 mouseCoords ,bool constrainPitch)
{
    double XPos = 0;
    double YPos = 0;
    if(Mouse::GetMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        XPos = Mouse::GetMouseCoords().x;
        YPos = Mouse::GetMouseCoords().y;

        float xoffset = XPos - Mouse::LastX;
        float yoffset = Mouse::LastY - YPos;

        Mouse::LastX = XPos;
        Mouse::LastY = YPos;

        mouseCoords.x *= MouseSensitivity;
        mouseCoords.y *= MouseSensitivity;

        Yaw += mouseCoords.x;
        Pitch += mouseCoords.y;

        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }
    }
}

void PerspectiveCamera::MouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void PerspectiveCamera::Update(float deltaTime)
{
    UpdateKeyboard(deltaTime);
    UpdateMouse(Mouse::GetMouseCoords());

    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));

    ViewMatrix = glm::lookAt(Position, Position + Front, Up);

    const auto 	Aspect = Width / Height;
    ProjectionMatrix = glm::perspective(glm::radians(Zoom), Aspect, 0.1f, 10000.0f);
    ProjectionMatrix[1][1] *= -1;
}
