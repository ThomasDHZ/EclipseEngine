#pragma once
#include "Light.h"
#include "UniformBuffer.h"
#include "RenderedCubeMapDepthTexture.h"
#include "RenderedDepthTexture.h"
#include <glm/gtc/matrix_transform.hpp>

class GLTFPointLight : public Light<GLTFPointLightBuffer>
{
private:
	std::shared_ptr<RenderedCubeMapDepthTexture> LightViewTexture;

	//std::shared_ptr<GameObject> debugMesh;

public:
	GLTFPointLight();
	GLTFPointLight(const std::string name, glm::vec3 Position, glm::vec3 DiffuseColor, float Intesity, float radius);
	GLTFPointLight(GLTFPointLightBuffer light);
	~GLTFPointLight();

	std::shared_ptr<RenderedDepthTexture> CubeMapSide[6];
	CubeMapSamplerBuffer cubeMapSampler;

	void Update() override;
	void Destroy() override;

	void SetLightViewTexture(std::shared_ptr<RenderedCubeMapDepthTexture> lightViewTexture);

	void GetCubeMapSamplerBuffer(std::vector<VkDescriptorBufferInfo>& CubeMapSamplerList)
	{
		VkDescriptorBufferInfo LightBufferInfo = {};
		LightBufferInfo.buffer = cubeMapSampler.GetVulkanBufferData().Buffer;
		LightBufferInfo.offset = 0;
		LightBufferInfo.range = VK_WHOLE_SIZE;
		CubeMapSamplerList.emplace_back(LightBufferInfo);
	}
	std::shared_ptr<RenderedCubeMapDepthTexture> GetLightViewTexture() { return LightViewTexture; }

	glm::vec3 GetPosition() { return LightBuffer.UniformDataInfo.position; }
	glm::vec3* GetPositionPtr() { return &LightBuffer.UniformDataInfo.position; }
	glm::vec3* GetDiffusePtr() { return &LightBuffer.UniformDataInfo.diffuse; }
	float GetRadius() { return LightBuffer.UniformDataInfo.radius; }
	float* GetIntensityPtr() { return &LightBuffer.UniformDataInfo.intensity; }
	float* GetRadiusPtr() { return &LightBuffer.UniformDataInfo.radius; }
};

