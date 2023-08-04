#include "DirectionalLight.h"
#include <glm/gtc/matrix_transform.hpp>

DirectionalLight::DirectionalLight() : Light<DirectionalLightBuffer>()
{

}

DirectionalLight::DirectionalLight(DirectionalLightBuffer light) : Light<DirectionalLightBuffer>(LightTypeEnum::kDirectionalLight, light)
{
	LightBuffer.Update(light);
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::Update()
{
	glm::mat4 ProjectionMatrix = glm::ortho(LeftRight.x, LeftRight.y, TopBottom.x, TopBottom.y, NearFar.x, NearFar.y);
	ProjectionMatrix[1][1] *= -1;

	glm::mat4 ViewMatrix = glm::lookAt(-LightBuffer.UniformDataInfo.direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	LightBuffer.UniformDataInfo.LightSpaceMatrix = ProjectionMatrix * ViewMatrix;

	Light::Update();
}

void DirectionalLight::Destroy()
{
	Light::Destroy();
}

void DirectionalLight::SetLightViewTexture(std::shared_ptr<RenderedDepthTexture> lightViewTexture)
{
	LightViewTexture = lightViewTexture;
}
