#include "GLTF_GraphicsPipeline.h"
#include "JsonGraphicsPipeline.h"
#include "SceneManager.h"
#include "File.h"

JsonGraphicsPipeline::JsonGraphicsPipeline()
{
}

JsonGraphicsPipeline::JsonGraphicsPipeline(const std::string filePath, std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions, std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions, VkRenderPass renderPass, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer)
{
    LoadGraphicsPipeline(filePath, VertexBindingDescriptions, VertexAttributeDescriptions, renderPass, ColorAttachments, samplecount, sizeofConstBuffer);
}

JsonGraphicsPipeline::JsonGraphicsPipeline(const std::string filePath, std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions, std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions, VkRenderPass renderPass, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer, PBRRenderPassTextureSubmitList& submitList)
{
    LoadReflectionIrradianceMapBuffer(submitList.IrradianceTextureList[0]);
    LoadReflectionPrefilterMapBuffer(submitList.PrefilterTextureList[0]);
    LoadDirectionalShadowBuffer(submitList.DirectionalLightTextureShadowMaps[0]);
    LoadPointShadowBuffers(submitList.PointLightShadowMaps);
    LoadGraphicsPipeline(filePath, VertexBindingDescriptions, VertexAttributeDescriptions, renderPass, ColorAttachments, samplecount, sizeofConstBuffer);
}

JsonGraphicsPipeline::JsonGraphicsPipeline(const std::string filePath, std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions, std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions, VkRenderPass renderPass, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer, std::shared_ptr<RenderedCubeMapTexture> cubeMap)
{
    LoadCubeMapBuffer(cubeMap);
    LoadGraphicsPipeline(filePath, VertexBindingDescriptions, VertexAttributeDescriptions, renderPass, ColorAttachments, samplecount, sizeofConstBuffer);
}

JsonGraphicsPipeline::JsonGraphicsPipeline(const std::string filePath, std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions, std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions, VkRenderPass renderPass, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer, std::shared_ptr<RenderedDepthTexture> depthTexture)
{
    LoadDepthTextureBuffer(depthTexture);
    LoadGraphicsPipeline(filePath, VertexBindingDescriptions, VertexAttributeDescriptions, renderPass, ColorAttachments, samplecount, sizeofConstBuffer);
}

JsonGraphicsPipeline::JsonGraphicsPipeline(const std::string filePath, std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions, std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions, VkRenderPass renderPass, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer, std::shared_ptr<RenderedColorTexture> colorTexture)
{
    LoadTextureBuffer(colorTexture);
    LoadGraphicsPipeline(filePath, VertexBindingDescriptions, VertexAttributeDescriptions, renderPass, ColorAttachments, samplecount, sizeofConstBuffer);
}

JsonGraphicsPipeline::JsonGraphicsPipeline(const std::string filePath, std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions, std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions, VkRenderPass renderPass, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer, CubeMapSamplerBuffer& reflectionViewBuffer, PBRRenderPassTextureSubmitList& submitList)
{
    LoadReflectionSamplerBuffer(reflectionViewBuffer);
    LoadReflectionIrradianceMapBuffer(submitList.IrradianceTextureList[0]);
    LoadReflectionPrefilterMapBuffer(submitList.PrefilterTextureList[0]);
    LoadDirectionalShadowBuffer(submitList.DirectionalLightTextureShadowMaps[0]);
    LoadPointShadowBuffers(submitList.PointLightShadowMaps);
    LoadGraphicsPipeline(filePath, VertexBindingDescriptions, VertexAttributeDescriptions, renderPass, ColorAttachments, samplecount, sizeofConstBuffer);
}

JsonGraphicsPipeline::~JsonGraphicsPipeline()
{
}

