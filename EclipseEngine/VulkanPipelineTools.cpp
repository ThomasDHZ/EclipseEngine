#include "VulkanPipelineTools.h"


VkShaderModule VulkanPipelineTools::ReadShaderFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = buffer.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(VulkanRenderer::GetDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module.");
    }

    return shaderModule;
}

VkPipelineShaderStageCreateInfo VulkanPipelineTools::CreateShader(const std::string& filename, VkShaderStageFlagBits shaderStages)
{
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = shaderStages;
    vertShaderStageInfo.module = ReadShaderFile(filename);
    vertShaderStageInfo.pName = "main";

    return vertShaderStageInfo;
}

VkDescriptorSet VulkanPipelineTools::CreateDescriptorSets(VkDescriptorPool descriptorPool, VkDescriptorSetLayout layout)
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

void VulkanPipelineTools::LoadDescriptorSets(nlohmann::json& json)
{
    std::vector<DescriptorBindingPropertiesEnum> BindingList;
    std::vector<VkDescriptorType> DescriptorList;
    for (int x = 0; x < json.size(); x++)
    {
        BindingList.emplace_back(json[x]["bindingType"]);
        DescriptorList.emplace_back(json[x]["descriptorType"]);
    }

    std::vector<VkDescriptorPoolSize> descriptorPoolSizeList;
    for (int x = 0; x < BindingList.size(); x++)
    {
        switch (BindingList[x])
        {
            case kMeshPropertiesDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], (uint32_t)GLTFSceneManager::GetGameObjectPropertiesBuffer().size() }); break;
            case kModelTransformDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x],  (uint32_t)GLTFSceneManager::GetGameObjectTransformBuffer().size() }); break;
            case kMaterialDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], (uint32_t)GLTFSceneManager::GetMaterialPropertiesBuffer().size() }); break;
            case kTextureDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], (uint32_t)GLTFSceneManager::GetTexturePropertiesBuffer().size() }); break;
            case kBRDFMapDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], 1 }); break;
            case kIrradianceMapDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], 1 }); break;
            case kPrefilterMapDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], 1 }); break;
            case kCubeMapDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], 1 }); break;
            case kEnvironmentDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], 1 }); break;
            case kSunLightDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], (uint32_t)GLTFSceneManager::GetSunLightPropertiesBuffer().size() }); break;
            case kDirectionalLightDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], (uint32_t)GLTFSceneManager::GetDirectionalLightPropertiesBuffer().size() }); break;
            case kPointLightDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], (uint32_t)GLTFSceneManager::GetPointLightPropertiesBuffer().size() }); break;
            case kSpotLightDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], (uint32_t)GLTFSceneManager::GetSpotLightPropertiesBuffer().size() }); break;
            case kReflectionViewDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], 1 }); break;
            case kReflectionIrradianceMapDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], (uint32_t)GLTFSceneManager::GetReflectionIrradianceMapDescriptor().size() }); break;
            case kReflectionPrefilterMapDescriptor: descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ DescriptorList[x], (uint32_t)GLTFSceneManager::GetReflectionPrefilterMapDescriptor().size() }); break;
        }
    }
    DescriptorPool = GLTF_GraphicsDescriptors::CreateDescriptorPool(descriptorPoolSizeList);

    std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBinding;
    for (int x = 0; x < BindingList.size(); x++)
    {
        switch (BindingList[x])
        {
            case kMeshPropertiesDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x, DescriptorList[x], (uint32_t)GLTFSceneManager::GetGameObjectPropertiesBuffer().size(), VK_SHADER_STAGE_ALL }); break;
            case kModelTransformDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x],  (uint32_t)GLTFSceneManager::GetGameObjectTransformBuffer().size(), VK_SHADER_STAGE_ALL }); break;
            case kMaterialDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], (uint32_t)GLTFSceneManager::GetMaterialPropertiesBuffer().size(), VK_SHADER_STAGE_ALL }); break;
            case kTextureDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], (uint32_t)GLTFSceneManager::GetTexturePropertiesBuffer().size(), VK_SHADER_STAGE_ALL }); break;
            case kBRDFMapDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], 1, VK_SHADER_STAGE_ALL }); break;
            case kIrradianceMapDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], 1, VK_SHADER_STAGE_ALL }); break;
            case kPrefilterMapDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], 1, VK_SHADER_STAGE_ALL }); break;
            case kCubeMapDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], 1, VK_SHADER_STAGE_ALL }); break;
            case kEnvironmentDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], 1, VK_SHADER_STAGE_ALL }); break;
            case kSunLightDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], (uint32_t)GLTFSceneManager::GetSunLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL }); break;
            case kDirectionalLightDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], (uint32_t)GLTFSceneManager::GetDirectionalLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL }); break;
            case kPointLightDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], (uint32_t)GLTFSceneManager::GetPointLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL }); break;
            case kSpotLightDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], (uint32_t)GLTFSceneManager::GetSpotLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL }); break;
            case kReflectionViewDescriptor:  descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], 1, VK_SHADER_STAGE_ALL }); break;
            case kReflectionIrradianceMapDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], (uint32_t)GLTFSceneManager::GetReflectionIrradianceMapDescriptor().size(), VK_SHADER_STAGE_ALL }); break;
            case kReflectionPrefilterMapDescriptor: descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ (uint32_t)x,  DescriptorList[x], (uint32_t)GLTFSceneManager::GetReflectionPrefilterMapDescriptor().size(), VK_SHADER_STAGE_ALL }); break;
        }
    }
    DescriptorSetLayout = GLTF_GraphicsDescriptors::CreateDescriptorSetLayout(descriptorSetLayoutBinding);

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    for (int x = 0; x < BindingList.size(); x++)
    {
        switch (BindingList[x])
        {
            case kMeshPropertiesDescriptor: AddStorageBufferDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetGameObjectPropertiesBuffer()); break;
            case kModelTransformDescriptor: AddStorageBufferDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetGameObjectTransformBuffer()); break;
            case kMaterialDescriptor: AddStorageBufferDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetMaterialPropertiesBuffer()); break;
            case kTextureDescriptor: AddTextureDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetTexturePropertiesBuffer()); break;
            case kBRDFMapDescriptor: AddTextureDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetBRDFMapDescriptor()); break;
            case kIrradianceMapDescriptor: AddTextureDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetIrradianceMapDescriptor()); break;
            case kPrefilterMapDescriptor: AddTextureDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetPrefilterMapDescriptor()); break;
            case kCubeMapDescriptor: AddTextureDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetCubeMapDescriptor()); break;
            case kEnvironmentDescriptor: AddTextureDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetEnvironmentMapDescriptor()); break;
            case kSunLightDescriptor: AddStorageBufferDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetSunLightPropertiesBuffer()); break;
            case kDirectionalLightDescriptor: AddStorageBufferDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetDirectionalLightPropertiesBuffer()); break;
            case kPointLightDescriptor: AddStorageBufferDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetPointLightPropertiesBuffer()); break;
            case kSpotLightDescriptor: AddStorageBufferDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetSpotLightPropertiesBuffer()); break;
            case kReflectionViewDescriptor: AddUniformBufferDescriptorSetBinding(DescriptorBindingList, x, GetReflectionMapBuffer()); break;
            case kReflectionIrradianceMapDescriptor: AddTextureDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetReflectionIrradianceMapDescriptor()); break;
            case kReflectionPrefilterMapDescriptor: AddTextureDescriptorSetBinding(DescriptorBindingList, x, GLTFSceneManager::GetReflectionPrefilterMapDescriptor()); break;
        }
    }
    DescriptorSet = GLTF_GraphicsDescriptors::CreateDescriptorSets(DescriptorPool, DescriptorSetLayout);

    std::vector<VkWriteDescriptorSet> writeDescriptorSet;
    for (auto& DescriptorBinding : DescriptorBindingList)
    {
        switch (DescriptorBinding.DescriptorType)
        {
        case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER: writeDescriptorSet.emplace_back(AddBufferDescriptorSet(DescriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.BufferDescriptor, DescriptorBinding.DescriptorType)); break;
        case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: writeDescriptorSet.emplace_back(AddTextureDescriptorSet(DescriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.TextureDescriptor, DescriptorBinding.DescriptorType)); break;
        case VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR: writeDescriptorSet.emplace_back(AddAccelerationBuffer(DescriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.AccelerationStructureDescriptor)); break;
        }
    }
    vkUpdateDescriptorSets(VulkanRenderer::GetDevice(), static_cast<uint32_t>(writeDescriptorSet.size()), writeDescriptorSet.data(), 0, nullptr);
}

