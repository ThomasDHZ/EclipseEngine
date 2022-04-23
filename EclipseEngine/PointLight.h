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

	glm::vec3* GetPositionPtr() { return &LightBuffer.UniformDataInfo.position; }
	glm::vec3* GetDiffusePtr() { return &LightBuffer.UniformDataInfo.diffuse; }
	glm::vec3* GetSpecularPtr() { return &LightBuffer.UniformDataInfo.specular; }
};
