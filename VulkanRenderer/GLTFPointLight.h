#pragma once
#include "Light.h"
#include "UniformBuffer.h"
#include "RenderedCubeMapDepthTexture.h"

class GLTFPointLight : public Light<GLTFPointLightBuffer>
{
private:
	std::shared_ptr<RenderedCubeMapDepthTexture> LightViewTexture;
	//std::shared_ptr<GameObject> debugMesh;

public:
	GLTFPointLight();
	GLTFPointLight(const std::string name, glm::vec3 Position, glm::vec3 DiffuseColor, float Intesity);
	GLTFPointLight(GLTFPointLightBuffer light);
	~GLTFPointLight();

	void Update() override;
	void Destroy() override;

	void SetLightViewTexture(std::shared_ptr<RenderedCubeMapDepthTexture> lightViewTexture);

	std::shared_ptr<RenderedCubeMapDepthTexture> GetLightViewTexture() { return LightViewTexture; }

	glm::vec3 GetPosition() { return LightBuffer.UniformDataInfo.position; }
	glm::vec3* GetPositionPtr() { return &LightBuffer.UniformDataInfo.position; }
	glm::vec3* GetDiffusePtr() { return &LightBuffer.UniformDataInfo.diffuse; }
	float* GetIntensityPtr() { return &LightBuffer.UniformDataInfo.intensity; }
};