void VulkanPipelineTools::AddAccelerationDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkWriteDescriptorSetAccelerationStructureKHR accelerationStructure, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
    DescriptorSetBinding.AccelerationStructureDescriptor = accelerationStructure;
    DescriptorSetBinding.Count = 1;
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void VulkanPipelineTools::AddStorageTextureSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorImageInfo TextureImageInfo, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    DescriptorSetBinding.TextureDescriptor = std::vector<VkDescriptorImageInfo>{ TextureImageInfo };
    DescriptorSetBinding.Count = 1;
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void VulkanPipelineTools::AddTextureDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorImageInfo> TextureImageInfo, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorSetBinding.TextureDescriptor = TextureImageInfo;
    DescriptorSetBinding.Count = TextureImageInfo.size();
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void VulkanPipelineTools::AddTextureDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorImageInfo TextureImageInfo, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorSetBinding.TextureDescriptor = std::vector<VkDescriptorImageInfo>{ TextureImageInfo };
    DescriptorSetBinding.Count = 1;
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void VulkanPipelineTools::AddUniformBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorBufferInfo BufferInfo, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorSetBinding.BufferDescriptor = std::vector<VkDescriptorBufferInfo>{ BufferInfo };
    DescriptorSetBinding.Count = 1;
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void VulkanPipelineTools::AddUniformBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo> BufferInfo, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorSetBinding.BufferDescriptor = BufferInfo;
    DescriptorSetBinding.Count = BufferInfo.size();
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void VulkanPipelineTools::AddStorageBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, VkDescriptorBufferInfo BufferInfo, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    DescriptorSetBinding.BufferDescriptor = std::vector<VkDescriptorBufferInfo>{ BufferInfo };
    DescriptorSetBinding.Count = 1;
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

