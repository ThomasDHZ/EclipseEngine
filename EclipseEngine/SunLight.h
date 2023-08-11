#pragma once
#include "Light.h"
#include "UniformBuffer.h"
#include "RenderedCubeMapDepthTexture.h"
#include "RenderedDepthTexture.h"

class SunLight : public Light<GLTFSunLightBuffer>
{
private:
	std::shared_ptr<RenderedCubeMapDepthTexture> LightViewTexture;
	//std::shared_ptr<GameObject> debugMesh;

public:
	SunLight();
	SunLight(const std::string name, glm::vec3 Position, glm::vec3 DiffuseColor, float Intesity);
	SunLight(GLTFSunLightBuffer light);
	~SunLight();

	void Update() override;
	void Destroy() override;

	void SetLightViewTexture(std::shared_ptr<RenderedCubeMapDepthTexture> lightViewTexture);

	std::shared_ptr<RenderedDepthTexture> CubeMapSide[6];
	std::shared_ptr<RenderedCubeMapDepthTexture> GetLightViewTexture() { return LightViewTexture; }

	glm::vec3 GetPosition() { return LightBuffer.UniformDataInfo.position; }
	glm::vec3* GetPositionPtr() { return &LightBuffer.UniformDataInfo.position; }
	glm::vec3* GetDiffusePtr() { return &LightBuffer.UniformDataInfo.diffuse; }
	float* GetIntensityPtr() { return &LightBuffer.UniformDataInfo.intensity; }
};
