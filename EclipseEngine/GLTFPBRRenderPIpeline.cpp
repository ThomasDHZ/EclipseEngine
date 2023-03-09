#include "GLTFPBRRenderPIpeline.h"
#include "SceneManager.h"

GLTFPBRRenderPIpeline::GLTFPBRRenderPIpeline()
{
}

GLTFPBRRenderPIpeline::~GLTFPBRRenderPIpeline()
{
}

void GLTFPBRRenderPIpeline::InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, GLTF_Temp_Model model)
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
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "GLTFPBRRendererVert.spv", VK_SHADER_STAGE_VERTEX_BIT));
    PipelineShaderStageList.emplace_back(CreateShader(BaseShaderFilePath + "GLTFPBRRendererFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT));

    std::vector<DescriptorSetBindingStruct> DescriptorBindingList;
    GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 0, model.GetMeshPropertiesBuffer());
    GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 1, model.GetTransformMatrixBuffer());
    GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 2, model.MaterialList[0]->GetAlbedoMapDescriptor());
    GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 3, model.MaterialList[0]->GetNormalMapDescriptor());
    GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 4, model.MaterialList[0]->GetMetallicRoughnessMapDescriptor());
    GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 5, model.MaterialList[0]->GetAmbientOcclusionMapDescriptor());
    GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 6, model.MaterialList[0]->GetAlphaMapDescriptor());
    GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 7, model.MaterialList[0]->GetDepthMapDescriptor());
    GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 8, SceneManager::GetBRDFMapDescriptor());
    GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 9, SceneManager::GetIrradianceMapDescriptor());
    GLTF_GraphicsDescriptors::AddTextureDescriptorSetBinding(DescriptorBindingList, 10, SceneManager::GetPrefilterMapDescriptor());
    GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 11, model.GetSunLightPropertiesBuffer());
    GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 12, model.GetDirectionalLightPropertiesBuffer());
    GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 13, model.GetPointLightPropertiesBuffer());
    GLTF_GraphicsDescriptors::AddStorageBufferDescriptorSetBinding(DescriptorBindingList, 14, model.GetSpotLightPropertiesBuffer());
    DescriptorSetLayout = GLTF_GraphicsDescriptors::SubmitDescriptorSetLayout(DescriptorBindingList);

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
    RenderPassInfo.DescriptorBindingList = DescriptorBindingList;
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

void GLTFPBRRenderPIpeline::Draw(VkCommandBuffer& commandBuffer, GLTF_Temp_Model model)
{
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ShaderPipeline);
    model.Draw(commandBuffer, ShaderPipelineLayout);
}
