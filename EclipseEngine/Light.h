#pragma once
#include "JsonConverter.h"
#include "UniformBuffer.h"

template <class T>
class Light
{

protected:
	std::string LightName;
	UniformBuffer<T> LightBuffer;

public:

	Light()
	{

	}

	Light(T UniformData)
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
};

