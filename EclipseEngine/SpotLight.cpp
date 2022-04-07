#include "SpotLight.h"
SpotLight::SpotLight() : Light<SpotLightBuffer>()
{

}

SpotLight::SpotLight(SpotLightBuffer light) : Light<SpotLightBuffer>(light)
{
	LightBuffer.Update(light);
}

SpotLight::~SpotLight()
{
}

void SpotLight::Update()
{
	Light::Update();
}

void SpotLight::Destroy()
{
	Light::Destroy();
}
