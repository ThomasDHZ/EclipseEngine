#pragma once
#include "Light.h"
#include "UniformBuffer.h"
#include "RenderedDepthTexture.h"

class DirectionalLight : public Light<DirectionalLightBuffer>
{
private:
	std::shared_ptr<RenderedDepthTexture> LightViewTexture;

public:
	DirectionalLight();
	DirectionalLight(DirectionalLightBuffer light);
	~DirectionalLight();

	//glm::vec2 LeftRight = glm::vec2(-25.0f, 25.0f);
	//glm::vec2 TopBottom = glm::vec2(-25.0f, 25.0f);
	//glm::vec2 NearFar = glm::vec2(0.1f, 25.0f);
	glm::vec2 LeftRight = glm::vec2(-180.0f, 192.0f);
	glm::vec2 TopBottom = glm::vec2(-110.0f, 117.0f);
	glm::vec2 NearFar = glm::vec2(-5000.0f, 460.0f);


	void Update();
	void Destroy() override;

	void SetLightViewTexture(std::shared_ptr<RenderedDepthTexture> lightViewTexture);

	std::shared_ptr<RenderedDepthTexture> GetLightViewTexture() { return LightViewTexture; }

	glm::vec3* GetPositionPtr() { return &LightBuffer.UniformDataInfo.position; }
	glm::vec3* GetDirectionPtr() { return &LightBuffer.UniformDataInfo.direction; }
	glm::vec3* GetDiffusePtr() { return &LightBuffer.UniformDataInfo.diffuse; }
	glm::vec3* GetSpecularPtr() { return &LightBuffer.UniformDataInfo.specular; }
	glm::vec2* GetLeftRightPtr() { return &LeftRight; }
	glm::vec2* GetTopBottomPtr() { return &TopBottom; }
	glm::vec2* GetNearFarPtr() { return &NearFar; }
};

