#pragma once
#include "Light.h"
#include "UniformBuffer.h"
#include "RenderedCubeMapDepthTexture.h"

class PointLight : public Light<PointLightBuffer>
{
private:
	std::shared_ptr<RenderedCubeMapDepthTexture> LightViewTexture;
	void from_json(nlohmann::json& json)
	{
		JsonConverter::from_json(json["LightName"], LightName);
		JsonConverter::from_json(json["position"], LightBuffer.UniformDataInfo.position);
		JsonConverter::from_json(json["diffuse"], LightBuffer.UniformDataInfo.diffuse);
		JsonConverter::from_json(json["specular"], LightBuffer.UniformDataInfo.specular);
		JsonConverter::from_json(json["constant"], LightBuffer.UniformDataInfo.constant);
		JsonConverter::from_json(json["linear"], LightBuffer.UniformDataInfo.linear);
		JsonConverter::from_json(json["quadratic"], LightBuffer.UniformDataInfo.quadratic);
		JsonConverter::from_json(json["LightSpaceMatrix"], LightBuffer.UniformDataInfo.LightSpaceMatrix);
	}

public:
	PointLight();
	PointLight(PointLightBuffer light);
	PointLight(nlohmann::json& json);
	~PointLight();

	void Update() override;
	void Destroy() override;

	void SetLightViewTexture(std::shared_ptr<RenderedCubeMapDepthTexture> lightViewTexture);

	std::shared_ptr<RenderedCubeMapDepthTexture> GetLightViewTexture() { return LightViewTexture; }

	glm::vec3 GetPosition() { return LightBuffer.UniformDataInfo.position; }
	glm::vec3* GetPositionPtr() { return &LightBuffer.UniformDataInfo.position; }
	glm::vec3* GetDiffusePtr() { return &LightBuffer.UniformDataInfo.diffuse; }
	glm::vec3* GetSpecularPtr() { return &LightBuffer.UniformDataInfo.specular; }

	void to_json(nlohmann::json& json)
	{
		JsonConverter::to_json(json["LightName"], LightName);
		JsonConverter::to_json(json["position"], LightBuffer.UniformDataInfo.position);
		JsonConverter::to_json(json["diffuse"], LightBuffer.UniformDataInfo.diffuse);
		JsonConverter::to_json(json["specular"], LightBuffer.UniformDataInfo.specular);
		JsonConverter::to_json(json["constant"], LightBuffer.UniformDataInfo.constant);
		JsonConverter::to_json(json["linear"], LightBuffer.UniformDataInfo.linear);
		JsonConverter::to_json(json["quadratic"], LightBuffer.UniformDataInfo.quadratic);
		JsonConverter::to_json(json["LightSpaceMatrix"], LightBuffer.UniformDataInfo.LightSpaceMatrix);
	}
};
