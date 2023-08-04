#include "SpotLight.h"
SpotLight::SpotLight() : Light<SpotLightBuffer>()
{

}

SpotLight::SpotLight(SpotLightBuffer light) : Light<SpotLightBuffer>(LightTypeEnum::kSpotLight, light)
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

void SpotLight::SetLightViewTexture(std::shared_ptr<RenderedDepthTexture> lightViewTexture)
{
	LightViewTexture = lightViewTexture;
}
