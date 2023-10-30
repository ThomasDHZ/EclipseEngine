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

VkDescriptorSetLayout GLTF_GraphicsDescriptors::CreateDescriptorSetLayou(std::vector<VkDescriptorSetLayoutBinding> LayoutBindingInfo)
{
	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(LayoutBindingInfo.size());
	layoutInfo.pBindings = LayoutBindingInfo.data();

	VkDescriptorSetLayout descriptorSet;
	if (vkCreateDescriptorSetLayout(VulkanRenderer::GetDevice(), &layoutInfo, nullptr, &descriptorSet) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create descriptor set layout.");
	}

	return descriptorSet;
}

VkDescriptorSet GLTF_GraphicsDescriptors::CreateDescriptorSets(VkDescriptorPool descriptorPool, VkDescriptorSetLayout layout)
{
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &layout;

	VkDescriptorSet DescriptorSets;
	if (vkAllocateDescriptorSets(VulkanRenderer::GetDevice(), &allocInfo, &DescriptorSets) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate descriptor sets.");
	}

	return DescriptorSets;
}

VkDescriptorSet GLTF_GraphicsDescriptors::CreateDescriptorSets(VkDescriptorPool descriptorPool, std::vector<VkDescriptorSetLayout> layout)
{
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(layout.size());
	allocInfo.pSetLayouts = layout.data();

	VkDescriptorSet DescriptorSets;
	if (vkAllocateDescriptorSets(VulkanRenderer::GetDevice(), &allocInfo, &DescriptorSets) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate descriptor sets.");
	}

	return DescriptorSets;
}

VkWriteDescriptorSetAccelerationStructureKHR GLTF_GraphicsDescriptors::AddAcclerationStructureBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, VkAccelerationStructureKHR* handle)
{
	VkWriteDescriptorSetAccelerationStructureKHR AccelerationDescriptorStructure = {};
	AccelerationDescriptorStructure.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR;
	AccelerationDescriptorStructure.accelerationStructureCount = 1;
	AccelerationDescriptorStructure.pAccelerationStructures = handle;
	return AccelerationDescriptorStructure;
}

VkDescriptorImageInfo GLTF_GraphicsDescriptors::AddRayTraceStorageImageDescriptor(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, VkImageLayout ImageLayout, VkImageView& ImageView)
{
	VkDescriptorImageInfo RayTraceImageDescriptor{};
	RayTraceImageDescriptor.imageView = ImageView;
	RayTraceImageDescriptor.imageLayout = ImageLayout;
	return RayTraceImageDescriptor;
}

VkDescriptorPool GLTF_GraphicsDescriptors::CreateDescriptorPool(std::vector<VkDescriptorPoolSize> DescriptorPoolInfo)
{
	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(DescriptorPoolInfo.size());
	poolInfo.pPoolSizes = DescriptorPoolInfo.data();
	poolInfo.maxSets = 50;

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

VkWriteDescriptorSet GLTF_GraphicsDescriptors::AddTextureDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, uint32_t dstArrayElement, VkDescriptorImageInfo& TextureImageInfo, VkDescriptorType descriptorType)
{
	VkWriteDescriptorSet TextureDescriptor = {};
	TextureDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	TextureDescriptor.dstSet = DescriptorSet;
	TextureDescriptor.dstBinding = BindingNumber;
	TextureDescriptor.dstArrayElement = dstArrayElement;
	TextureDescriptor.descriptorType = descriptorType;
	TextureDescriptor.descriptorCount = 1;
	TextureDescriptor.pImageInfo = &TextureImageInfo;
	return TextureDescriptor;
}

VkWriteDescriptorSet GLTF_GraphicsDescriptors::AddTextureDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, uint32_t dstArrayElement, std::vector<VkDescriptorImageInfo>& TextureImageInfo, VkDescriptorType descriptorType)
{
	VkWriteDescriptorSet TextureDescriptor = {};
	TextureDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	TextureDescriptor.dstSet = DescriptorSet;
	TextureDescriptor.dstBinding = BindingNumber;
	TextureDescriptor.dstArrayElement = dstArrayElement;
	TextureDescriptor.descriptorType = descriptorType;
	TextureDescriptor.descriptorCount = static_cast<uint32_t>(TextureImageInfo.size());
	TextureDescriptor.pImageInfo = TextureImageInfo.data();
	return TextureDescriptor;
}

VkWriteDescriptorSet GLTF_GraphicsDescriptors::AddBufferDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, uint32_t dstArrayElement, VkDescriptorBufferInfo& BufferInfo, VkDescriptorType descriptorType)
{
	VkWriteDescriptorSet BufferDescriptor = {};
	BufferDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	BufferDescriptor.dstSet = DescriptorSet;
	BufferDescriptor.dstBinding = BindingNumber;
	BufferDescriptor.dstArrayElement = dstArrayElement;
	BufferDescriptor.descriptorType = descriptorType;
	BufferDescriptor.descriptorCount = 1;
	BufferDescriptor.pBufferInfo = &BufferInfo;
	return BufferDescriptor;
}

