#include "VulkanPipelineEditor.h"

//const char* VulkanPipelineEditor::GetPipelineDescriptorEnumText(DescriptorBindingPropertiesEnum descriptorBindingEnum)
//{
//	switch (descriptorBindingEnum)
//	{
//		case DescriptorBindingPropertiesEnum::kModelTransformDescriptor: return "kModelTransformDescriptor"; break;
//		case DescriptorBindingPropertiesEnum::kMeshPropertiesDescriptor: return "kMeshPropertiesDescriptor"; break;
//		case DescriptorBindingPropertiesEnum::kTextureDescriptor: return "kTextureDescriptor"; break;
//		case DescriptorBindingPropertiesEnum::kMaterialDescriptor: return "kMaterialDescriptor"; break;
//		case DescriptorBindingPropertiesEnum::kBRDFMapDescriptor: return "kBRDFMapDescriptor"; break;
//		case DescriptorBindingPropertiesEnum::kIrradianceMapDescriptor: return "kIrradianceMapDescriptor"; break;
//		case DescriptorBindingPropertiesEnum::kPrefilterMapDescriptor: return "kPrefilterMapDescriptor"; break;
//		case DescriptorBindingPropertiesEnum::kCubeMapDescriptor: return "kCubeMapDescriptor"; break;
//		case DescriptorBindingPropertiesEnum::kEnvironmentDescriptor: return "kEnvironmentDescriptor"; break;
//		case DescriptorBindingPropertiesEnum::kSunLightDescriptor: return "kSunLightDescriptor"; break;
//		case DescriptorBindingPropertiesEnum::kDirectionalLightDescriptor: return "kDirectionalLightDescriptor"; break;
//		case DescriptorBindingPropertiesEnum::kPointLightDescriptor: return "kPointLightDescriptor"; break;
//		case DescriptorBindingPropertiesEnum::kSpotLightDescriptor: return "kSpotLightDescriptor"; break;
//	}
//}

VulkanPipelineEditor::VulkanPipelineEditor()
{

}

VulkanPipelineEditor::~VulkanPipelineEditor()
{
}