VkDescriptorPool JsonGraphicsPipeline::CreateDescriptorPool(std::vector<VkDescriptorPoolSize> DescriptorPoolInfo, uint32_t modelCount)
{
    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(DescriptorPoolInfo.size());
    poolInfo.pPoolSizes = DescriptorPoolInfo.data();
    poolInfo.maxSets = 300;

    VkDescriptorPool descriptorPool;
    if (vkCreateDescriptorPool(VulkanRenderer::GetDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create descriptor pool.");
    }

    return descriptorPool;
}

VkDescriptorPool JsonGraphicsPipeline::LoadCreateDescriptorPool(nlohmann::json& json)
{
    VkDescriptorPoolCreateInfo DescriptorPoolCreateInfo = {};
    DescriptorPoolCreateInfo.pNext = nullptr;
    DescriptorPoolCreateInfo.flags = json["flags"];
    DescriptorPoolCreateInfo.maxSets = json["maxSets"];
    DescriptorPoolCreateInfo.poolSizeCount = json["poolSizeCount"];
    DescriptorPoolCreateInfo.pPoolSizes = nullptr;

    VkDescriptorPool DescriptorPool;
    if (vkCreateDescriptorPool(VulkanRenderer::GetDevice(), &DescriptorPoolCreateInfo, nullptr, &DescriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor pool.");
    }

    return DescriptorPool;
}

void JsonGraphicsPipeline::LoadGraphicsPipeline(const std::string filePath, std::vector<VkVertexInputBindingDescription> VertexBindingDescriptions, std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions, VkRenderPass renderPass, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits samplecount, uint32_t sizeofConstBuffer)
{
    if (ShaderPipeline != VK_NULL_HANDLE)
    {
        Destroy();
    }

    std::string SceneInfo;
    std::ifstream SceneFile;
    SceneFile.open(File::OpenFile(filePath));
    if (SceneFile.is_open())
    {
        while (!SceneFile.eof())
        {
            getline(SceneFile, SceneInfo);
        }
    }
    else std::cout << "Unable to open file";
    SceneFile.close();

    nlohmann::json json = nlohmann::json::parse(SceneInfo);

    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    for (int x = 0; x < json["Shader"].size(); x++)
    {
        PipelineShaderStageList.emplace_back(LoadPipelineShaderStageCreateInfo(json["Shader"][x]));
    }

    LoadDescriptorSets(json["DescriptorBindingLayout"]);

    if (ShaderPipeline == nullptr)
    {
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(VertexBindingDescriptions.size());
        vertexInputInfo.pVertexBindingDescriptions = VertexBindingDescriptions.data();
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(VertexAttributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = VertexAttributeDescriptions.data();

        VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = LoadPipelineDepthStencilStateCreateInfo(json["PipelineDepthStencilStateCreateInfo"]);
        VkPipelineInputAssemblyStateCreateInfo inputAssembly = LoadPipelineInputAssemblyStateCreateInfo(json["PipelineInputAssemblyStateCreateInfo"]);
        VkPipelineViewportStateCreateInfo viewportState = LoadPipelineViewportStateCreateInfo(json["PipelineViewportStateCreateInfo"]);
        VkPipelineRasterizationStateCreateInfo rasterizer = LoadPipelineRasterizationStateCreateInfo(json["PipelineRasterizationStateCreateInfo"]);
        VkPipelineMultisampleStateCreateInfo multisampling = LoadPipelineMultisampleStateCreateInfo(json["PipelineMultisampleStateCreateInfo"], samplecount);
        VkPipelineColorBlendStateCreateInfo colorBlending = LoadPipelineColorBlendStateCreateInfo(json["PipelineColorBlendStateCreateInfo"], ColorAttachments);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &DescriptorSetLayout;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        if (sizeofConstBuffer != 0)
        {
            VkPushConstantRange pushConstantRange{};
            pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            pushConstantRange.offset = 0;
            pushConstantRange.size = sizeofConstBuffer;
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
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = &depthStencilStateCreateInfo;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.layout = ShaderPipelineLayout;
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        VkResult result = vkCreateGraphicsPipelines(VulkanRenderer::GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &ShaderPipeline);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline.");
        }
    }

    for (auto& shader : PipelineShaderStageList)
    {
        vkDestroyShaderModule(VulkanRenderer::GetDevice(), shader.module, nullptr);
    }
}

void JsonGraphicsPipeline::SaveGraphicsPipeline(const std::string fileName, nlohmann::json& json)
{
    std::ofstream pipelineFile(PathConsts::PipelinePath + fileName);
    pipelineFile << json;
    pipelineFile.close();
}

void JsonGraphicsPipeline::BuildAndSaveShaderPipeLine(nlohmann::json& json, BuildGraphicsPipelineInfo& buildPipelineInfo, VkDescriptorSetLayout descriptorSetLayout)
{
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = buildPipelineInfo.VertexDescription.VertexTopology;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
    SavePipelineInputAssemblyStateCreateInfo(json["PipelineInputAssemblyStateCreateInfo"], inputAssembly);

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    SavePipelineViewportStateCreateInfo(json["PipelineViewportStateCreateInfo"], viewportState);

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.lineWidth = 1.0f;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.cullMode = buildPipelineInfo.VertexDescription.CullMode;
    rasterizer.polygonMode = buildPipelineInfo.VertexDescription.PolygonMode;
    rasterizer.depthBiasEnable = VK_FALSE;
    SavePipelineRasterizationStateCreateInfo(json["PipelineRasterizationStateCreateInfo"], rasterizer);

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_TRUE;
    multisampling.rasterizationSamples = buildPipelineInfo.RenderPassDescription.sampleCount;
    SavePipelineMultisampleStateCreateInfo(json["PipelineMultisampleStateCreateInfo"], multisampling);

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.attachmentCount = static_cast<uint32_t>(buildPipelineInfo.RenderPassDescription.ColorAttachments.size());
    colorBlending.pAttachments = buildPipelineInfo.RenderPassDescription.ColorAttachments.data();
    SavePipelineColorBlendStateCreateInfo(json["PipelineColorBlendStateCreateInfo"], colorBlending);
}

void JsonGraphicsPipeline::Destroy()
{
    vkDestroyPipeline(VulkanRenderer::GetDevice(), ShaderPipeline, nullptr);
    vkDestroyPipelineLayout(VulkanRenderer::GetDevice(), ShaderPipelineLayout, nullptr);
    vkDestroyDescriptorPool(VulkanRenderer::GetDevice(), DescriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(VulkanRenderer::GetDevice(), DescriptorSetLayout, nullptr);

    ShaderPipeline = VK_NULL_HANDLE;
    ShaderPipelineLayout = VK_NULL_HANDLE;
    DescriptorPool = VK_NULL_HANDLE;
    DescriptorSetLayout = VK_NULL_HANDLE;

   // ReflectionSampler.Destroy();
  //  ReflectionMapSampler.Destroy();
}