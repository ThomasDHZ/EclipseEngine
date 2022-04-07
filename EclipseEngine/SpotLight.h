#pragma once
#include "Light.h"
#include "UniformBuffer.h"

class SpotLight : public Light<SpotLightBuffer>
{
public:
	SpotLight();
	SpotLight(SpotLightBuffer light);
	~SpotLight();

	void Update() override;
	void Destroy() override;
};