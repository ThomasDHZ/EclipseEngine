#include "SpotLight.h"
SpotLight::SpotLight() : Light<GLTFSpotLightBuffer>()
{

}

SpotLight::SpotLight(const std::string name, glm::vec3 Position, glm::vec3 Direction, glm::vec3 DiffuseColor, float Intesity) : Light<GLTFSpotLightBuffer>(LightTypeEnum::kSpotLight)
{
	Up = glm::vec3(0.0f, 1.0f, 0.0f);
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	Yaw = -90.0f;
	Pitch = 0.0f;

	LightName = name;
	LightBuffer.UniformDataInfo.diffuse = DiffuseColor;
	LightBuffer.UniformDataInfo.position = Position;
	LightBuffer.UniformDataInfo.direction = Direction;
	LightBuffer.UniformDataInfo.intensity = Intesity;
	LightBuffer.Update();
}

SpotLight::SpotLight(GLTFSpotLightBuffer light) : Light<GLTFSpotLightBuffer>(LightTypeEnum::kSpotLight, light)
{
	LightBuffer.Update(light);
}

SpotLight::~SpotLight()
{
}

void SpotLight::Update()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);

	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));

	ViewMatrix = glm::lookAt(LightBuffer.UniformDataInfo.position, LightBuffer.UniformDataInfo.position + Front, Up);

	ProjectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, ZNear, ZFar);
	ProjectionMatrix[1][1] *= -1;

	Light::Update();
}

void SpotLight::Destroy()
{
	Light::Destroy();
}

void SpotLight::SetLightViewTexture(std::shared_ptr<RenderedDepthTexture> lightViewTexture)
{
	LightViewTexture = lightViewTexture;
}
