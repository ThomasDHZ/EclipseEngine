#pragma once
#include "Light.h"
#include "UniformBuffer.h"

class DirectionalLight : public Light<DirectionalLightBuffer>
{
public:
	DirectionalLight();
	DirectionalLight(DirectionalLightBuffer light);
	~DirectionalLight();

	void Update();
	void Destroy() override;

	glm::vec3* GetPositionPtr() { return &LightBuffer.UniformDataInfo.position; }
	glm::vec3* GetDirectionPtr() { return &LightBuffer.UniformDataInfo.direction; }
	glm::vec3* GetDiffusePtr() { return &LightBuffer.UniformDataInfo.diffuse; }
	glm::vec3* GetSpecularPtr() { return &LightBuffer.UniformDataInfo.specular; }

	//alignas(16) glm::vec3 position = glm::vec3(0.0f);
	//alignas(16) glm::vec3 direction = glm::vec3(0.00001f);
	//alignas(16) glm::vec3 diffuse = glm::vec3(1.0f);
	//alignas(16) glm::vec3 specular = glm::vec3(1.0f);
	//alignas(16) glm::mat4 LightSpaceMatrix = glm::mat4(1.0f);

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

