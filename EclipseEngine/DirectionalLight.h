#pragma once
#include "Light.h"
#include "UniformBuffer.h"
#include "RenderedDepthTexture.h"

class DirectionalLight : public Light<GLTFDirectionalLightBuffer>
{
private:

public:
	DirectionalLight();
	DirectionalLight(const std::string name, glm::vec3 Direction, glm::vec3 DiffuseColor, float Intesity);
	DirectionalLight(GLTFDirectionalLightBuffer light);
	~DirectionalLight();

	glm::vec2 LeftRight = glm::vec2(-6.396, 17.941f);
	glm::vec2 NearFar = glm::vec2(100, -8.580f);
	glm::vec2 TopBottom = glm::vec2(-7.644f, 11.388f);

	std::shared_ptr<RenderedDepthTexture> LightViewTexture;

	void Update();
	void Destroy() override;

	void SetLightViewTexture(std::shared_ptr<RenderedDepthTexture> lightViewTexture);

	std::shared_ptr<RenderedDepthTexture> GetLightViewTexture() { return LightViewTexture; }

	glm::vec3* GetDiffusePtr() { return &LightBuffer.UniformDataInfo.diffuse; }
	glm::vec3* GetDirectionPtr() { return &LightBuffer.UniformDataInfo.direction; }
	float* GetIntensityPtr() { return &LightBuffer.UniformDataInfo.intensity; }

	glm::vec2* GetLeftRightPtr() { return &LeftRight; }
	glm::vec2* GetTopBottomPtr() { return &TopBottom; }
	glm::vec2* GetNearFarPtr() { return &NearFar; }
};