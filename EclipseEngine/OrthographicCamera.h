#pragma once
#include "Camera.h"

class OrthographicCamera : public Camera
{
public:
	OrthographicCamera();
	OrthographicCamera(std::string Name);
	OrthographicCamera(std::string Name, float width, float height);
	OrthographicCamera(std::string Name, float width, float height, float zoom);
	OrthographicCamera(std::string Name, const glm::vec2& viewScreenSize);
	OrthographicCamera(std::string Name, const glm::vec2& viewScreenSize, float zoom);
	OrthographicCamera(std::string Name, const glm::vec2& viewScreenSize, const glm::vec2& position);
	OrthographicCamera(std::string Name, const glm::vec2& viewScreenSize, const glm::vec2& position, float zoom);

	virtual void Update(float deltaTime) override;
	virtual void UpdateKeyboard(float deltaTime) override;
	virtual void UpdateMouse() override;
};

