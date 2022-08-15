#pragma once
#include "Light.h"
#include "UniformBuffer.h"

class DirectionalLight : public Light<DirectionalLightBuffer>
{
private:

	void from_json(nlohmann::json& json)
	{
		JsonConverter::from_json(json["LightName"], LightName);
		JsonConverter::from_json(json["position"], LightBuffer.UniformDataInfo.position);
		JsonConverter::from_json(json["direction"], LightBuffer.UniformDataInfo.direction);
		JsonConverter::from_json(json["diffuse"], LightBuffer.UniformDataInfo.diffuse);
		JsonConverter::from_json(json["specular"], LightBuffer.UniformDataInfo.specular);
		JsonConverter::from_json(json["LightSpaceMatrix"], LightBuffer.UniformDataInfo.LightSpaceMatrix);
	}

public:
	DirectionalLight();
	DirectionalLight(DirectionalLightBuffer light);
	DirectionalLight(nlohmann::json& json);
	~DirectionalLight();

	//glm::vec2 LeftRight = glm::vec2(-25.0f, 25.0f);
	//glm::vec2 TopBottom = glm::vec2(-25.0f, 25.0f);
	//glm::vec2 NearFar = glm::vec2(0.1f, 25.0f);
	glm::vec2 LeftRight = glm::vec2(-180.0f, 192.0f);
	glm::vec2 TopBottom = glm::vec2(-110.0f, 117.0f);
	glm::vec2 NearFar = glm::vec2(-5000.0f, 460.0f);


	void Update();
	void Destroy() override;

	glm::vec3* GetPositionPtr() { return &LightBuffer.UniformDataInfo.position; }
	glm::vec3* GetDirectionPtr() { return &LightBuffer.UniformDataInfo.direction; }
	glm::vec3* GetDiffusePtr() { return &LightBuffer.UniformDataInfo.diffuse; }
	glm::vec3* GetSpecularPtr() { return &LightBuffer.UniformDataInfo.specular; }
	glm::vec2* GetLeftRightPtr() { return &LeftRight; }
	glm::vec2* GetTopBottomPtr() { return &TopBottom; }
	glm::vec2* GetNearFarPtr() { return &NearFar; }

	void to_json(nlohmann::json& json)
	{
		JsonConverter::to_json(json["LightName"], LightName);
		JsonConverter::to_json(json["position"], LightBuffer.UniformDataInfo.position);
		JsonConverter::to_json(json["direction"], LightBuffer.UniformDataInfo.direction);
		JsonConverter::to_json(json["diffuse"], LightBuffer.UniformDataInfo.diffuse);
		JsonConverter::to_json(json["specular"], LightBuffer.UniformDataInfo.specular);
		JsonConverter::to_json(json["LightSpaceMatrix"], LightBuffer.UniformDataInfo.LightSpaceMatrix);
	}
};

