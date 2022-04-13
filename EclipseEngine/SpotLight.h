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

	glm::vec3* GetPositionPtr() { return &LightBuffer.UniformDataInfo.position; }
	glm::vec3* GetDirectionPtr() { return &LightBuffer.UniformDataInfo.direction; }
	glm::vec3* GetAmbientPtr() { return &LightBuffer.UniformDataInfo.ambient; }
	glm::vec3* GetDiffusePtr() { return &LightBuffer.UniformDataInfo.diffuse; }
	glm::vec3* GetSpecularPtr() { return &LightBuffer.UniformDataInfo.specular; }
};