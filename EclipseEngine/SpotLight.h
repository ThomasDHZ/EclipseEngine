#pragma once
#include "Light.h"
#include "UniformBuffer.h"
#include "RenderedDepthTexture.h"

class SpotLight : public Light<SpotLightBuffer>
{
private:
	std::shared_ptr<RenderedDepthTexture> LightViewTexture;

public:
	SpotLight();
	SpotLight(SpotLightBuffer light);
	~SpotLight();

	void Update() override;
	void Destroy() override;

	void SetLightViewTexture(std::shared_ptr<RenderedDepthTexture> lightViewTexture);

	std::shared_ptr<RenderedDepthTexture> GetLightViewTexture() { return LightViewTexture; }

	glm::vec3* GetPositionPtr() { return &LightBuffer.UniformDataInfo.position; }
	glm::vec3* GetDirectionPtr() { return &LightBuffer.UniformDataInfo.direction; }
	glm::vec3* GetDiffusePtr() { return &LightBuffer.UniformDataInfo.diffuse; }
	glm::vec3* GetSpecularPtr() { return &LightBuffer.UniformDataInfo.specular; }
};