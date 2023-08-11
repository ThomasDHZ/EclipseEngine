#include "DirectionalLight.h"
#include <glm/gtc/matrix_transform.hpp>

DirectionalLight::DirectionalLight() : Light<GLTFDirectionalLightBuffer>()
{

}

DirectionalLight::DirectionalLight(const std::string name, glm::vec3 Direction, glm::vec3 DiffuseColor, float Intesity) : Light<GLTFDirectionalLightBuffer>(LightTypeEnum::kDirectionalLight)
{
	LightName = name;
	LightBuffer.UniformDataInfo.diffuse = DiffuseColor;
	LightBuffer.UniformDataInfo.direction = Direction;
	LightBuffer.UniformDataInfo.intensity = Intesity;
	LightBuffer.Update();
}

DirectionalLight::DirectionalLight(GLTFDirectionalLightBuffer light) : Light<GLTFDirectionalLightBuffer>(LightTypeEnum::kDirectionalLight, light)
{
	LightBuffer.Update(light);
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::Update()
{
	ProjectionMatrix = glm::ortho(LeftRight.x, LeftRight.y, TopBottom.x, TopBottom.y, NearFar.x, NearFar.y);
	ProjectionMatrix[1][1] *= -1;

	ViewMatrix = glm::lookAt(LightBuffer.UniformDataInfo.direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

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