void VulkanPipelineTools::AddStorageBufferDescriptorSetBinding(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo> BufferInfo, VkShaderStageFlags StageFlags)
{
    DescriptorSetBindingStruct DescriptorSetBinding{};
    DescriptorSetBinding.DescriptorSlotNumber = BindingNumber;
    DescriptorSetBinding.StageFlags = StageFlags;
    DescriptorSetBinding.DescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    DescriptorSetBinding.BufferDescriptor = BufferInfo;
    DescriptorSetBinding.Count = BufferInfo.size();
    DescriptorBindingList.emplace_back(DescriptorSetBinding);
}

VkWriteDescriptorSet VulkanPipelineTools::AddAccelerationBuffer(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, VkWriteDescriptorSetAccelerationStructureKHR& accelerationStructure)
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

VkWriteDescriptorSet VulkanPipelineTools::AddTextureDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, std::vector<VkDescriptorImageInfo>& TextureImageInfo, VkDescriptorType descriptorType)
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

VkWriteDescriptorSet VulkanPipelineTools::AddBufferDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, VkDescriptorBufferInfo& BufferInfo, VkDescriptorType descriptorType)
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

VkWriteDescriptorSet VulkanPipelineTools::AddBufferDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, std::vector<VkDescriptorBufferInfo>& BufferInfoList, VkDescriptorType descriptorType)
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

VkWriteDescriptorSet VulkanPipelineTools::AddTextureDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, uint32_t dstArrayElement, VkDescriptorImageInfo& TextureImageInfo, VkDescriptorType descriptorType)
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

VkWriteDescriptorSet VulkanPipelineTools::AddTextureDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, uint32_t dstArrayElement, std::vector<VkDescriptorImageInfo>& TextureImageInfo, VkDescriptorType descriptorType)
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

