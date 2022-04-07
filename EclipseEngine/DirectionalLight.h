#pragma once
#include "Light.h"
#include "UniformBuffer.h"

class DirectionalLight : public Light<DirectionalLightBuffer>
{
public:
	DirectionalLight();
	DirectionalLight(DirectionalLightBuffer light);
	~DirectionalLight();

	void Update();
	void Destroy() override;
};

