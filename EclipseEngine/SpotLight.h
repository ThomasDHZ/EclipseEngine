#pragma once
#include "Light.h"
#include "UniformBuffer.h"

class SpotLight : public Light<SpotLightBuffer>
{
private:
	void from_json(nlohmann::json& json)
	{
		JsonConverter::from_json(json["LightName"], LightName);
		JsonConverter::from_json(json["position"], LightBuffer.UniformDataInfo.position);
		JsonConverter::from_json(json["diffuse"], LightBuffer.UniformDataInfo.diffuse);
		JsonConverter::from_json(json["specular"], LightBuffer.UniformDataInfo.specular);
		JsonConverter::from_json(json["cutOff"], LightBuffer.UniformDataInfo.cutOff);
		JsonConverter::from_json(json["outerCutOff"], LightBuffer.UniformDataInfo.outerCutOff);
		JsonConverter::from_json(json["constant"], LightBuffer.UniformDataInfo.constant);
		JsonConverter::from_json(json["linear"], LightBuffer.UniformDataInfo.linear);
		JsonConverter::from_json(json["quadratic"], LightBuffer.UniformDataInfo.quadratic);
		JsonConverter::from_json(json["LightSpaceMatrix"], LightBuffer.UniformDataInfo.LightSpaceMatrix);
	}

public:
	SpotLight();
	SpotLight(SpotLightBuffer light);
	SpotLight(nlohmann::json& json);
	~SpotLight();

	void Update() override;
	void Destroy() override;

	glm::vec3* GetPositionPtr() { return &LightBuffer.UniformDataInfo.position; }
	glm::vec3* GetDirectionPtr() { return &LightBuffer.UniformDataInfo.direction; }
	glm::vec3* GetDiffusePtr() { return &LightBuffer.UniformDataInfo.diffuse; }
	glm::vec3* GetSpecularPtr() { return &LightBuffer.UniformDataInfo.specular; }

	void to_json(nlohmann::json& json)
	{
		JsonConverter::to_json(json["LightName"], LightName);
		JsonConverter::to_json(json["position"], LightBuffer.UniformDataInfo.position);
		JsonConverter::to_json(json["diffuse"], LightBuffer.UniformDataInfo.diffuse);
		JsonConverter::to_json(json["specular"], LightBuffer.UniformDataInfo.specular);
		JsonConverter::to_json(json["cutOff"], LightBuffer.UniformDataInfo.cutOff);
		JsonConverter::to_json(json["outerCutOff"], LightBuffer.UniformDataInfo.outerCutOff);
		JsonConverter::to_json(json["constant"], LightBuffer.UniformDataInfo.constant);
		JsonConverter::to_json(json["linear"], LightBuffer.UniformDataInfo.linear);
		JsonConverter::to_json(json["quadratic"], LightBuffer.UniformDataInfo.quadratic);
		JsonConverter::to_json(json["LightSpaceMatrix"], LightBuffer.UniformDataInfo.LightSpaceMatrix);
	}
};