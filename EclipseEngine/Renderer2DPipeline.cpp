#include "Renderer2DPipeline.h"
#include "Vertex.h"

Renderer2DPipeline::Renderer2DPipeline() : GraphicsPipeline()
{
}

Renderer2DPipeline::Renderer2DPipeline(const VkRenderPass& renderPass, GameObject obj) : GraphicsPipeline()
{
    SetUpDescriptorBindings(obj);
    SetUpShaderPipeLine(renderPass);
}

Renderer2DPipeline::~Renderer2DPipeline()
{
}

void Renderer2DPipeline::SetUpDescriptorBindings(GameObject obj)
{
    VkBuffer buffer = VK_NULL_HANDLE;
    auto spriteRenderer = obj.GetComponentByType(ComponentType::kSpriteRenderer);
    auto transform2D = obj.GetComponentByType(ComponentType::kTransform2D);
    if (spriteRenderer != nullptr &&
        transform2D != nullptr)
    {
        SpriteRenderer* sprite = static_cast<SpriteRenderer*>(spriteRenderer.get());
        Transform2D* transform = static_cast<Transform2D*>(transform2D.get());

        MeshProperties2 meshProps = {};
        meshProps.MeshTransform = transform->Transform;
        sprite->MeshProperties.Update(meshProps);
        buffer = sprite->MeshProperties.GetVulkanBufferData().GetBuffer();
    }

    std::vector<VkDescriptorBufferInfo> MeshPropertiesmBufferList{};
    VkDescriptorBufferInfo MeshPropertiesmBufferBufferInfo = {};
    MeshPropertiesmBufferBufferInfo.buffer = buffer;
    MeshPropertiesmBufferBufferInfo.offset = 0;
    MeshPropertiesmBufferBufferInfo.range = VK_WHOLE_SIZE;
    MeshPropertiesmBufferList.emplace_back(MeshPropertiesmBufferBufferInfo);

    std::vector<VkDescriptorBufferInfo> MeshPropertyDataBufferInfo = MeshPropertiesmBufferList;
    AddStorageBufferDescriptorSetBinding(0, MeshPropertyDataBufferInfo, 1);

    SubmitDescriptorSet();
}

void Renderer2DPipeline::SetUpShaderPipeLine(const VkRenderPass& renderPass)
{
    std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
    PipelineShaderStageList.emplace_back(CreateShader("Shaders/Renderer2DVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
    PipelineShaderStageList.emplace_back(CreateShader("Shaders/Renderer2DFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)VulkanRenderer::GetSwapChainResolution().width;
    viewport.height = (float)VulkanRenderer::GetSwapChainResolution().height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = VulkanRenderer::GetSwapChainResolution();

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_NONE;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_TRUE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;

    std::array<VkPipelineColorBlendAttachmentState, 1> ColorAttachment = {};
    ColorAttachment[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    ColorAttachment[0].blendEnable = VK_TRUE;
    ColorAttachment[0].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    ColorAttachment[0].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    ColorAttachment[0].colorBlendOp = VK_BLEND_OP_ADD;
    ColorAttachment[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    ColorAttachment[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    ColorAttachment[0].alphaBlendOp = VK_BLEND_OP_SUBTRACT;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = static_cast<uint32_t>(ColorAttachment.size());
    colorBlending.pAttachments = ColorAttachment.data();
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SceneProperties);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &DescriptorSetLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

    if (vkCreatePipelineLayout(VulkanRenderer::GetDevice(), &pipelineLayoutInfo, nullptr, &ShaderPipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create gbuffer pipeline layout.");
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
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = ShaderPipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(VulkanRenderer::GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &ShaderPipeline) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create gbuffer pipeline.");
    }

    for (auto& shader : PipelineShaderStageList)
    {
        vkDestroyShaderModule(VulkanRenderer::GetDevice(), shader.module, nullptr);
    }
}

void Renderer2DPipeline::UpdateGraphicsPipeLine(const VkRenderPass& renderPass, GameObject obj)
{
    GraphicsPipeline::UpdateGraphicsPipeLine();
    SetUpDescriptorBindings(obj);
    SetUpShaderPipeLine(renderPass);
}
