#pragma once
#include "Light.h"
#include "UniformBuffer.h"
#include "RenderedCubeMapDepthTexture.h"

class PointLight : public Light<PointLightBuffer>
{
private:
	std::shared_ptr<RenderedCubeMapDepthTexture> LightViewTexture;

public:
	PointLight();
	PointLight(PointLightBuffer light);
	~PointLight();

	void Update() override;
	void Destroy() override;

	void SetLightViewTexture(std::shared_ptr<RenderedCubeMapDepthTexture> lightViewTexture);


	std::shared_ptr<RenderedCubeMapDepthTexture> GetLightViewTexture() { return LightViewTexture; }

	glm::vec3 GetPosition() { return LightBuffer.UniformDataInfo.position; }
	glm::vec3* GetPositionPtr() { return &LightBuffer.UniformDataInfo.position; }
	glm::vec3* GetDiffusePtr() { return &LightBuffer.UniformDataInfo.diffuse; }
	glm::vec3* GetSpecularPtr() { return &LightBuffer.UniformDataInfo.specular; }
};
