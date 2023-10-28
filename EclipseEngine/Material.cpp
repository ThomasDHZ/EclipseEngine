#include "Material.h"

uint64_t Material::MaterialIDCounter = 0;

Material::Material()
{
}

Material::Material(const std::string& materialName)
{
	MaterialName = materialName;
	MaterialIDCounter++;
	MaterialBufferIndex = MaterialIDCounter;
	MaterialBuffer = VulkanBuffer(&MaterialBuffer, sizeof(GLTFMaterialBufferInfo), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

Material::~Material()
{
}

void Material::GenerateID()
{
	MaterialIDCounter++;
	MaterialID = MaterialIDCounter;
}

void Material::UpdateMaterialBufferIndex(uint64_t bufferIndex)
{
	MaterialBufferIndex = bufferIndex;
}

void Material::UpdateBuffer()
{
	MaterialInfo.Albedo = Albedo;
	MaterialInfo.Metallic = Metallic;
	MaterialInfo.Roughness = Roughness;
	MaterialInfo.AmbientOcclusion = AmbientOcclusion;
	MaterialInfo.Emission = Emission;
	MaterialInfo.Alpha = Alpha;
	MaterialInfo.CheckPaletteSwapColor = CheckPaletteSwapColor;

	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::Destroy()
{
	MaterialBuffer.DestroyBuffer();
}

void Material::GetMaterialPropertiesBuffer(std::vector<VkDescriptorBufferInfo>& MaterialBufferList)
{
	VkDescriptorBufferInfo MaterialBufferInfo = {};
	MaterialBufferInfo.buffer = MaterialBuffer.Buffer;
	MaterialBufferInfo.offset = 0;
	MaterialBufferInfo.range = VK_WHOLE_SIZE;
	MaterialBufferList.emplace_back(MaterialBufferInfo);
}

void Material::SetAlbedo(glm::vec3 color)
{
	Albedo = color;
	MaterialInfo.Albedo = color;
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::SetMetallic(float value)
{
	Metallic = value;
	MaterialInfo.Metallic = value;
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::SetRoughness(float value)
{
	Roughness = value;
	MaterialInfo.Roughness = value;
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::SetAmbientOcclusion(float value)
{
	AmbientOcclusion = value;
	MaterialInfo.AmbientOcclusion = value;
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::SetEmission(glm::vec3 color)
{
	Emission = color;
	MaterialInfo.Emission = color;
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::SetTransmission(float value)
{
	Transmission = value;
	MaterialInfo.Transmission = value;
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::SetIndexOfRefraction(float value)
{
	IndexOfRefraction = value;
	MaterialInfo.IndexOfRefraction = value;
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::SetAlpha(float value)
{
	Alpha = value;
	MaterialInfo.Alpha = value;
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::SetCheckPaletteSwapColor(glm::vec3 color)
{
	CheckPaletteSwapColor = color;
	MaterialInfo.CheckPaletteSwapColor = color;
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::SetPaletteSwapPixelColor(glm::vec3 color)
{
	PaletteSwapPixelColor = color;
	MaterialInfo.PaletteSwapPixelColor = color;
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::SetAlbedoMap(std::shared_ptr<Texture> texture)
{
	AlbedoMap = texture;
	MaterialInfo.AlbedoMap = AlbedoMap->GetTextureBufferIndex();
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::SetMetallicRoughnessMap(std::shared_ptr<Texture> texture)
{
	MetallicRoughnessMap = texture;
	MaterialInfo.MetallicRoughnessMap = MetallicRoughnessMap->GetTextureBufferIndex();
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::SetMetallicMap(std::shared_ptr<Texture> texture)
{
	MetallicMap = texture;
	MaterialInfo.MetallicMap = MetallicMap->GetTextureBufferIndex();
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::SetRoughnessMap(std::shared_ptr<Texture> texture)
{
	RoughnessMap = texture;
	MaterialInfo.RoughnessMap = RoughnessMap->GetTextureBufferIndex();
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::SetAmbientOcclusionMap(std::shared_ptr<Texture> texture)
{
	AmbientOcclusionMap = texture;
	MaterialInfo.AmbientOcclusionMap = AmbientOcclusionMap->GetTextureBufferIndex();
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::SetNormalMap(std::shared_ptr<Texture> texture)
{
	NormalMap = texture;
	MaterialInfo.NormalMap = NormalMap->GetTextureBufferIndex();
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::SetDepthMap(std::shared_ptr<Texture> texture)
{
	DepthMap = texture;
	MaterialInfo.DepthMap = DepthMap->GetTextureBufferIndex();
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::SetAlphaMap(std::shared_ptr<Texture> texture)
{
	AlphaMap = texture;
	MaterialInfo.AlphaMap = AlphaMap->GetTextureBufferIndex();
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::SetEmissionMap(std::shared_ptr<Texture> texture)
{
	EmissionMap = texture;
	MaterialInfo.EmissionMap = EmissionMap->GetTextureBufferIndex();
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::SetTransmissionMap(std::shared_ptr<Texture> texture)
{
	TransmissionMap = texture;
	MaterialInfo.TransmissionMap = TransmissionMap->GetTextureBufferIndex();
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::SetIndexOfRefractionMap(std::shared_ptr<Texture> texture)
{
	IndexOfRefractionMap = texture;
	MaterialInfo.IndexOfRefractionMap = IndexOfRefractionMap->GetTextureBufferIndex();
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::SetHeightMap(std::shared_ptr<Texture> texture)
{
	HeightMap = texture;
	MaterialInfo.HeightMap = HeightMap->GetTextureBufferIndex();
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::SetCheckPaletteSwapColorMap(std::shared_ptr<Texture> texture)
{
	CheckPaletteSwapColorMap = texture;
	MaterialInfo.CheckPaletteSwapColorMap = CheckPaletteSwapColorMap->GetTextureBufferIndex();
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}

void Material::SetPaletteSwapPixelColorMap(std::shared_ptr<Texture> texture)
{
	PaletteSwapPixelColorMap = texture;
	MaterialInfo.PaletteSwapPixelColorMap = PaletteSwapPixelColorMap->GetTextureBufferIndex();
	MaterialBuffer.UpdateBufferMemory(&MaterialInfo, sizeof(GLTFMaterialBufferInfo));
}
