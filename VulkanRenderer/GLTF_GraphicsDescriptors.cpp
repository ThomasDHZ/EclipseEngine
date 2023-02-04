#include "GLTF_GraphicsDescriptors.h"
#include "GraphicsPipeline.h"

VkDescriptorPoolSize GLTF_GraphicsDescriptors::AddDescriptorPoolBinding(VkDescriptorType descriptorType, uint32_t descriptorCount)
{
	VkDescriptorPoolSize DescriptorPoolBinding = {};
	DescriptorPoolBinding.type = descriptorType;
	DescriptorPoolBinding.descriptorCount = descriptorCount;

	return DescriptorPoolBinding;
}

VkDescriptorSetLayout GLTF_GraphicsDescriptors::CreateDescriptorSetLayout(std::vector<DescriptorSetLayoutBindingInfo> LayoutBindingInfo)
{
	std::vector<VkDescriptorSetLayoutBinding> LayoutBindingList = {};

	for (auto Binding : LayoutBindingInfo)
	{
		VkDescriptorSetLayoutBinding LayoutBinding = {};
		LayoutBinding.binding = Binding.Binding;
		LayoutBinding.descriptorCount = Binding.Count;
		LayoutBinding.descriptorType = Binding.DescriptorType;
		LayoutBinding.pImmutableSamplers = nullptr;
		LayoutBinding.stageFlags = Binding.StageFlags;

		LayoutBindingList.emplace_back(LayoutBinding);
	}

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};

	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(LayoutBindingList.size());
	layoutInfo.pBindings = LayoutBindingList.data();

	VkDescriptorSetLayout descriptorSet;
	if (vkCreateDescriptorSetLayout(VulkanRenderer::GetDevice(), &layoutInfo, nullptr, &descriptorSet) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create descriptor set layout.");
	}

	return descriptorSet;
}

VkDescriptorSet GLTF_GraphicsDescriptors::CreateDescriptorSets(VkDescriptorPool descriptorPool, VkDescriptorSetLayout layout)
{
	{

		uint32_t variableDescCounts[] = { 1 };

		VkDescriptorSetVariableDescriptorCountAllocateInfoEXT VariableDescriptorCountAllocateInfo{};
		VariableDescriptorCountAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO_EXT;
		VariableDescriptorCountAllocateInfo.descriptorSetCount = 1;
		VariableDescriptorCountAllocateInfo.pDescriptorCounts = variableDescCounts;

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &layout;
		allocInfo.pNext = &VariableDescriptorCountAllocateInfo;

		VkDescriptorSet DescriptorSets;
		if (vkAllocateDescriptorSets(VulkanRenderer::GetDevice(), &allocInfo, &DescriptorSets) != VK_SUCCESS) {
			throw std::runtime_error("Failed to allocate descriptor sets.");
		}

		return DescriptorSets;
	}
}

VkDescriptorPool GLTF_GraphicsDescriptors::CreateDescriptorPool(std::vector<VkDescriptorPoolSize> DescriptorPoolInfo)
{
	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(DescriptorPoolInfo.size());
	poolInfo.pPoolSizes = DescriptorPoolInfo.data();
	poolInfo.maxSets = static_cast<uint32_t>(VulkanRenderer::GetSwapChainImageCount());

	VkDescriptorPool descriptorPool;
	if (vkCreateDescriptorPool(VulkanRenderer::GetDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create descriptor pool.");
	}

	return descriptorPool;
}

VkWriteDescriptorSet GLTF_GraphicsDescriptors::AddAccelerationBuffer(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, VkWriteDescriptorSetAccelerationStructureKHR& accelerationStructure)
{
	VkWriteDescriptorSet AccelerationDesciptorSet = {};
	AccelerationDesciptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	AccelerationDesciptorSet.descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
	AccelerationDesciptorSet.dstSet = DescriptorSet;
	AccelerationDesciptorSet.dstBinding = BindingNumber;
	AccelerationDesciptorSet.descriptorCount = 1;
	AccelerationDesciptorSet.pNext = &accelerationStructure;
	return AccelerationDesciptorSet;
}

VkWriteDescriptorSet GLTF_GraphicsDescriptors::AddTextureDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, VkDescriptorImageInfo& TextureImageInfo, VkDescriptorType descriptorType)
{
	VkWriteDescriptorSet TextureDescriptor = {};
	TextureDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	TextureDescriptor.dstSet = DescriptorSet;
	TextureDescriptor.dstBinding = BindingNumber;
	TextureDescriptor.dstArrayElement = 0;
	TextureDescriptor.descriptorType = descriptorType;
	TextureDescriptor.descriptorCount = 1;
	TextureDescriptor.pImageInfo = &TextureImageInfo;
	return TextureDescriptor;
}

VkWriteDescriptorSet GLTF_GraphicsDescriptors::AddTextureDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, std::vector<VkDescriptorImageInfo>& TextureImageInfo, VkDescriptorType descriptorType)
{
	VkWriteDescriptorSet TextureDescriptor = {};
	TextureDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	TextureDescriptor.dstSet = DescriptorSet;
	TextureDescriptor.dstBinding = BindingNumber;
	TextureDescriptor.dstArrayElement = 0;
	TextureDescriptor.descriptorType = descriptorType;
	TextureDescriptor.descriptorCount = static_cast<uint32_t>(TextureImageInfo.size());
	TextureDescriptor.pImageInfo = TextureImageInfo.data();
	return TextureDescriptor;
}

