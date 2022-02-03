#pragma once
#include "GraphicsPipeline.h"
#include "UniformBuffer.h"
#include "GameObjectManager.h"

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
	UniformBuffer<MeshProperties> meshproperties;
	void SetUpDescriptorBindings(std::shared_ptr<GameObject> obj, std::shared_ptr<GameObject> obj2);
	void SetUpShaderPipeLine(const VkRenderPass& renderPass);
public:
	Renderer2DPipeline();
	Renderer2DPipeline(const VkRenderPass& renderPass, std::shared_ptr<GameObject> obj, std::shared_ptr<GameObject> obj2);
	~Renderer2DPipeline();

	void UpdateGraphicsPipeLine(const VkRenderPass& renderPass, std::shared_ptr<GameObject> obj, std::shared_ptr<GameObject> obj2);
};

