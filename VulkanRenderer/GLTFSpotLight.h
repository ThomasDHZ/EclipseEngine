#pragma once
#include "Light.h"
#include "UniformBuffer.h"
#include "RenderedDepthTexture.h"

class GLTFSpotLight : public Light<GLTFSpotLightBuffer>
{
private:
	std::shared_ptr<RenderedDepthTexture> LightViewTexture;

public:
	GLTFSpotLight();
	GLTFSpotLight(const std::string name, glm::vec3 Position, glm::vec3 Direction, glm::vec3 DiffuseColor, float Intesity);
	GLTFSpotLight(GLTFSpotLightBuffer light);
	~GLTFSpotLight();

	void Update() override;
	void Destroy() override;

	void SetLightViewTexture(std::shared_ptr<RenderedDepthTexture> lightViewTexture);

	std::shared_ptr<RenderedDepthTexture> GetLightViewTexture() { return LightViewTexture; }

	glm::vec3* GetPositionPtr() { return &LightBuffer.UniformDataInfo.position; }
	glm::vec3* GetDirectionPtr() { return &LightBuffer.UniformDataInfo.direction; }
	glm::vec3* GetDiffusePtr() { return &LightBuffer.UniformDataInfo.diffuse; }
	float* GetIntensityPtr() { return &LightBuffer.UniformDataInfo.intensity; }
};