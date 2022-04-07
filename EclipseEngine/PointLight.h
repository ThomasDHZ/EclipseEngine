#pragma once
#include "Light.h"
#include "UniformBuffer.h"

class PointLight : public Light<PointLightBuffer>
{
public:
	PointLight();
	PointLight(PointLightBuffer light);
	~PointLight();

	void Update() override;
	void Destroy() override;
};
