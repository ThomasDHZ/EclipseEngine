#include "GLTFDirectionalLight.h"
#include <glm/gtc/matrix_transform.hpp>

GLTFDirectionalLight::GLTFDirectionalLight() : Light<GLTFDirectionalLightBuffer>()
{

}

GLTFDirectionalLight::GLTFDirectionalLight(GLTFDirectionalLightBuffer light) : Light<GLTFDirectionalLightBuffer>(light)
{
	LightBuffer.Update(light);
}

GLTFDirectionalLight::~GLTFDirectionalLight()
{
}

void GLTFDirectionalLight::Update()
{
	glm::mat4 ProjectionMatrix = glm::ortho(LeftRight.x, LeftRight.y, TopBottom.x, TopBottom.y, NearFar.x, NearFar.y);
	ProjectionMatrix[1][1] *= -1;

	glm::mat4 ViewMatrix = glm::lookAt(-LightBuffer.UniformDataInfo.direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	LightBuffer.UniformDataInfo.LightSpaceMatrix = ProjectionMatrix * ViewMatrix;

	Light::Update();
}

void GLTFDirectionalLight::Destroy()
{
	Light::Destroy();
}

void GLTFDirectionalLight::SetLightViewTexture(std::shared_ptr<RenderedDepthTexture> lightViewTexture)
{
	LightViewTexture = lightViewTexture;
}
