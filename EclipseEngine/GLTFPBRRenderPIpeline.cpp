#include "GLTFPBRRenderPIpeline.h"
#include "SceneManager.h"

GLTFPBRRenderPIpeline::GLTFPBRRenderPIpeline()
{
}

GLTFPBRRenderPIpeline::~GLTFPBRRenderPIpeline()
{
}

void GLTFPBRRenderPIpeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::vector<std::shared_ptr<GLTF_Temp_Model>> modelList)
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

    jsonPipeline.SavePipelineShaderStageCreateInfo(json["Shader"][0], a, BaseShaderFilePath + "GLTFPBRRendererVert.spv");
    jsonPipeline.SavePipelineShaderStageCreateInfo(json["Shader"][1], b, BaseShaderFilePath + "GLTFPBRRendererFrag.spv");

    for (int x = 0; x < json["Shader"].size(); x++)
    {
        PipelineShaderStageList.emplace_back(jsonPipeline.LoadPipelineShaderStageCreateInfo(json["Shader"][x]));
    }

    std::vector<VkDescriptorPool> DescriptorPool1;

        std::vector<VkDescriptorPoolSize> descriptorPoolSizeList;
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)modelList[0]->GetMeshPropertiesBuffer().size()});
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1 });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetMaterialPropertiesBuffer().size() });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, (uint32_t)GLTFSceneManager::GetTexturePropertiesBuffer().size() });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetSunLightPropertiesBuffer().size() });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetDirectionalLightPropertiesBuffer().size() });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetPointLightPropertiesBuffer().size() });
        descriptorPoolSizeList.emplace_back(VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetSpotLightPropertiesBuffer().size() });

        jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][0], VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, DescriptorBindingPropertiesEnum::kModelTransformDescriptor);
        jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][1], VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, DescriptorBindingPropertiesEnum::kMeshPropertiesDescriptor);
        jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][2], VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, DescriptorBindingPropertiesEnum::kMaterialDescriptor);
        jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][3], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, DescriptorBindingPropertiesEnum::kTextureDescriptor);
        jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][4], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, DescriptorBindingPropertiesEnum::kBRDFMapDescriptor);
        jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][5], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, DescriptorBindingPropertiesEnum::kIrradianceMapDescriptor);
        jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][6], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, DescriptorBindingPropertiesEnum::kPrefilterMapDescriptor);
        jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][7], VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, DescriptorBindingPropertiesEnum::kSunLightDescriptor);
        jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][8], VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, DescriptorBindingPropertiesEnum::kDirectionalLightDescriptor);
        jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][9], VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, DescriptorBindingPropertiesEnum::kPointLightDescriptor);
        jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][10], VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, DescriptorBindingPropertiesEnum::kSpotLightDescriptor);
        DescriptorPool1.emplace_back(jsonPipeline.CreateDescriptorPool(descriptorPoolSizeList, modelList.size()));
    
 
        std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBinding;
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)modelList[0]->GetMeshPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetMaterialPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, (uint32_t)GLTFSceneManager::GetTexturePropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 5, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 6, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 7, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetSunLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 8, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetDirectionalLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 9, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetPointLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
        descriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding{ 10, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (uint32_t)GLTFSceneManager::GetSpotLightPropertiesBuffer().size(), VK_SHADER_STAGE_ALL });
        DescriptorSetLayoutList.emplace_back(GLTF_GraphicsDescriptors::CreateDescriptorSetLayout(descriptorSetLayoutBinding));
    
        for (int y = 0; y < modelList[0]->MaterialList.size(); y++)
        {
            std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
            GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, modelList[0]->GetMeshPropertiesBuffer());
            GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 1, modelList[0]->MeshList[0]->GetTransformMatrixBuffer()[0]);
            GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 2, GLTFSceneManager::GetMaterialPropertiesBuffer());
            GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 3, GLTFSceneManager::GetTexturePropertiesBuffer());
            GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 4, GLTFSceneManager::GetBRDFMapDescriptor());
            GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 5, GLTFSceneManager::GetIrradianceMapDescriptor());
            GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 6, GLTFSceneManager::GetPrefilterMapDescriptor());
            GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 7, GLTFSceneManager::GetSunLightPropertiesBuffer());
            GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 8, GLTFSceneManager::GetDirectionalLightPropertiesBuffer());
            GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 9, GLTFSceneManager::GetPointLightPropertiesBuffer());
            GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 10, GLTFSceneManager::GetSpotLightPropertiesBuffer());
            descriptorSet = GLTF_GraphicsDescriptors::CreateDescriptorSets(DescriptorPool1[0], DescriptorSetLayoutList[0]);

            std::vector<VkWriteDescriptorSet> writeDescriptorSet;
            for (auto& DescriptorBinding : DescriptorBindingList)
            {
                if (DescriptorBinding.BufferDescriptor.size() > 0)
                {
                    writeDescriptorSet.emplace_back(GLTF_GraphicsDescriptors::AddBufferDescriptorSet(descriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.BufferDescriptor, DescriptorBinding.DescriptorType));
                }
                else if (DescriptorBinding.TextureDescriptor.size() > 0)
                {
                    writeDescriptorSet.emplace_back(GLTF_GraphicsDescriptors::AddTextureDescriptorSet(descriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.TextureDescriptor, DescriptorBinding.DescriptorType));
                }
                else
                {
                    writeDescriptorSet.emplace_back(GLTF_GraphicsDescriptors::AddAccelerationBuffer(descriptorSet, DescriptorBinding.DescriptorSlotNumber, DescriptorBinding.AccelerationStructureDescriptor));
                }
            }
            vkUpdateDescriptorSets(VulkanRenderer::GetDevice(), static_cast<uint32_t>(writeDescriptorSet.size()), writeDescriptorSet.data(), 0, nullptr);
        }
    

    if (ShaderPipeline == nullptr)
    {
        const std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions = Vertex3D::getBindingDescriptions();
        const std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions = Vertex3D::getAttributeDescriptions();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(VertexBindingDescriptions.size());
        vertexInputInfo.pVertexBindingDescriptions = VertexBindingDescriptions.data();
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(VertexAttributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = VertexAttributeDescriptions.data();

        VkPipelineDepthStencilStateCreateInfo saveDepthStencilStateCreateInfo{};
        saveDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        saveDepthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
        saveDepthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
        saveDepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
        saveDepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
        saveDepthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        jsonPipeline.SavePipelineDepthStencilStateCreateInfo(json["PipelineDepthStencilStateCreateInfo"], saveDepthStencilStateCreateInfo);

        VkPipelineInputAssemblyStateCreateInfo saveinputAssembly = {};
        saveinputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        saveinputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        saveinputAssembly.primitiveRestartEnable = VK_FALSE;
        jsonPipeline.SavePipelineInputAssemblyStateCreateInfo(json["PipelineInputAssemblyStateCreateInfo"], saveinputAssembly);

        VkPipelineViewportStateCreateInfo saveviewportState{};
        saveviewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        saveviewportState.scissorCount = 0;
        saveviewportState.viewportCount = 0;
        jsonPipeline.SavePipelineViewportStateCreateInfo(json["PipelineViewportStateCreateInfo"], saveviewportState);

        VkPipelineRasterizationStateCreateInfo saverasterizer{};
        saverasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        saverasterizer.depthClampEnable = VK_FALSE;
        saverasterizer.rasterizerDiscardEnable = VK_FALSE;
        saverasterizer.lineWidth = 1.0f;
        saverasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        saverasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        saverasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        saverasterizer.depthBiasEnable = VK_FALSE;
        jsonPipeline.SavePipelineRasterizationStateCreateInfo(json["PipelineRasterizationStateCreateInfo"], saverasterizer);

        VkPipelineMultisampleStateCreateInfo savemultisampling{};
        savemultisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        savemultisampling.sampleShadingEnable = VK_TRUE;
        savemultisampling.rasterizationSamples = pipelineInfoStruct.SampleCount;
        jsonPipeline.SavePipelineMultisampleStateCreateInfo(json["PipelineMultisampleStateCreateInfo"], savemultisampling);

        VkPipelineColorBlendStateCreateInfo savecolorBlending{};
        savecolorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        savecolorBlending.attachmentCount = static_cast<uint32_t>(pipelineInfoStruct.ColorAttachments.size());
        savecolorBlending.pAttachments = pipelineInfoStruct.ColorAttachments.data();
        jsonPipeline.SavePipelineColorBlendStateCreateInfo(json["PipelineColorBlendStateCreateInfo"], savecolorBlending);

        std::cout << json << std::endl;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        if (DescriptorSetLayoutList.size() == 0)
        {
            pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipelineLayoutInfo.setLayoutCount = 1;
            pipelineLayoutInfo.pSetLayouts = &DescriptorSetLayout;
        }
        else
        {
            pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(DescriptorSetLayoutList.size());
            pipelineLayoutInfo.pSetLayouts = DescriptorSetLayoutList.data();
        }
        pipelineLayoutInfo.pushConstantRangeCount = 1;

        if (sizeof(SceneProperties) != 0)
        {
            VkPushConstantRange pushConstantRange{};
            pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            pushConstantRange.offset = 0;
            pushConstantRange.size = sizeof(SceneProperties);
            pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        }

        if (vkCreatePipelineLayout(VulkanRenderer::GetDevice(), &pipelineLayoutInfo, nullptr, &ShaderPipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline layout.");
        }

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = static_cast<uint32_t>(PipelineShaderStageList.size());
        pipelineInfo.pStages = PipelineShaderStageList.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &saveinputAssembly;
        pipelineInfo.pViewportState = &saveviewportState;
        pipelineInfo.pRasterizationState = &saverasterizer;
        pipelineInfo.pMultisampleState = &savemultisampling;
        pipelineInfo.pDepthStencilState = &saveDepthStencilStateCreateInfo;
        pipelineInfo.pColorBlendState = &savecolorBlending;
        pipelineInfo.layout = ShaderPipelineLayout;
        pipelineInfo.renderPass = pipelineInfoStruct.renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        VkResult result = vkCreateGraphicsPipelines(VulkanRenderer::GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &ShaderPipeline);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline.");
        }

        jsonPipeline.SaveGraphicsPipeline("GLTFPBRPipeline.txt", json);
        // jsonPipeline.LoadGraphicsPipeline("GLTFPBRPipeline.txt", pipelineInfoStruct.renderPass, modelList, pipelineInfoStruct.ColorAttachments, sizeof(SceneProperties));

    }
    else
    {
        Destroy();
        //  UpdateGraphicsPipeLine(buildGraphicsPipelineInfo);
    }

    for (auto& shader : PipelineShaderStageList)
    {
        vkDestroyShaderModule(VulkanRenderer::GetDevice(), shader.module, nullptr);
    }
}

void GLTFPBRRenderPIpeline::Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<GLTF_Temp_Model> model)
{
    //vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
    //model->Draw(commandBuffer, ShaderPipelineLayout);
}