VkWriteDescriptorSet VulkanPipelineTools::AddBufferDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, uint32_t dstArrayElement, VkDescriptorBufferInfo& BufferInfo, VkDescriptorType descriptorType)
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

VkWriteDescriptorSet VulkanPipelineTools::AddBufferDescriptorSet(VkDescriptorSet& DescriptorSet, uint32_t BindingNumber, uint32_t dstArrayElement, std::vector<VkDescriptorBufferInfo>& BufferInfoList, VkDescriptorType descriptorType)
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

VkPipelineLayoutCreateInfo VulkanPipelineTools::LoadPipelineLayoutCreateInfo()
{
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    return pipelineLayoutCreateInfo;
}

VkDescriptorBufferInfo VulkanPipelineTools::GetReflectionMapBuffer()
{
    VkDescriptorBufferInfo ReflectionMapBuffer = {};
  //  ReflectionMapBuffer.buffer = ReflectionMapSampler.GetVulkanBufferData().Buffer;
    ReflectionMapBuffer.offset = 0;
    ReflectionMapBuffer.range = VK_WHOLE_SIZE;
    return ReflectionMapBuffer;
}

VkPipelineShaderStageCreateInfo VulkanPipelineTools::LoadPipelineShaderStageCreateInfo(nlohmann::json& json)
{
    VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo{};
    PipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    PipelineShaderStageCreateInfo.stage = json["stage"];
    PipelineShaderStageCreateInfo.module = ReadShaderFile(json["shaderFile"]);
    PipelineShaderStageCreateInfo.pName = "main";
    PipelineShaderStageCreateInfo.pSpecializationInfo = nullptr;
    PipelineShaderStageCreateInfo.pNext = nullptr;

    return PipelineShaderStageCreateInfo;
}

VkPipelineDepthStencilStateCreateInfo VulkanPipelineTools::LoadPipelineDepthStencilStateCreateInfo(nlohmann::json& json)
{
    VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo = {};

    pipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    pipelineDepthStencilStateCreateInfo.pNext = nullptr;
    pipelineDepthStencilStateCreateInfo.depthTestEnable = json["depthTestEnable"];
    pipelineDepthStencilStateCreateInfo.depthWriteEnable = json["depthWriteEnable"];
    pipelineDepthStencilStateCreateInfo.depthCompareOp = json["depthCompareOp"];
    pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = json["depthBoundsTestEnable"];
    pipelineDepthStencilStateCreateInfo.stencilTestEnable = json["stencilTestEnable"];
    //pipelineDepthStencilStateCreateInfo.front = json["front"];
   // pipelineDepthStencilStateCreateInfo.back = json["back"];
    pipelineDepthStencilStateCreateInfo.minDepthBounds = json["minDepthBounds"];
    pipelineDepthStencilStateCreateInfo.maxDepthBounds = json["maxDepthBounds"];

    return pipelineDepthStencilStateCreateInfo;
}

VkPipelineInputAssemblyStateCreateInfo VulkanPipelineTools::LoadPipelineInputAssemblyStateCreateInfo(nlohmann::json& json)
{
    VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo{};
    pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    pipelineInputAssemblyStateCreateInfo.pNext = nullptr;
    pipelineInputAssemblyStateCreateInfo.flags = json["flags"];
    pipelineInputAssemblyStateCreateInfo.topology = json["topology"];
    pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = json["primitiveRestartEnable"];

    return pipelineInputAssemblyStateCreateInfo;
}

VkPipelineViewportStateCreateInfo VulkanPipelineTools::LoadPipelineViewportStateCreateInfo(nlohmann::json& json)
{
    VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo{};
    pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    pipelineViewportStateCreateInfo.pNext = nullptr;
    pipelineViewportStateCreateInfo.flags = json["flags"];
    pipelineViewportStateCreateInfo.viewportCount = json["viewportCount"];
    pipelineViewportStateCreateInfo.pViewports = nullptr;
    pipelineViewportStateCreateInfo.pScissors = nullptr;

    return pipelineViewportStateCreateInfo;
}

