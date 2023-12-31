#include "Skybox.h"
Skybox::Skybox()
{
}

Skybox::~Skybox()
{
}

void Skybox::StartUp()
{
	std::vector<SkyboxVertexLayout> SkyBoxVertices =
	{
		{{1.0f, 1.0f, -1.0f}},
		{{1.0f, 1.0f, -1.0f}},
		{{1.0f, 1.0f, -1.0f}},
		{{1.0f, -1.0f, -1.0f}},
		{{1.0f, -1.0f, -1.0f}},
		{{1.0f, -1.0f, -1.0f}},
		{{1.0f, 1.0f, 1.0f}},
		{{1.0f, 1.0f, 1.0f}},
		{{1.0f, 1.0f, 1.0f}},
		{{1.0f, -1.0f, 1.0f}},
		{{1.0f, -1.0f, 1.0f}},
		{{1.0f, -1.0f, 1.0f}},
		{{-1.0f, 1.0f, -1.0f}},
		{{-1.0f, 1.0f, -1.0f}},
		{{-1.0f, 1.0f, -1.0f}},
		{{-1.0f, -1.0f, -1.0f}},
		{{-1.0f, -1.0f, -1.0f}},
		{{-1.0f, -1.0f, -1.0f}},
		{{-1.0f, 1.0f, 1.0f}},
		{{-1.0f, 1.0f, 1.0f}},
		{{-1.0f, 1.0f, 1.0f}},
		{{-1.0f, -1.0f, 1.0f}},
		{{-1.0f, -1.0f, 1.0f}},
		{{-1.0f, -1.0f, 1.0f}}
	};

	std::vector<uint32_t> IndexList
	{
		1,  14, 20, 1,  20,
		7,  10, 6,  19, 10,
		19, 23, 21, 18, 12,
		21, 12, 15, 16, 3,
		9,  16, 9,  22, 5,
		2,  8,  5,  8,  11,
		17, 13, 0,  17, 0,
		4
	};

	VertexBuffer.CreateBuffer(SkyBoxVertices.data(), SkyBoxVertices.size() * sizeof(SkyboxVertexLayout), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT |VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT |VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	skyBoxView = SkyBoxView{};
}

void Skybox::Update(std::shared_ptr<Camera> camera)
{
	skyBoxView.view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
	skyBoxView.proj = glm::perspective(glm::radians(camera->GetZoom()), VulkanRenderer::GetSwapChainResolution().width / (float)VulkanRenderer::GetSwapChainResolution().height, 0.1f, 100.0f);
	skyBoxView.proj[1][1] *= -1;
}

void Skybox::Draw(VkCommandBuffer& commandBuffer, VkPipelineLayout layout, VkDescriptorSet descriptorSet)
{
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &descriptorSet, 0, nullptr);
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &VertexBuffer.Buffer, offsets);
	vkCmdBindIndexBuffer(commandBuffer, IndexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
	vkCmdDrawIndexed(commandBuffer, 36, 1, 0, 0, 0);
}

void Skybox::Destroy()
{
	VertexBuffer.DestroyBuffer();
	IndexBuffer.DestroyBuffer();
}

