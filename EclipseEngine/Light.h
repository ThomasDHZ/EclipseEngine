#pragma once
#include "UniformBuffer.h"

enum LightTypeEnum
{
	kSunLight,
	kDirectionalLight,
	kPointLight,
	kSpotLight
};

class LightBase
{
private:
protected:
	std::string LightName;
	LightTypeEnum LightType;

	glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
	glm::mat4 ViewMatrix = glm::mat4(1.0f);

	bool StaticLight = true;
	bool SelectedLight = false;
public:
	LightBase()
	{

	}

	~LightBase()
	{

	}

	void SetSelectedLight(bool setting)
	{
		SelectedLight = setting;
	}

	std::string GetLightName() { return LightName; }
	glm::mat4 GetProjectionMatrix() { return ProjectionMatrix; }
	glm::mat4 GetViewMatrix() { return ViewMatrix; }
	bool GetStaticLightStatus() { return StaticLight; }
	bool GetSelectedLightStatus() { return SelectedLight; }
	bool* GetSelectedLightStatusPtr() { return &SelectedLight; }
	bool* GetStaticLightStatusPtr() { return &StaticLight; }
};

template <class T>
class Light : public LightBase
{

protected:


public:
	UniformBuffer<T> LightBuffer;
	Light() : LightBase()
	{

	}

	Light(LightTypeEnum lightType, T UniformData) : LightBase()
	{
		LightType = lightType;
	}

	~Light()
	{

	}

	virtual void Update()
	{
		LightBuffer.Update();
	}

	virtual void Destroy()
	{
		LightBuffer.Destroy();
	}

	void  GetLightPropertiesBuffer(std::vector<VkDescriptorBufferInfo>& LightPropertiesBufferList)
	{
		VkDescriptorBufferInfo LightBufferInfo = {};
		LightBufferInfo.buffer = LightBuffer.GetVulkanBufferData().Buffer;
		LightBufferInfo.offset = 0;
		LightBufferInfo.range = VK_WHOLE_SIZE;
		LightPropertiesBufferList.emplace_back(LightBufferInfo);
	}
};

