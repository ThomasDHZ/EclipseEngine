#include "ComputeAnimationPipeline.h"
#include "Vertex.h"
#include "GraphicsPipeline.h"

ComputeAnimationPipeline::ComputeAnimationPipeline()
{
}

ComputeAnimationPipeline::~ComputeAnimationPipeline()
{
}

void ComputeAnimationPipeline::StartUp()
{
	for (auto& mesh : MeshRendererManager::GetMeshList())
	{
		auto mesh3D = static_cast<Mesh3D*>(mesh.get());
		if (mesh3D->GetBoneCount() > 0)
		{
			meshPtr.emplace_back(mesh3D);
		}
	}

	SetUpDescriptorBindings();
	CreateShaderPipeLine();

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = VulkanRenderer::GetCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;

	if (vkAllocateCommandBuffers(VulkanRenderer::GetDevice(), &allocInfo, &commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void ComputeAnimationPipeline::SetUpDescriptorBindings()
{
	std::vector<VkDescriptorBufferInfo> VertexBufferInfo;
	std::vector<VkDescriptorBufferInfo> BoneWeightBufferInfo;
	std::vector<VkDescriptorBufferInfo> MeshDataBufferInfo;
	std::vector<VkDescriptorBufferInfo> BoneTransformBufferInfo;

	for (auto& mesh : meshPtr)
	{
		VertexBufferCopy.emplace_back(mesh->GetVertexVulkanBufferPtr());
		VertexBufferInfo.emplace_back(AddBufferDescriptor(mesh->VertexBuffer));
		BoneWeightBufferInfo.emplace_back(AddBufferDescriptor(mesh->BoneWeightBuffer));
		MeshDataBufferInfo.emplace_back(AddBufferDescriptor(mesh->MeshPropertiesBuffer.VulkanBufferData));
		BoneTransformBufferInfo.emplace_back(AddBufferDescriptor(mesh->BoneTransformBuffer));
	}

	std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
	AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, VertexBufferInfo);
	AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 1, BoneWeightBufferInfo);
	AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 2, MeshDataBufferInfo);
	AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 3, BoneTransformBufferInfo);
	SubmitDescriptorSet(DescriptorBindingList);
}

void ComputeAnimationPipeline::CreateShaderPipeLine()
{
	auto ComputeShaderCode = CreateShader(BaseShaderFilePath + "animate.spv", VK_SHADER_STAGE_COMPUTE_BIT);

	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(ConstMeshInfo);

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pSetLayouts = &DescriptorSetLayout;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
	vkCreatePipelineLayout(VulkanRenderer::GetDevice(), &pipelineLayoutInfo, nullptr, &ShaderPipelineLayout);

	VkPipelineCacheCreateInfo pipelineCacheInfo{};
	pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	vkCreatePipelineCache(VulkanRenderer::GetDevice(), &pipelineCacheInfo, nullptr, &PipelineCache);

	VkComputePipelineCreateInfo ComputePipelineInfo{};
	ComputePipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	ComputePipelineInfo.layout = ShaderPipelineLayout;
	ComputePipelineInfo.flags = 0;
	ComputePipelineInfo.stage = ComputeShaderCode;

	if (vkCreateComputePipelines(VulkanRenderer::GetDevice(), PipelineCache, 1, &ComputePipelineInfo, nullptr, &ShaderPipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create compute pipeline!");
	}

	vkDestroyShaderModule(VulkanRenderer::GetDevice(), ComputeShaderCode.module, nullptr);
}

void ComputeAnimationPipeline::Compute()
{
	VkCommandBufferBeginInfo CommandBufferBeginInfo{};
	CommandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	CommandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	vkBeginCommandBuffer(commandBuffer, &CommandBufferBeginInfo);

	for (int x = 0; x < meshPtr.size(); x++)
	{
		ConstMeshInfo meshInfo;
		meshInfo.MeshIndex = x;

		VkBufferMemoryBarrier BufferMemoryBarrier{};
		BufferMemoryBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
		BufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		BufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		BufferMemoryBarrier.buffer = VertexBufferCopy[x]->Buffer;
		BufferMemoryBarrier.size = VK_WHOLE_SIZE;
		BufferMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
		BufferMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		BufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		BufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, 0, 0, nullptr, 1, &BufferMemoryBarrier, 0, nullptr);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, ShaderPipeline);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, ShaderPipelineLayout, 0, 1, &DescriptorSet, 0, 0);
		vkCmdPushConstants(commandBuffer, ShaderPipelineLayout, VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(ConstMeshInfo), &meshInfo);
		vkCmdDispatch(commandBuffer, VertexBufferCopy[x]->GetBufferSize(), 1, 1);

		BufferMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
		BufferMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		BufferMemoryBarrier.buffer = VertexBufferCopy[x]->Buffer;
		BufferMemoryBarrier.size = VK_WHOLE_SIZE;
		BufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		BufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 1, &BufferMemoryBarrier, 0, nullptr);

		meshPtr[x]->GetVertexVulkanBufferPtr()->CopyBufferToMemory(&meshPtr[x]->GetVertexList()[0], sizeof(Vertex3D) * meshPtr[x]->GetVertexList().size());
	}
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	VkFenceCreateInfo fenceCreateInfo{};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = 0;

	VkFence fence;
	vkCreateFence(VulkanRenderer::GetDevice(), &fenceCreateInfo, nullptr, &fence);
	vkQueueSubmit(VulkanRenderer::GetGraphicsQueue(), 1, &submitInfo, fence);
	vkWaitForFences(VulkanRenderer::GetDevice(), 1, &fence, VK_TRUE, UINT64_MAX);
	vkDestroyFence(VulkanRenderer::GetDevice(), fence, nullptr);
}