//#include "GLTF_GraphicsPipeline.h"
//#include <fstream>
//
//std::string GLTF_GraphicsPipeline::BaseShaderFilePath = "../Shaders/";
//
//GLTF_GraphicsPipeline::GLTF_GraphicsPipeline()
//{
//}
//
//GLTF_GraphicsPipeline::GLTF_GraphicsPipeline(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo)
//{
//    BuildShaderPipeLine(buildGraphicsPipelineInfo);
//}
//
//GLTF_GraphicsPipeline::~GLTF_GraphicsPipeline()
//{
//}
//
//VkShaderModule GLTF_GraphicsPipeline::ReadShaderFile(const std::string& filename)
//{
//    std::ifstream file(filename, std::ios::ate | std::ios::binary);
//
//    if (!file.is_open()) {
//        throw std::runtime_error("Failed to open file: " + filename);
//    }
//
//    size_t fileSize = (size_t)file.tellg();
//    std::vector<char> buffer(fileSize);
//
//    file.seekg(0);
//    file.read(buffer.data(), fileSize);
//
//    file.close();
//
//    VkShaderModuleCreateInfo createInfo = {};
//    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
//    createInfo.codeSize = buffer.size();
//    createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());
//
//    VkShaderModule shaderModule;
//    if (vkCreateShaderModule(VulkanRenderer::GetDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
//        throw std::runtime_error("Failed to create shader module.");
//    }
//
//    return shaderModule;
//}
//
//void GLTF_GraphicsPipeline::BuildShaderPipeLine(BuildGraphicsPipelineInfo& buildPipelineInfo)
//{
//    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
//    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
//    vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(buildPipelineInfo.VertexDescription.VertexBindingDescriptions.size());
//    vertexInputInfo.pVertexBindingDescriptions = buildPipelineInfo.VertexDescription.VertexBindingDescriptions.data();
//    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(buildPipelineInfo.VertexDescription.VertexAttributeDescriptions.size());
//    vertexInputInfo.pVertexAttributeDescriptions = buildPipelineInfo.VertexDescription.VertexAttributeDescriptions.data();
//
//    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
//    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
//    inputAssembly.topology = buildPipelineInfo.VertexDescription.VertexTopology;
//    inputAssembly.primitiveRestartEnable = VK_FALSE;
//
//    VkPipelineViewportStateCreateInfo viewportState{};
//    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
//
//    VkPipelineRasterizationStateCreateInfo rasterizer{};
//    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
//    rasterizer.depthClampEnable = VK_FALSE;
//    rasterizer.rasterizerDiscardEnable = VK_FALSE;
//    rasterizer.lineWidth = 1.0f;
//    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
//    rasterizer.cullMode = buildPipelineInfo.VertexDescription.CullMode;
//    rasterizer.polygonMode = buildPipelineInfo.VertexDescription.PolygonMode;
//    rasterizer.depthBiasEnable = VK_FALSE;
//
//    VkPipelineMultisampleStateCreateInfo multisampling{};
//    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
//    multisampling.sampleShadingEnable = VK_TRUE;
//    multisampling.rasterizationSamples = buildPipelineInfo.RenderPassDescription.sampleCount;
//
//    VkPipelineColorBlendStateCreateInfo colorBlending{};
//    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
//    colorBlending.attachmentCount = static_cast<uint32_t>(buildPipelineInfo.RenderPassDescription.ColorAttachments.size());
//    colorBlending.pAttachments = buildPipelineInfo.RenderPassDescription.ColorAttachments.data();
//
//    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
//    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
//    pipelineLayoutInfo.setLayoutCount = 1;
//    pipelineLayoutInfo.pSetLayouts = &buildPipelineInfo.DescriptorSetLayout;
//    pipelineLayoutInfo.pushConstantRangeCount = 1;
//
//    if (buildPipelineInfo.RenderPassDescription.ConstBufferSize != 0)
//    {
//        VkPushConstantRange pushConstantRange{};
//        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
//        pushConstantRange.offset = 0;
//        pushConstantRange.size = buildPipelineInfo.RenderPassDescription.ConstBufferSize;
//        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
//    }
//
//    if (vkCreatePipelineLayout(VulkanRenderer::GetDevice(), &pipelineLayoutInfo, nullptr, &ShaderPipelineLayout) != VK_SUCCESS) {
//        throw std::runtime_error("Failed to create pipeline layout.");
//    }
//
//    VkGraphicsPipelineCreateInfo pipelineInfo{};
//    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
//    pipelineInfo.stageCount = static_cast<uint32_t>(buildPipelineInfo.RenderPassDescription.PipelineShaderStageList.size());
//    pipelineInfo.pStages = buildPipelineInfo.RenderPassDescription.PipelineShaderStageList.data();
//    pipelineInfo.pVertexInputState = &vertexInputInfo;
//    pipelineInfo.pInputAssemblyState = &inputAssembly;
//    pipelineInfo.pViewportState = &viewportState;
//    pipelineInfo.pRasterizationState = &rasterizer;
//    pipelineInfo.pMultisampleState = &multisampling;
//    pipelineInfo.pDepthStencilState = &buildPipelineInfo.RenderPassDescription.DepthStencilInfo;
//    pipelineInfo.pColorBlendState = &colorBlending;
//    pipelineInfo.layout = ShaderPipelineLayout;
//    pipelineInfo.renderPass = buildPipelineInfo.RenderPassDescription.renderPass;
//    pipelineInfo.subpass = 0;
//    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
//
//    VkResult result = vkCreateGraphicsPipelines(VulkanRenderer::GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &ShaderPipeline);
//    if (result != VK_SUCCESS) {
//        throw std::runtime_error("Failed to create pipeline.");
//    }
//}
//
//VkPipelineShaderStageCreateInfo GLTF_GraphicsPipeline::CreateShader(const std::string& filename, VkShaderStageFlagBits shaderStages)
//{
//    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
//    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//    vertShaderStageInfo.stage = shaderStages;
//    vertShaderStageInfo.module = ReadShaderFile(filename);
//    vertShaderStageInfo.pName = "main";
//
//    return vertShaderStageInfo;
//}
//
//void GLTF_GraphicsPipeline::UpdateGraphicsPipeLine()
//{
//    vkDestroyPipeline(VulkanRenderer::GetDevice(), ShaderPipeline, nullptr);
//    vkDestroyPipelineLayout(VulkanRenderer::GetDevice(), ShaderPipelineLayout, nullptr);
//
//    ShaderPipeline = VK_NULL_HANDLE;
//    ShaderPipelineLayout = VK_NULL_HANDLE;
//}
//
//void GLTF_GraphicsPipeline::UpdateGraphicsPipeLine(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo)
//{
//    vkDestroyPipeline(VulkanRenderer::GetDevice(), ShaderPipeline, nullptr);
//    vkDestroyPipelineLayout(VulkanRenderer::GetDevice(), ShaderPipelineLayout, nullptr);
//
//    ShaderPipeline = VK_NULL_HANDLE;
//    ShaderPipelineLayout = VK_NULL_HANDLE;
//
//    BuildShaderPipeLine(buildGraphicsPipelineInfo);
//}
//
//void GLTF_GraphicsPipeline::Destroy()
//{
//    vkDestroyPipeline(VulkanRenderer::GetDevice(), ShaderPipeline, nullptr);
//    vkDestroyPipelineLayout(VulkanRenderer::GetDevice(), ShaderPipelineLayout, nullptr);
//
//    ShaderPipeline = VK_NULL_HANDLE;
//    ShaderPipelineLayout = VK_NULL_HANDLE;
//}