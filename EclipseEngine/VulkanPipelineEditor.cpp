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

VkDescriptorType VulkanPipelineEditor::GetBindingType(DescriptorBindingPropertiesEnum bindingType)
{
	switch (bindingType)
	{
		case DescriptorBindingPropertiesEnum::kModelTransformDescriptor: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER; break;
		case DescriptorBindingPropertiesEnum::kMeshPropertiesDescriptor: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER; break;
		case DescriptorBindingPropertiesEnum::kTextureDescriptor: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; break;
		case DescriptorBindingPropertiesEnum::kMaterialDescriptor: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER; break;
		case DescriptorBindingPropertiesEnum::kBRDFMapDescriptor: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; break;
		case DescriptorBindingPropertiesEnum::kIrradianceMapDescriptor: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; break;
		case DescriptorBindingPropertiesEnum::kPrefilterMapDescriptor: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; break;
		case DescriptorBindingPropertiesEnum::kCubeMapDescriptor: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; break;
		case DescriptorBindingPropertiesEnum::kEnvironmentDescriptor: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; break;
		case DescriptorBindingPropertiesEnum::kSunLightDescriptor: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER; break;
		case DescriptorBindingPropertiesEnum::kDirectionalLightDescriptor: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER; break;
		case DescriptorBindingPropertiesEnum::kPointLightDescriptor: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER; break;
		case DescriptorBindingPropertiesEnum::kSpotLightDescriptor: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER; break;
		case DescriptorBindingPropertiesEnum::kReflectionViewDescriptor: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; break;
		default: throw std::runtime_error("Unknown binding type.");
	}
}

void VulkanPipelineEditor::Update()
{
	ImGui::Begin("Pipeline List");
	
	std::vector<std::string> pipelineListcharstring;
	std::vector<const char *> pipelineListchar;
	pipelineListchar.emplace_back("Default");

	for (const auto& file : std::filesystem::directory_iterator(PathConsts::PipelinePath))
	{
		auto splitFile = file.path().string().substr(PathConsts::PipelinePath.size());
		pipelineListcharstring.emplace_back(splitFile);
	}
	for (auto& list : pipelineListcharstring)
	{
		pipelineListchar.emplace_back(list.c_str());
	}

	int z = 0;
	if (ImGui::ListBox("PipelineSelection", &z, pipelineListchar.data(), pipelineListchar.size()))
	{
		LoadPipeline(pipelineListcharstring[z]);
	}

	ImGui::End();

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

	//int ColorBlendAttachment = 0;
	//bool					   blendEnable;
	//BlendFactorMode            srcColorBlendFactor;
	//BlendFactorMode            dstColorBlendFactor;
	//BlendOpMode                colorBlendOp;
	//BlendFactorMode            srcAlphaBlendFactor;
	//BlendFactorMode            dstAlphaBlendFactor;
	//BlendOpMode                alphaBlendOp;
	//VkColorComponentFlags    colorWriteMask;

	//ImGui::InputInt("Number of Bindings", &BindingDescriptorNum);
	//BindingSelectionList.resize(BindingDescriptorNum);
	//for (int x = 0; x < BindingSelectionList.size(); x++)
	//{
	//	ImGui::Separator();

	//	if (ImGui::BeginCombo(("Binding " + std::to_string(x)).c_str(), BindingSelectionList[x]))
	//	{
	//		for (int n = 0; n < IM_ARRAYSIZE(PipelineDescriptorList); n++)
	//		{
	//			bool is_selected = (BindingSelectionList[x] == PipelineDescriptorList[n]);
	//			if (ImGui::Selectable(PipelineDescriptorList[n], is_selected))
	//			{
	//				BindingSelectionList[x] = PipelineDescriptorList[n];
	//				if (is_selected)
	//				{
	//					ImGui::SetItemDefaultFocus();
	//				}
	//			}
	//		}
	//		ImGui::EndCombo();
	//	}

	//	ImGui::Separator();
	//}


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

void VulkanPipelineEditor::LoadPipeline(std::string& pipelineFile)
{
	/*if (ShaderPipeline != VK_NULL_HANDLE)
	{
		Destroy();
	}

	std::string SceneInfo;
	std::ifstream SceneFile;
	SceneFile.open(PathConsts::PipelinePath + filePath);
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
	}*/
}

void VulkanPipelineEditor::BuildPipeline()
{
}

void VulkanPipelineEditor::SavePipeline()
{
	nlohmann::json json;
	JsonGraphicsPipeline jsonPipeline{};

	std::string vertString = PathConsts::ShaderPath + ShaderName + "Vert.spv";
	std::string fragString = PathConsts::ShaderPath + ShaderName + "Frag.spv";

	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	auto vert = CreateShader(vertString, VK_SHADER_STAGE_VERTEX_BIT);
	auto frag = CreateShader(fragString, VK_SHADER_STAGE_FRAGMENT_BIT);

	std::string outputVert = PathConsts::ShaderPath + ShaderName + "Vert.spv";
	std::string outputFrag = PathConsts::ShaderPath + ShaderName + "Frag.spv";

	jsonPipeline.SavePipelineShaderStageCreateInfo(json["Shader"][0], vert, outputVert);
	jsonPipeline.SavePipelineShaderStageCreateInfo(json["Shader"][1], frag, outputFrag);

	for (int x = 0; x < BindingSelectionList.size(); x++)
	{
		jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][x], GetBindingType(DescriptorBindingProperties::SelectionToEnum(BindingSelectionList[x])), DescriptorBindingProperties::SelectionToEnum(BindingSelectionList[x]));
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