VkPipelineRasterizationStateCreateInfo  VulkanPipelineTools::LoadPipelineRasterizationStateCreateInfo(nlohmann::json& json)
{
    VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo{};
    pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    pipelineRasterizationStateCreateInfo.pNext = nullptr;
    pipelineRasterizationStateCreateInfo.depthClampEnable = json["depthClampEnable"];
    pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = json["rasterizerDiscardEnable"];
    pipelineRasterizationStateCreateInfo.polygonMode = json["polygonMode"];
    pipelineRasterizationStateCreateInfo.cullMode = json["cullMode"];
    pipelineRasterizationStateCreateInfo.frontFace = json["frontFace"];
    pipelineRasterizationStateCreateInfo.depthBiasEnable = json["depthBiasEnable"];
    pipelineRasterizationStateCreateInfo.depthBiasConstantFactor = json["depthBiasConstantFactor"];
    pipelineRasterizationStateCreateInfo.depthBiasClamp = json["depthBiasClamp"];
    pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = json["depthBiasSlopeFactor"];
    pipelineRasterizationStateCreateInfo.lineWidth = json["lineWidth"];

    return pipelineRasterizationStateCreateInfo;
}

VkPipelineMultisampleStateCreateInfo VulkanPipelineTools::LoadPipelineMultisampleStateCreateInfo(nlohmann::json& json, VkSampleCountFlagBits sampleCount)
{
    VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo{};
    pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    pipelineMultisampleStateCreateInfo.pNext = nullptr;
    pipelineMultisampleStateCreateInfo.flags = json["flags"];
    pipelineMultisampleStateCreateInfo.rasterizationSamples = sampleCount;
    pipelineMultisampleStateCreateInfo.sampleShadingEnable = json["sampleShadingEnable"];
    pipelineMultisampleStateCreateInfo.minSampleShading = json["minSampleShading"];
    pipelineMultisampleStateCreateInfo.pSampleMask = nullptr;
    pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = json["alphaToCoverageEnable"];
    pipelineMultisampleStateCreateInfo.alphaToOneEnable = json["alphaToOneEnable"];

    return pipelineMultisampleStateCreateInfo;
}

VkPipelineColorBlendStateCreateInfo VulkanPipelineTools::LoadPipelineColorBlendStateCreateInfo(nlohmann::json& json, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments)
{
    VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo{};
    pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pipelineColorBlendStateCreateInfo.flags = json["flags"];
    pipelineColorBlendStateCreateInfo.logicOpEnable = json["logicOpEnable"];
    pipelineColorBlendStateCreateInfo.logicOp = json["logicOp"];
    pipelineColorBlendStateCreateInfo.attachmentCount = static_cast<uint32_t>(ColorAttachments.size());
    pipelineColorBlendStateCreateInfo.pAttachments = ColorAttachments.data();
    pipelineColorBlendStateCreateInfo.blendConstants[0] = json["blendConstants"][0];
    pipelineColorBlendStateCreateInfo.blendConstants[1] = json["blendConstants"][1];
    pipelineColorBlendStateCreateInfo.blendConstants[2] = json["blendConstants"][2];
    pipelineColorBlendStateCreateInfo.blendConstants[3] = json["blendConstants"][3];

    return pipelineColorBlendStateCreateInfo;
}

void VulkanPipelineTools::SaveDescriptorBindingLayout(nlohmann::json& json, VkDescriptorType descriptorType, DescriptorBindingPropertiesEnum descriptorBindingPropertiesEnum)
{
    JsonConverter::to_json(json["descriptorType"], descriptorType);
    JsonConverter::to_json(json["bindingType"], descriptorBindingPropertiesEnum);
}

void VulkanPipelineTools::SaveCreateDescriptorPool(nlohmann::json& json, VkDescriptorPoolCreateInfo descriptorPoolCreateInfo)
{
    //JsonConverter::to_json(json["pNext"], descriptorPool.pNext);
    JsonConverter::to_json(json["flags"], descriptorPoolCreateInfo.flags);
    JsonConverter::to_json(json["maxSets"], descriptorPoolCreateInfo.maxSets);
    JsonConverter::to_json(json["poolSizeCount"], descriptorPoolCreateInfo.poolSizeCount);
    //JsonConverter::to_json(json["pPoolSizes"], descriptorPool.pPoolSizes);
}

