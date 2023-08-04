#include "GLTFSpotLight.h"
GLTFSpotLight::GLTFSpotLight() : Light<GLTFSpotLightBuffer>()
{

}

GLTFSpotLight::GLTFSpotLight(const std::string name, glm::vec3 Position, glm::vec3 Direction, glm::vec3 DiffuseColor, float Intesity)
{
	LightName = name;
	LightBuffer.UniformDataInfo.diffuse = DiffuseColor;
	LightBuffer.UniformDataInfo.position = Position;
	LightBuffer.UniformDataInfo.direction = Direction;
	LightBuffer.UniformDataInfo.intensity = Intesity;
	LightBuffer.Update();
}

GLTFSpotLight::GLTFSpotLight(GLTFSpotLightBuffer light) : Light<GLTFSpotLightBuffer>(LightTypeEnum::kSpotLight, light)
{
	LightBuffer.Update(light);
}

GLTFSpotLight::~GLTFSpotLight()
{
}

void GLTFSpotLight::Update()
{
	Light::Update();
}

void GLTFSpotLight::Destroy()
{
	Light::Destroy();
}

void GLTFSpotLight::SetLightViewTexture(std::shared_ptr<RenderedDepthTexture> lightViewTexture)
{
	LightViewTexture = lightViewTexture;
}
