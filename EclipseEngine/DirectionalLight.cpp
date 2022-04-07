#include "DirectionalLight.h"
DirectionalLight::DirectionalLight() : Light<DirectionalLightBuffer>()
{

}

DirectionalLight::DirectionalLight(DirectionalLightBuffer light) : Light<DirectionalLightBuffer>(light)
{
	LightBuffer.Update(light);
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::Update()
{
	Light::Update();
}

void DirectionalLight::Destroy()
{
	Light::Destroy();
}
