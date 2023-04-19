#pragma once
#include "Light.h"
#include "UniformBuffer.h"
#include "RenderedCubeMapDepthTexture.h"

class GLTFSunLight : public Light<GLTFSunLightBuffer>
{
private:
	std::shared_ptr<RenderedCubeMapDepthTexture> LightViewTexture;
	//std::shared_ptr<GameObject> debugMesh;

public:
	GLTFSunLight();
	GLTFSunLight(const std::string name, glm::vec3 Position, glm::vec3 DiffuseColor, float Intesity);
	GLTFSunLight(GLTFSunLightBuffer light);
	~GLTFSunLight();

	void Update() override;
	void Destroy() override;

	void SetLightViewTexture(std::shared_ptr<RenderedCubeMapDepthTexture> lightViewTexture);

	std::shared_ptr<RenderedCubeMapDepthTexture> GetLightViewTexture() { return LightViewTexture; }

	glm::vec3 GetPosition() { return LightBuffer.UniformDataInfo.position; }
	glm::vec3* GetPositionPtr() { return &LightBuffer.UniformDataInfo.position; }
	glm::vec3* GetDiffusePtr() { return &LightBuffer.UniformDataInfo.diffuse; }
	float* GetIntensityPtr() { return &LightBuffer.UniformDataInfo.intensity; }
};