void VulkanPipelineTools::SaveBufferDescriptorSet(nlohmann::json& json, VkWriteDescriptorSet writeDescriptorSet)
{
    //JsonConverter::to_json(json["pNext"], writeDescriptorSet.pNext);
    //JsonConverter::to_json(json["dstSet"], writeDescriptorSet.dstSet);
    JsonConverter::to_json(json["dstBinding"], writeDescriptorSet.dstBinding);
    JsonConverter::to_json(json["dstArrayElement"], writeDescriptorSet.dstArrayElement);
    JsonConverter::to_json(json["descriptorCount"], writeDescriptorSet.descriptorCount);
    JsonConverter::to_json(json["descriptorType"], writeDescriptorSet.descriptorType);
    //JsonConverter::to_json(json["pImageInfo"], writeDescriptorSet.pImageInfo);
    //JsonConverter::to_json(json["pBufferInfo"], writeDescriptorSet.pBufferInfo);
    //JsonConverter::to_json(json["pTexelBufferView"], writeDescriptorSet.pTexelBufferView);
}

void VulkanPipelineTools::SavePipelineDepthStencilStateCreateInfo(nlohmann::json& json, VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo)
{
    //JsonConverter::to_json(json["pNext"], pipelineDepthStencilStateCreateInfo.pNext);
    JsonConverter::to_json(json["flags"], pipelineDepthStencilStateCreateInfo.flags);
    JsonConverter::to_json(json["depthTestEnable"], pipelineDepthStencilStateCreateInfo.depthTestEnable);
    JsonConverter::to_json(json["depthWriteEnable"], pipelineDepthStencilStateCreateInfo.depthWriteEnable);
    JsonConverter::to_json(json["depthCompareOp"], pipelineDepthStencilStateCreateInfo.depthCompareOp);
    JsonConverter::to_json(json["depthBoundsTestEnable"], pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable);
    JsonConverter::to_json(json["stencilTestEnable"], pipelineDepthStencilStateCreateInfo.stencilTestEnable);
    //JsonConverter::to_json(json["front"], pipelineDepthStencilStateCreateInfo.front);
    //JsonConverter::to_json(json["back"], pipelineDepthStencilStateCreateInfo.back);
    JsonConverter::to_json(json["minDepthBounds"], pipelineDepthStencilStateCreateInfo.minDepthBounds);
    JsonConverter::to_json(json["maxDepthBounds"], pipelineDepthStencilStateCreateInfo.maxDepthBounds);
}

void VulkanPipelineTools::SavePipelineShaderStageCreateInfo(nlohmann::json& json, VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo, std::string filename)
{
    JsonConverter::to_json(json["flags"], PipelineShaderStageCreateInfo.flags);
    JsonConverter::to_json(json["stage"], PipelineShaderStageCreateInfo.stage);
    JsonConverter::to_json(json["shaderFile"], filename);
    json["pName"] = "main";
    //JsonConverter::to_json(json["pSpecializationInfo"], PipelineShaderStageCreateInfo.pSpecializationInfo);
    //JsonConverter::to_json(json["pNext"], PipelineShaderStageCreateInfo.pNext);
}

void VulkanPipelineTools::SavePipelineInputAssemblyStateCreateInfo(nlohmann::json& json, VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo)
{
    //JsonConverter::to_json(json["pNext"], pipelineInputAssemblyStateCreateInfo.pNext);
    JsonConverter::to_json(json["flags"], pipelineInputAssemblyStateCreateInfo.flags);
    JsonConverter::to_json(json["topology"], pipelineInputAssemblyStateCreateInfo.topology);
    JsonConverter::to_json(json["primitiveRestartEnable"], pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable);
}