VkWriteDescriptorSet GLTF_GraphicsDescriptors::AddBufferDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, uint32_t dstArrayElement, std::vector<VkDescriptorBufferInfo>& BufferInfoList, VkDescriptorType descriptorType)
{
	VkWriteDescriptorSet BufferDescriptor = {};
	BufferDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	BufferDescriptor.dstSet = DescriptorSet;
	BufferDescriptor.dstBinding = BindingNumber;
	BufferDescriptor.dstArrayElement = dstArrayElement;
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

VkWriteDescriptorSet GLTF_GraphicsDescriptors::WriteBufferDescriptorSet(VkDescriptorSet descriptorSet, uint32_t BindingNumber, VkDescriptorBufferInfo Buffer)
{
	std::vector<VkDescriptorBufferInfo> BufferInfo = std::vector<VkDescriptorBufferInfo>{ Buffer };

	VkWriteDescriptorSet writeTextureDescriptorSet = {};
	writeTextureDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeTextureDescriptorSet.dstSet = descriptorSet;
	writeTextureDescriptorSet.dstBinding = BindingNumber;
	writeTextureDescriptorSet.dstArrayElement = 0;
	writeTextureDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writeTextureDescriptorSet.descriptorCount = static_cast<uint32_t>(BufferInfo.size());
	writeTextureDescriptorSet.pBufferInfo = BufferInfo.data();
	return writeTextureDescriptorSet;
}

VkWriteDescriptorSet GLTF_GraphicsDescriptors::WriteBufferDescriptorSet(VkDescriptorSet descriptorSet, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo> Buffer)
{
	VkWriteDescriptorSet writeTextureDescriptorSet = {};
	writeTextureDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeTextureDescriptorSet.dstSet = descriptorSet;
	writeTextureDescriptorSet.dstBinding = BindingNumber;
	writeTextureDescriptorSet.dstArrayElement = 0;
	writeTextureDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writeTextureDescriptorSet.descriptorCount = static_cast<uint32_t>(Buffer.size());
	writeTextureDescriptorSet.pBufferInfo = Buffer.data();
	return writeTextureDescriptorSet;
}

VkWriteDescriptorSet GLTF_GraphicsDescriptors::WriteTextureDescriptorSet(VkDescriptorSet descriptorSet, uint32_t BindingNumber, VkDescriptorImageInfo TextureBuffer)
{
	std::vector<VkDescriptorImageInfo> TextureBufferInfo = std::vector<VkDescriptorImageInfo>{ TextureBuffer };

	VkWriteDescriptorSet writeTextureDescriptorSet = {};
	writeTextureDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeTextureDescriptorSet.dstSet = descriptorSet;
	writeTextureDescriptorSet.dstBinding = BindingNumber;
	writeTextureDescriptorSet.dstArrayElement = 0;
	writeTextureDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	writeTextureDescriptorSet.descriptorCount = static_cast<uint32_t>(TextureBufferInfo.size());
	writeTextureDescriptorSet.pImageInfo = TextureBufferInfo.data();
	return writeTextureDescriptorSet;
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

VkDescriptorPool GLTF_GraphicsDescriptors::CreateDesciptorPool(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList)
{
	VkDescriptorPool DescriptorPool = VK_NULL_HANDLE;

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
	}
	return DescriptorPool;
}

VkDescriptorSetLayout GLTF_GraphicsDescriptors::CreateDescriptorSetLayout(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList)
{
	VkDescriptorSetLayout DescriptorSetLayout = VK_NULL_HANDLE;
	std::vector<DescriptorSetLayoutBindingInfo> LayoutBindingInfo = {};
	for (auto& DescriptorBinding : DescriptorBindingList)
	{
		LayoutBindingInfo.emplace_back(DescriptorSetLayoutBindingInfo{ DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.DescriptorType, DescriptorBinding.StageFlags, DescriptorBinding.Count });
	}
	DescriptorSetLayout = CreateDescriptorSetLayout(LayoutBindingInfo);
	return DescriptorSetLayout;
}

VkDescriptorSetLayout GLTF_GraphicsDescriptors::CreateDescriptorSetLayout(std::vector<VkDescriptorSetLayoutBinding>& DescriptorBindingList)
{
	VkDescriptorSetLayout DescriptorSetLayout = VK_NULL_HANDLE;
	DescriptorSetLayout = CreateDescriptorSetLayou(DescriptorBindingList);
	return DescriptorSetLayout;
}

VkDescriptorSet GLTF_GraphicsDescriptors::CreateDescriptorSet(VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, std::vector<DescriptorSetBindingStruct>& DescriptorBindingList)
{
	std::vector<VkWriteDescriptorSet> DescriptorList;
	VkDescriptorSet DescriptorSet2 = CreateDescriptorSets(descriptorPool, descriptorSetLayout);
	for (auto& DescriptorBinding : DescriptorBindingList)
	{
		if (DescriptorBinding.BufferDescriptor.size() > 0)
		{
			DescriptorList.emplace_back(AddBufferDescriptorSet(DescriptorSet2, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.BufferDescriptor, DescriptorBinding.DescriptorType));
		}
		else if (DescriptorBinding.TextureDescriptor.size() > 0)
		{
			DescriptorList.emplace_back(AddTextureDescriptorSet(DescriptorSet2, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.TextureDescriptor, DescriptorBinding.DescriptorType));
		}
		else
		{
			DescriptorList.emplace_back(AddAccelerationBuffer(DescriptorSet2, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.AccelerationStructureDescriptor));
		}
	}
	vkUpdateDescriptorSets(VulkanRenderer::GetDevice(), static_cast<uint32_t>(DescriptorList.size()), DescriptorList.data(), 0, nullptr);
	return DescriptorSet2;
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
