#pragma once
#include "Light.h"
#include "UniformBuffer.h"
#include "RenderedDepthTexture.h"
#include "RenderedDepthTexture.h"
#include <glm/gtc/matrix_transform.hpp>

class GLTFSpotLight : public Light<GLTFSpotLightBuffer>
{
private:
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

	std::shared_ptr<RenderedDepthTexture> LightViewTexture;

public:
	GLTFSpotLight();
	GLTFSpotLight(const std::string name, glm::vec3 Position, glm::vec3 Direction, glm::vec3 DiffuseColor, float Intesity);
	GLTFSpotLight(GLTFSpotLightBuffer light);
	~GLTFSpotLight();

	void Update() override;
	void Destroy() override;

	void SetLightViewTexture(std::shared_ptr<RenderedDepthTexture> lightViewTexture);

	std::shared_ptr<RenderedDepthTexture> GetLightViewTexture() { return LightViewTexture; }

	glm::vec3* GetPositionPtr() { return &LightBuffer.UniformDataInfo.position; }
	glm::vec3* GetDirectionPtr() { return &LightBuffer.UniformDataInfo.direction; }
	glm::vec3* GetDiffusePtr() { return &LightBuffer.UniformDataInfo.diffuse; }
	float* GetIntensityPtr() { return &LightBuffer.UniformDataInfo.intensity; }
};