VkWriteDescriptorSet GLTF_GraphicsDescriptors::AddBufferDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, VkDescriptorBufferInfo& BufferInfo, VkDescriptorType descriptorType)
{
	VkWriteDescriptorSet BufferDescriptor = {};
	BufferDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	BufferDescriptor.dstSet = DescriptorSet;
	BufferDescriptor.dstBinding = BindingNumber;
	BufferDescriptor.dstArrayElement = 0;
	BufferDescriptor.descriptorType = descriptorType;
	BufferDescriptor.descriptorCount = 1;
	BufferDescriptor.pBufferInfo = &BufferInfo;
	return BufferDescriptor;
}

VkWriteDescriptorSet GLTF_GraphicsDescriptors::AddBufferDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo>& BufferInfoList, VkDescriptorType descriptorType)
{
	VkWriteDescriptorSet BufferDescriptor = {};
	BufferDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	BufferDescriptor.dstSet = DescriptorSet;
	BufferDescriptor.dstBinding = BindingNumber;
	BufferDescriptor.dstArrayElement = 0;
	BufferDescriptor.descriptorType = descriptorType;
	BufferDescriptor.descriptorCount = static_cast<uint32_t>(BufferInfoList.size());
	BufferDescriptor.pBufferInfo = BufferInfoList.data();
	return BufferDescriptor;
}

VkDescriptorSet GLTF_GraphicsDescriptors::SubmitDescriptorSet(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList)
{
	VkDescriptorPool DescriptorPool = VK_NULL_HANDLE;
	VkDescriptorSetLayout DescriptorSetLayout = VK_NULL_HANDLE;
	VkDescriptorSet DescriptorSet = VK_NULL_HANDLE;

	std::vector<VkDescriptorPoolSize>  DescriptorPoolList{};
	std::vector<DescriptorSetLayoutBindingInfo> LayoutBindingInfo{};
	std::vector<VkWriteDescriptorSet> DescriptorList{};

	if (DescriptorBindingList.size() > 0)
	{
		{
			std::vector<VkDescriptorPoolSize>  DescriptorPoolList = {};
			for (auto& DescriptorBinding : DescriptorBindingList)
			{
				DescriptorPoolList.emplace_back(AddDescriptorPoolBinding(DescriptorBinding.DescriptorType, DescriptorBinding.Count));
			}
			DescriptorPool = CreateDescriptorPool(DescriptorPoolList);
		}
		{
			std::vector<DescriptorSetLayoutBindingInfo> LayoutBindingInfo = {};
			for (auto& DescriptorBinding : DescriptorBindingList)
			{
				LayoutBindingInfo.emplace_back(DescriptorSetLayoutBindingInfo{ DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.DescriptorType, DescriptorBinding.StageFlags, DescriptorBinding.Count });
			}
			DescriptorSetLayout = CreateDescriptorSetLayout(LayoutBindingInfo);
		}
		{
			DescriptorSet = CreateDescriptorSets(DescriptorPool, DescriptorSetLayout);

			std::vector<VkWriteDescriptorSet> DescriptorList;

			for (auto& DescriptorBinding : DescriptorBindingList)
			{
				if (DescriptorBinding.BufferDescriptor.size() > 0)
				{
					DescriptorList.emplace_back(AddBufferDescriptorSet(DescriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.BufferDescriptor, DescriptorBinding.DescriptorType));
				}
				else if (DescriptorBinding.TextureDescriptor.size() > 0)
				{
					DescriptorList.emplace_back(AddTextureDescriptorSet(DescriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.TextureDescriptor, DescriptorBinding.DescriptorType));
				}
				else
				{
					DescriptorList.emplace_back(AddAccelerationBuffer(DescriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.AccelerationStructureDescriptor));
				}
			}
			vkUpdateDescriptorSets(VulkanRenderer::GetDevice(), static_cast<uint32_t>(DescriptorList.size()), DescriptorList.data(), 0, nullptr);
		}
	}
	else
	{
		DescriptorPoolList.emplace_back(AddDescriptorPoolBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1));
		DescriptorPool = CreateDescriptorPool(DescriptorPoolList);

		LayoutBindingInfo.emplace_back(DescriptorSetLayoutBindingInfo{ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL, 1 });
		DescriptorSetLayout = CreateDescriptorSetLayout(LayoutBindingInfo);

		DescriptorSet = CreateDescriptorSets(DescriptorPool, DescriptorSetLayout);
		vkUpdateDescriptorSets(VulkanRenderer::GetDevice(), static_cast<uint32_t>(DescriptorList.size()), DescriptorList.data(), 0, nullptr);
	}

	return DescriptorSet;
}

