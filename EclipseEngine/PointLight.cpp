#include "PointLight.h"
PointLight::PointLight() : Light<PointLightBuffer>()
{

}

PointLight::PointLight(PointLightBuffer light) : Light<PointLightBuffer>(light)
{
	LightBuffer.Update(light);
}

PointLight::~PointLight()
{
}

void PointLight::Update()
{
	Light::Update();
}

void PointLight::Destroy()
{
	Light::Destroy();
}