void VulkanPipelineTools::SavePipelineViewportStateCreateInfo(nlohmann::json& json, VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo)
{
    //JsonConverter::to_json(json["pNext"], pipelineViewportStateCreateInfo.pNext);
    JsonConverter::to_json(json["flags"], pipelineViewportStateCreateInfo.flags);
    JsonConverter::to_json(json["viewportCount"], pipelineViewportStateCreateInfo.viewportCount);
    //JsonConverter::to_json(json["pViewports"], pipelineViewportStateCreateInfo.pViewports);
    JsonConverter::to_json(json["scissorCount"], pipelineViewportStateCreateInfo.scissorCount);
    // JsonConverter::to_json(json["pScissors"], pipelineViewportStateCreateInfo.pScissors);
}

void VulkanPipelineTools::SavePipelineRasterizationStateCreateInfo(nlohmann::json& json, VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo)
{
    //JsonConverter::to_json(json["pNext"], pipelineRasterizationStateCreateInfo.pNext);
    JsonConverter::to_json(json["flags"], pipelineRasterizationStateCreateInfo.flags);
    JsonConverter::to_json(json["depthClampEnable"], pipelineRasterizationStateCreateInfo.depthClampEnable);
    JsonConverter::to_json(json["rasterizerDiscardEnable"], pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable);
    JsonConverter::to_json(json["polygonMode"], pipelineRasterizationStateCreateInfo.polygonMode);
    JsonConverter::to_json(json["cullMode"], pipelineRasterizationStateCreateInfo.cullMode);
    JsonConverter::to_json(json["frontFace"], pipelineRasterizationStateCreateInfo.frontFace);
    JsonConverter::to_json(json["depthBiasEnable"], pipelineRasterizationStateCreateInfo.depthBiasEnable);
    JsonConverter::to_json(json["depthBiasConstantFactor"], pipelineRasterizationStateCreateInfo.depthBiasConstantFactor);
    JsonConverter::to_json(json["depthBiasClamp"], pipelineRasterizationStateCreateInfo.depthBiasClamp);
    JsonConverter::to_json(json["depthBiasSlopeFactor"], pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor);
    JsonConverter::to_json(json["lineWidth"], pipelineRasterizationStateCreateInfo.lineWidth);
}

void VulkanPipelineTools::SavePipelineMultisampleStateCreateInfo(nlohmann::json& json, VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo)
{
    //JsonConverter::to_json(json["pNext"], pipelineMultisampleStateCreateInfo.pNext);
    JsonConverter::to_json(json["flags"], pipelineMultisampleStateCreateInfo.flags);
    JsonConverter::to_json(json["sampleShadingEnable"], pipelineMultisampleStateCreateInfo.sampleShadingEnable);
    JsonConverter::to_json(json["minSampleShading"], pipelineMultisampleStateCreateInfo.minSampleShading);
    //JsonConverter::to_json(json["pSampleMask"], pipelineMultisampleStateCreateInfo.pSampleMask);
    JsonConverter::to_json(json["alphaToCoverageEnable"], pipelineMultisampleStateCreateInfo.alphaToCoverageEnable);
    JsonConverter::to_json(json["alphaToOneEnable"], pipelineMultisampleStateCreateInfo.alphaToOneEnable);
}

void VulkanPipelineTools::SavePipelineColorBlendStateCreateInfo(nlohmann::json& json, VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo)
{
    JsonConverter::to_json(json["flags"], pipelineColorBlendStateCreateInfo.flags);
    JsonConverter::to_json(json["logicOpEnable"], pipelineColorBlendStateCreateInfo.logicOpEnable);
    JsonConverter::to_json(json["logicOp"], pipelineColorBlendStateCreateInfo.logicOp);
    JsonConverter::to_json(json["blendConstants"][0], pipelineColorBlendStateCreateInfo.blendConstants[0]);
    JsonConverter::to_json(json["blendConstants"][1], pipelineColorBlendStateCreateInfo.blendConstants[1]);
    JsonConverter::to_json(json["blendConstants"][2], pipelineColorBlendStateCreateInfo.blendConstants[2]);
    JsonConverter::to_json(json["blendConstants"][3], pipelineColorBlendStateCreateInfo.blendConstants[3]);
}