#pragma once
#include "UniformBuffer.h"

class LightBase
{
private:
public:
	LightBase()
	{

	}

	~LightBase()
	{

	}
};

template <class T>
class Light : public LightBase
{

protected:
	std::string LightName;


public:
	glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
	glm::mat4 ViewMatrix = glm::mat4(1.0f);
	UniformBuffer<T> LightBuffer;
	Light() : LightBase()
	{

	}

	Light(T UniformData) : LightBase()
	{
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

	glm::mat4 GetProjectionMatrix() { return ProjectionMatrix; }
	glm::mat4 GetViewMatrix() { return ViewMatrix; }
};