void VulkanPipelineEditor::Update()
{
	ImGui::Begin("Pipeline Editor");
	//ImGui::BeginMenuBar();
	//if (ImGui::BeginMenu("File"))
	//{
	//	ImGui::MenuItem("Exit");
	//	ImGui::EndMenu();
	//}
	//ImGui::EndMainMenuBar();

	ImGui::Separator();
	ImGui::Text("Shader Locations:");

	ImGui::InputText("Shader Name", ShaderName, IM_ARRAYSIZE(ShaderName));

	ImGui::Separator();
	ImGui::Text("Descriptor Sets:");

	ImGui::InputInt("Number of Bindings", &BindingDescriptorNum);
	BindingSelectionList.resize(BindingDescriptorNum);
	for (int x = 0; x < BindingSelectionList.size(); x++)
	{
		if (ImGui::BeginCombo(("Binding " + std::to_string(x)).c_str(), BindingSelectionList[x]))
		{
			for (int n = 0; n < IM_ARRAYSIZE(PipelineDescriptorList); n++)
			{
				bool is_selected = (BindingSelectionList[x] == PipelineDescriptorList[n]);
				if (ImGui::Selectable(PipelineDescriptorList[n], is_selected))
				{
					BindingSelectionList[x] = PipelineDescriptorList[n];
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
			}
			ImGui::EndCombo();
		}
	}


	ImGui::Separator();
	ImGui::Text("Depth Stencil State Create Info:");

	ImGui::Checkbox("Depth Test", &DepthTestEnableSelecton);
	ImGui::Checkbox("Depth Write", &DepthWriteEnableSelecton);
	ImGui::Checkbox("Depth Bounds Test", &DepthBoundsTestEnableSelecton);
	ImGui::Checkbox("Stencil Test", &StencilTestEnableSelecton);
	if (ImGui::BeginCombo("Depth Compare Opperation", DepthCompareOpperationsSelecton))
	{
		for (int n = 0; n < IM_ARRAYSIZE(DepthCompareOpperationEnumList); n++)
		{
			bool is_selected = (DepthCompareOpperationsSelecton == DepthCompareOpperationEnumList[n]);
			if (ImGui::Selectable(DepthCompareOpperationEnumList[n], is_selected))
			{
				DepthCompareOpperationsSelecton = DepthCompareOpperationEnumList[n];
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Separator();
	ImGui::Text("Input Assembly State Create Info:");

	ImGui::Checkbox("Primitive Restart", &PrimitiveRestartEnableSelection);
	if (ImGui::BeginCombo("Topology", TopologySelection))
	{
		for (int n = 0; n < IM_ARRAYSIZE(PrimitiveTopologyEnumList); n++)
		{
			bool is_selected = (TopologySelection == PrimitiveTopologyEnumList[n]);
			if (ImGui::Selectable(PrimitiveTopologyEnumList[n], is_selected))
			{
				TopologySelection = PrimitiveTopologyEnumList[n];
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Separator();
	//ImGui::Text("Viewport State Create Info:");

	//ImGui::InputInt("Scissor Count", &ScissorCount);
	//ImGui::InputInt("ViewportCount", &ViewportCount);

	//ImGui::Separator();
	ImGui::Text("Rasterization State Create Info:");

	ImGui::Checkbox("Depth Clamp", &DepthClampEnable);
	ImGui::Checkbox("Depth Bias", &DepthBiasEnable);
	ImGui::Checkbox("Rasterizer Discard", &RasterizerDiscardEnable);
	ImGui::InputFloat("RasterizerDiscardEnable", &LineWidth);
	if (ImGui::BeginCombo("Front Face Type", FrontFace))
	{
		for (int n = 0; n < IM_ARRAYSIZE(FrontFaceEnumList); n++)
		{
			bool is_selected = (FrontFace == FrontFaceEnumList[n]);
			if (ImGui::Selectable(FrontFaceEnumList[n], is_selected))
			{
				FrontFace = FrontFaceEnumList[n];
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}
	if (ImGui::BeginCombo("Cull Mode", CullMode))
	{
		for (int n = 0; n < IM_ARRAYSIZE(CullModeEnumList); n++)
		{
			bool is_selected = (CullMode == CullModeEnumList[n]);
			if (ImGui::Selectable(CullModeEnumList[n], is_selected))
			{
				CullMode = CullModeEnumList[n];
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}
	if (ImGui::BeginCombo("Polygon Type", PolygonMode))
	{
		for (int n = 0; n < IM_ARRAYSIZE(PolygonModeEnumList); n++)
		{
			bool is_selected = (PolygonMode == PolygonModeEnumList[n]);
			if (ImGui::Selectable(PolygonModeEnumList[n], is_selected))
			{
				PolygonMode = PolygonModeEnumList[n];
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Separator();
	if (ImGui::Button("Save"))
	{
		SavePipeline();
	}
	ImGui::End();
}

void VulkanPipelineEditor::SavePipeline()
{
	nlohmann::json json;
	JsonGraphicsPipeline jsonPipeline{};

	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	auto a = CreateShader("../Shaders/PBRInstanceRendererVert.spv", VK_SHADER_STAGE_VERTEX_BIT);
	auto b = CreateShader("../Shaders/PBRInstanceRendererFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

	std::string outputVert = "../Shaders/";
	outputVert.append(ShaderName);
	outputVert.append("Vert.spv");

	std::string outputFrag = "../Shaders/";
	outputFrag.append(ShaderName);
	outputFrag.append("Frag.spv");

	jsonPipeline.SavePipelineShaderStageCreateInfo(json["Shader"][0], a, outputVert);
	jsonPipeline.SavePipelineShaderStageCreateInfo(json["Shader"][1], b, outputFrag);

	for (int x = 0; x < json["Shader"].size(); x++)
	{
		PipelineShaderStageList.emplace_back(jsonPipeline.LoadPipelineShaderStageCreateInfo(json["Shader"][x]));
	}

	for (int x = 0; x < BindingSelectionList.size(); x++)
	{
		jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][x], VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, DescriptorBindingProperties::SelectionToEnum(BindingSelectionList[x]));
	}

	VkPipelineDepthStencilStateCreateInfo DepthStencilStateCreateInfo{};
	DepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	DepthStencilStateCreateInfo.depthTestEnable = DepthTestEnableSelecton;
	DepthStencilStateCreateInfo.depthWriteEnable = DepthWriteEnableSelecton;
	DepthStencilStateCreateInfo.depthBoundsTestEnable = DepthBoundsTestEnableSelecton;
	DepthStencilStateCreateInfo.stencilTestEnable = StencilTestEnableSelecton;
	DepthStencilStateCreateInfo.depthCompareOp = CompareOpperation::SelectionToEnum(DepthCompareOpperationsSelecton);
	jsonPipeline.SavePipelineDepthStencilStateCreateInfo(json["PipelineDepthStencilStateCreateInfo"], DepthStencilStateCreateInfo);

	VkPipelineInputAssemblyStateCreateInfo saveinputAssembly = {};
	saveinputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	saveinputAssembly.topology = PrimitiveTopology::SelectionToEnum(TopologySelection);
	saveinputAssembly.primitiveRestartEnable = PrimitiveRestartEnableSelection;
	jsonPipeline.SavePipelineInputAssemblyStateCreateInfo(json["PipelineInputAssemblyStateCreateInfo"], saveinputAssembly);

	VkPipelineViewportStateCreateInfo saveviewportState{};
	saveviewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	saveviewportState.scissorCount = 0;
	saveviewportState.viewportCount = 0;
	jsonPipeline.SavePipelineViewportStateCreateInfo(json["PipelineViewportStateCreateInfo"], saveviewportState);

	VkPipelineRasterizationStateCreateInfo saverasterizer{};
	saverasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	saverasterizer.depthClampEnable = DepthClampEnable;
	saverasterizer.depthBiasEnable = DepthBiasEnable;
	saverasterizer.rasterizerDiscardEnable = RasterizerDiscardEnable;
	saverasterizer.lineWidth = LineWidth;
	saverasterizer.frontFace = FrontFace::SelectionToEnum(FrontFace);
	saverasterizer.cullMode = CullMode::SelectionToEnum(CullMode);
	saverasterizer.polygonMode = PolygonMode::SelectionToEnum(PolygonMode);
	jsonPipeline.SavePipelineRasterizationStateCreateInfo(json["PipelineRasterizationStateCreateInfo"], saverasterizer);

	VkPipelineMultisampleStateCreateInfo savemultisampling{};
	savemultisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	savemultisampling.sampleShadingEnable = VK_TRUE;
	savemultisampling.rasterizationSamples = GraphicsDevice::GetMaxSampleCount();
	jsonPipeline.SavePipelineMultisampleStateCreateInfo(json["PipelineMultisampleStateCreateInfo"], savemultisampling);

	std::vector<VkPipelineColorBlendAttachmentState> ColorAttachmentList;

	VkPipelineColorBlendAttachmentState ColorAttachment{};
	ColorAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	ColorAttachment.blendEnable = VK_TRUE;
	ColorAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	ColorAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	ColorAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	ColorAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	ColorAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	ColorAttachment.alphaBlendOp = VK_BLEND_OP_SUBTRACT;
	ColorAttachmentList.emplace_back(ColorAttachment);

	VkPipelineColorBlendStateCreateInfo savecolorBlending{};
	savecolorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	savecolorBlending.attachmentCount = static_cast<uint32_t>(ColorAttachmentList.size());
	savecolorBlending.pAttachments = ColorAttachmentList.data();
	jsonPipeline.SavePipelineColorBlendStateCreateInfo(json["PipelineColorBlendStateCreateInfo"], savecolorBlending);

	std::string shader = ShaderName;
	shader.append(".txt");
	jsonPipeline.SaveGraphicsPipeline(shader.c_str(), json);
}
