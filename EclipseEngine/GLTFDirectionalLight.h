#pragma once
#include "Light.h"
#include "UniformBuffer.h"
#include "RenderedDepthTexture.h"

class GLTFDirectionalLight : public Light<GLTFDirectionalLightBuffer>
{
private:
	std::shared_ptr<RenderedDepthTexture> LightViewTexture;

public:
	GLTFDirectionalLight();
	GLTFDirectionalLight(GLTFDirectionalLightBuffer light);
	~GLTFDirectionalLight();

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
	float* GetIntensityPtr() { return &LightBuffer.UniformDataInfo.intensity; }

	glm::vec2* GetLeftRightPtr() { return &LeftRight; }
	glm::vec2* GetTopBottomPtr() { return &TopBottom; }
	glm::vec2* GetNearFarPtr() { return &NearFar; }
};