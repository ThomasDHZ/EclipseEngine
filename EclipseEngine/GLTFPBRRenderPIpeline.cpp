#include "GLTFPBRRenderPIpeline.h"
#include "SceneManager.h"

GLTFPBRRenderPIpeline::GLTFPBRRenderPIpeline()
{
}

GLTFPBRRenderPIpeline::~GLTFPBRRenderPIpeline()
{
}

void GLTFPBRRenderPIpeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::vector<GLTF_Temp_Model> modelList)
{
    VkSampler Sampler = nullptr;
    VkSamplerCreateInfo TextureImageSamplerInfo = {};
    TextureImageSamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    TextureImageSamplerInfo.magFilter = VK_FILTER_NEAREST;
    TextureImageSamplerInfo.minFilter = VK_FILTER_NEAREST;
    TextureImageSamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    TextureImageSamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    TextureImageSamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    TextureImageSamplerInfo.anisotropyEnable = VK_TRUE;
    TextureImageSamplerInfo.maxAnisotropy = 16.0f;
    TextureImageSamplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    TextureImageSamplerInfo.unnormalizedCoordinates = VK_FALSE;
    TextureImageSamplerInfo.compareEnable = VK_FALSE;
    TextureImageSamplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    TextureImageSamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    TextureImageSamplerInfo.minLod = 0;
    TextureImageSamplerInfo.maxLod = 0;
    TextureImageSamplerInfo.mipLodBias = 0;

    if (vkCreateSampler(VulkanRenderer::GetDevice(), &TextureImageSamplerInfo, nullptr, &Sampler))
    {
        throw std::runtime_error("Failed to create Sampler.");
    }

    std::vector<VkDescriptorImageInfo> TextureDescriptorList;
    VkDescriptorImageInfo nullBuffer;
    nullBuffer.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    nullBuffer.imageView = VK_NULL_HANDLE;
    nullBuffer.sampler = Sampler;


    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    auto a = CreateShader(BaseShaderFilePath + "GLTFPBRRendererVert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    auto b = CreateShader(BaseShaderFilePath + "GLTFPBRRendererFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

    nlohmann::json json;
    JsonGraphicsPipeline jsonPipeline{};
    jsonPipeline.SaveVKPipelineShaderStageCreateInfo(json["Shader"][0], a, BaseShaderFilePath + "GLTFPBRRendererVert.spv");
    jsonPipeline.SaveVKPipelineShaderStageCreateInfo(json["Shader"][1], b, BaseShaderFilePath + "GLTFPBRRendererFrag.spv");

    for (int x = 0; x < json["Shader"].size(); x++)
    {
        PipelineShaderStageList.emplace_back(jsonPipeline.LoadVKPipelineShaderStageCreateInfo(json["Shader"][x]));
    }

    std::vector<VkDescriptorPool> DescriptorPool1;
    for (auto& model : modelList)
    {
        std::vector<VkDescriptorPoolSize> descriptorPoolSizeList;
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)model.GetMeshPropertiesBuffer().size() });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1 });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)model.GetSunLightPropertiesBuffer().size() });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)model.GetDirectionalLightPropertiesBuffer().size() });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)model.GetPointLightPropertiesBuffer().size() });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)model.GetSpotLightPropertiesBuffer().size() });

        for (int x = 0; x < descriptorPoolSizeList.size(); x++)
        {
            jsonPipeline.SaveDescriptorPoolSize(json["DescriptorPoolSize"][x], descriptorPoolSizeList[x]);
        }

        std::vector<VkDescriptorPoolSize> loaddescriptorPoolSizeList;
        for (int x = 0; x < json["DescriptorPoolSize"].size(); x++)
        {
            loaddescriptorPoolSizeList.emplace_back(jsonPipeline.LoadDescriptorPoolSize(json["DescriptorPoolSize"][x]));
        }
        DescriptorPool1.emplace_back(jsonPipeline.CreateDescriptorPool(loaddescriptorPoolSizeList, modelList.size()));
    }

    for (int z = 0; z < modelList.size(); z++)
    {
        std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBinding;
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)modelList[z].GetMeshPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 5, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 6, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 , VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 7, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 8, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 9, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 10, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 11, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)modelList[z].GetSunLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 12, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)modelList[z].GetDirectionalLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 13, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)modelList[z].GetPointLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 14, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)modelList[z].GetSpotLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
        for (int x = 0; x < descriptorSetLayoutBinding.size(); x++)
        {
            jsonPipeline.SaveDescriptorLayoutSet(json["DescriptorSetLayoutBinding"][x], descriptorSetLayoutBinding[x]);
        }

        std::vector<VkDescriptorSetLayoutBinding> loaddescriptorSetLayoutBinding;
        for (int x = 0; x < json["DescriptorSetLayoutBinding"].size(); x++)
        {
            loaddescriptorSetLayoutBinding.emplace_back(jsonPipeline.LoadDescriptorLayoutSet(json["DescriptorSetLayoutBinding"][x]));
        }
        DescriptorSetLayoutList.emplace_back(GLTF_GraphicsDescriptors::CreateDescriptorSetLayout(descriptorSetLayoutBinding));
    }

        for (int z = 0; z < modelList.size(); z++)
        {
            for (int y = 0; y < modelList[z].MaterialList.size(); y++)
            {
                std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
                GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, modelList[z].GetMeshPropertiesBuffer());
                GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 1, modelList[z].MeshList[0]->GetTransformMatrixBuffer()[0]);
                GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 2, modelList[z].MaterialList[y]->GetAlbedoMapDescriptor());
                GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 3, modelList[z].MaterialList[y]->GetNormalMapDescriptor());
                GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 4, modelList[z].MaterialList[y]->GetMetallicRoughnessMapDescriptor());
                GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 5, modelList[z].MaterialList[y]->GetAmbientOcclusionMapDescriptor());
                GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 6, modelList[z].MaterialList[y]->GetAlphaMapDescriptor());
                GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 7, modelList[z].MaterialList[y]->GetDepthMapDescriptor());
                GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 8, SceneManager::GetBRDFMapDescriptor());
                GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 9, SceneManager::GetIrradianceMapDescriptor());
                GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 10, SceneManager::GetPrefilterMapDescriptor());
                GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 11, modelList[z].GetSunLightPropertiesBuffer());
                GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 12, modelList[z].GetDirectionalLightPropertiesBuffer());
                GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 13, modelList[z].GetPointLightPropertiesBuffer());
                GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 14, modelList[z].GetSpotLightPropertiesBuffer());
                modelList[z].MaterialList[y]->descriptorSet = GLTF_GraphicsDescriptors::CreateDescriptorSets(DescriptorPool1[z], DescriptorSetLayoutList[z]);

                std::vector<VkWriteDescriptorSet> writeDescriptorSet;
                for (auto& DescriptorBinding : DescriptorBindingList)
                {
                    if (DescriptorBinding.BufferDescriptor.size() > 0)
                    {
                        writeDescriptorSet.emplace_back(GLTF_GraphicsDescriptors::AddBufferDescriptorSet(modelList[z].MaterialList[y]->descriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.BufferDescriptor, DescriptorBinding.DescriptorType));
                    }
                    else if (DescriptorBinding.TextureDescriptor.size() > 0)
                    {
                        writeDescriptorSet.emplace_back(GLTF_GraphicsDescriptors::AddTextureDescriptorSet(modelList[z].MaterialList[y]->descriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.TextureDescriptor, DescriptorBinding.DescriptorType));
                    }
                    else
                    {
                        writeDescriptorSet.emplace_back(GLTF_GraphicsDescriptors::AddAccelerationBuffer(modelList[z].MaterialList[y]->descriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.AccelerationStructureDescriptor));
                    }
                }
                vkUpdateDescriptorSets(VulkanRenderer::GetDevice(), static_cast<uint32_t>(writeDescriptorSet.size()), writeDescriptorSet.data(), 0, nullptr);
                for (int x = 0; x < modelList[z].MeshList.size(); x++)
                {
                    modelList[z].MeshList[x]->MaterialList = modelList[z].MaterialList;
                }
            }
        }
    
    std::cout << json << std::endl;


    VkPipelineDepthStencilStateCreateInfo DepthStencilStateCreateInfo{};
    DepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    DepthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
    DepthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
    DepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
    DepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
    DepthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;

    BuildVertexDescription VertexDescriptionInfo{};
    VertexDescriptionInfo.VertexBindingDescriptions = Vertex3D::getBindingDescriptions();
    VertexDescriptionInfo.VertexAttributeDescriptions = Vertex3D::getAttributeDescriptions();
    VertexDescriptionInfo.VertexTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    VertexDescriptionInfo.PolygonMode = VK_POLYGON_MODE_FILL;
    VertexDescriptionInfo.CullMode = VK_CULL_MODE_BACK_BIT;

    BuildRenderPassDescription RenderPassInfo{};
    RenderPassInfo.PipelineShaderStageList = PipelineShaderStageList;
    RenderPassInfo.ColorAttachments = pipelineInfoStruct.ColorAttachments;
    RenderPassInfo.DepthStencilInfo = DepthStencilStateCreateInfo;
    RenderPassInfo.renderPass = pipelineInfoStruct.renderPass;
    RenderPassInfo.sampleCount = pipelineInfoStruct.SampleCount;
    RenderPassInfo.ConstBufferSize = sizeof(SceneProperties);

    BuildGraphicsPipelineInfo buildGraphicsPipelineInfo{};
    buildGraphicsPipelineInfo.VertexDescription = VertexDescriptionInfo;
    buildGraphicsPipelineInfo.RenderPassDescription = RenderPassInfo;

    if (ShaderPipeline == nullptr)
    {
        BuildShaderPipeLine(buildGraphicsPipelineInfo);
    }
    else
    {
        Destroy();
        UpdateGraphicsPipeLine(buildGraphicsPipelineInfo);
    }

    for (auto& shader : PipelineShaderStageList)
    {
        vkDestroyShaderModule(VulkanRenderer::GetDevice(), shader.module, nullptr);
    }
}

void GLTFPBRRenderPIpeline::Draw(VkCommandBuffer& commandBuffer, GLTF_Temp_Model model, uint32_t descriptorsetIndex)
{
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
    model.Draw(commandBuffer, ShaderPipelineLayout, descriptorsetIndex);
}
