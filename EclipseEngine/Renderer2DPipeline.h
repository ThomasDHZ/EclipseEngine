#pragma once
#include "GraphicsPipeline.h"
#include "UniformBuffer.h"
#include "GameObject.h"

struct ConstMeshInfo
{
	alignas(4) uint32_t MeshIndex;
	alignas(16) glm::mat4 proj = glm::mat4(1.0f);
	alignas(16) glm::mat4 view = glm::mat4(1.0f);
	alignas(16) glm::vec3 CameraPos = glm::vec3(0.0f);
};

class Renderer2DPipeline : public GraphicsPipeline
{
private:
	UniformBuffer<MeshProperties2> meshproperties;
	void SetUpDescriptorBindings(GameObject obj);
	void SetUpShaderPipeLine(const VkRenderPass& renderPass);
public:
	Renderer2DPipeline();
	Renderer2DPipeline(const VkRenderPass& renderPass, GameObject obj);
	~Renderer2DPipeline();

	void UpdateGraphicsPipeLine(const VkRenderPass& renderPass, GameObject obj);
};

