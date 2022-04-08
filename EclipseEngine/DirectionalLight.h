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

	glm::vec3* GetPositionPtr() { return &LightBuffer.UniformDataInfo.position; }
	glm::vec3* GetDirectionPtr() { return &LightBuffer.UniformDataInfo.direction; }
	glm::vec3* GetAmbientPtr() { return &LightBuffer.UniformDataInfo.ambient; }
	glm::vec3* GetDiffusePtr() { return &LightBuffer.UniformDataInfo.diffuse; }
	glm::vec3* GetSpecularPtr() { return &LightBuffer.UniformDataInfo.specular; }
};