VkDescriptorSetLayout GLTF_GraphicsDescriptors::SubmitDescriptorSetLayout(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList)
{
	VkDescriptorPool DescriptorPool = VK_NULL_HANDLE;
	VkDescriptorSetLayout DescriptorSetLayout = VK_NULL_HANDLE;

	std::vector<VkDescriptorPoolSize>  DescriptorPoolList{};
	std::vector<DescriptorSetLayoutBindingInfo> LayoutBindingInfo{};

	if (DescriptorBindingList.size() > 0)
	{
		{
			std::vector<VkDescriptorPoolSize>  DescriptorPoolList = {};
			for (auto& DescriptorBinding : DescriptorBindingList)
			{
				DescriptorPoolList.emplace_back(AddDescriptorPoolBinding(DescriptorBinding.DescriptorType, DescriptorBinding.Count));
			}
			DescriptorPool = CreateDescriptorPool(DescriptorPoolList);
		}
		{
			std::vector<DescriptorSetLayoutBindingInfo> LayoutBindingInfo = {};
			for (auto& DescriptorBinding : DescriptorBindingList)
			{
				LayoutBindingInfo.emplace_back(DescriptorSetLayoutBindingInfo{ DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.DescriptorType, DescriptorBinding.StageFlags, DescriptorBinding.Count });
			}
			DescriptorSetLayout = CreateDescriptorSetLayout(LayoutBindingInfo);
		}
	}
	else
	{
		DescriptorPoolList.emplace_back(AddDescriptorPoolBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1));
		DescriptorPool = CreateDescriptorPool(DescriptorPoolList);

		LayoutBindingInfo.emplace_back(DescriptorSetLayoutBindingInfo{ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL, 1 });
		DescriptorSetLayout = CreateDescriptorSetLayout(LayoutBindingInfo);
	}

	return DescriptorSetLayout;
}

void GLTF_GraphicsDescriptors::AddAccelerationDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkWriteDescriptorSetAccelerationStructureKHR accelerationStructure, VkShaderStageFlags StageFlags)
{
	DescriptorSetBindingStruct DescriptorSetBinding{};
	DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
	DescriptorSetBinding.StageFlags = StageFlags;
	DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
	DescriptorSetBinding.AccelerationStructureDescriptor = accelerationStructure;
	DescriptorSetBinding.Count = 1;
	DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void GLTF_GraphicsDescriptors::AddStorageTextureSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorImageInfo TextureImageInfo, VkShaderStageFlags StageFlags)
{
	DescriptorSetBindingStruct DescriptorSetBinding{};
	DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
	DescriptorSetBinding.StageFlags = StageFlags;
	DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	DescriptorSetBinding.TextureDescriptor = std::vector<VkDescriptorImageInfo>{ TextureImageInfo };
	DescriptorSetBinding.Count = 1;
	DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorImageInfo> TextureImageInfo, VkShaderStageFlags StageFlags)
{
	DescriptorSetBindingStruct DescriptorSetBinding{};
	DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
	DescriptorSetBinding.StageFlags = StageFlags;
	DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorSetBinding.TextureDescriptor = TextureImageInfo;
	DescriptorSetBinding.Count = TextureImageInfo.size();
	DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorImageInfo TextureImageInfo, VkShaderStageFlags StageFlags)
{
	DescriptorSetBindingStruct DescriptorSetBinding{};
	DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
	DescriptorSetBinding.StageFlags = StageFlags;
	DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorSetBinding.TextureDescriptor = std::vector<VkDescriptorImageInfo>{ TextureImageInfo };
	DescriptorSetBinding.Count = 1;
	DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void GLTF_GraphicsDescriptors::AddUniformBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorBufferInfo BufferInfo, VkShaderStageFlags StageFlags)
{
	DescriptorSetBindingStruct DescriptorSetBinding{};
	DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
	DescriptorSetBinding.StageFlags = StageFlags;
	DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorSetBinding.BufferDescriptor = std::vector<VkDescriptorBufferInfo>{ BufferInfo };
	DescriptorSetBinding.Count = 1;
	DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void GLTF_GraphicsDescriptors::AddUniformBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo> BufferInfo, VkShaderStageFlags StageFlags)
{
	DescriptorSetBindingStruct DescriptorSetBinding{};
	DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
	DescriptorSetBinding.StageFlags = StageFlags;
	DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorSetBinding.BufferDescriptor = BufferInfo;
	DescriptorSetBinding.Count = BufferInfo.size();
	DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorBufferInfo BufferInfo, VkShaderStageFlags StageFlags)
{
	DescriptorSetBindingStruct DescriptorSetBinding{};
	DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
	DescriptorSetBinding.StageFlags = StageFlags;
	DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	DescriptorSetBinding.BufferDescriptor = std::vector<VkDescriptorBufferInfo>{ BufferInfo };
	DescriptorSetBinding.Count = 1;
	DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo> BufferInfo, VkShaderStageFlags StageFlags)
{
	DescriptorSetBindingStruct DescriptorSetBinding{};
	DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
	DescriptorSetBinding.StageFlags = StageFlags;
	DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	DescriptorSetBinding.BufferDescriptor = BufferInfo;
	DescriptorSetBinding.Count = BufferInfo.size();
	DescriptorBindingList.emplace_back(DescriptorSetBinding);
}
