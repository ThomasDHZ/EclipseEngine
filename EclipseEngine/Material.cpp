#include "Material.h"

uint64_t Material::MaterialIDCounter = 0;

Material::Material()
{
}

Material::Material(const std::string materialName)
{
	GenerateID();
	MaterialName = materialName;

	MaterialBuffer.CreateBuffer(&materialTextureData, sizeof(MaterialProperties), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	VulkanRenderer::UpdateRendererFlag = true;
}

Material::Material(const std::string materialName, MaterialProperties& MaterialInfo)
{
	GenerateID();
	MaterialName = materialName;
	materialTextureData = MaterialInfo;

	MaterialBuffer.CreateBuffer(&materialTextureData, sizeof(MaterialProperties), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	VulkanRenderer::UpdateRendererFlag = true;
}

Material::~Material()
{
}

void Material::Update()
{
	materialTextureData = materialProperties;
	MaterialBuffer.CopyBufferToMemory(&materialTextureData, sizeof(MaterialBufferData));
}

void Material::Destroy()
{
	MaterialBuffer.DestoryBuffer();
}

void Material::GenerateID()
{
	MaterialIDCounter++;
	MaterialID = MaterialIDCounter;
}

void Material::SetBufferIndex(int bufferIndex)
{
	MaterialBufferIndex = bufferIndex;
}