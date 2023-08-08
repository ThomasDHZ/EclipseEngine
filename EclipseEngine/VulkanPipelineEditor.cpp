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
		case DescriptorBindingPropertiesEnum::kDirectionalShadowDescriptor: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; break;
		case DescriptorBindingPropertiesEnum::kPointShadowDescriptor: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; break;
		case DescriptorBindingPropertiesEnum::kSpotShadowDescriptor: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; break;
		case DescriptorBindingPropertiesEnum::kViewTextureDescriptor: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; break;
		case DescriptorBindingPropertiesEnum::kViewDepthTextureDescriptor: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; break;
		case DescriptorBindingPropertiesEnum::kCubeMapSamplerDescriptor: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; break;
		default: throw std::runtime_error("Unknown binding type.");
	}
}

void VulkanPipelineEditor::Update()
{
	ImGui::Begin("Render Pass Editor");
	ImGui::Separator();
	ImGui::InputText("Render Pass Name", RenderPassName, IM_ARRAYSIZE(RenderPassName));

	ImGui::Separator();
	if (ImGui::BeginCombo("Texture Resolution:", TextureResolutionSelection))
	{
		for (int n = 0; n < IM_ARRAYSIZE(TextureResolutionList); n++)
		{
			bool is_selected = (TextureResolutionSelection == TextureResolutionList[n]);
			if (ImGui::Selectable(TextureResolutionList[n], is_selected))
			{
				TextureResolutionSelection = TextureResolutionList[n];
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Color Texture Format:", ColorTextureFormatSelection))
	{
		for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::VkFormatEnumList); n++)
		{
			bool is_selected = (ColorTextureFormatSelection == VulkanMenu::VkFormatEnumList[n]);
			if (ImGui::Selectable(VulkanMenu::VkFormatEnumList[n], is_selected))
			{
				ColorTextureFormatSelection = VulkanMenu::VkFormatEnumList[n];
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Depth Texture Format:", DepthTextureFormatSelection))
	{
		for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::VkFormatEnumList); n++)
		{
			bool is_selected = (DepthTextureFormatSelection == VulkanMenu::VkFormatEnumList[n]);
			if (ImGui::Selectable(VulkanMenu::VkFormatEnumList[n], is_selected))
			{
				DepthTextureFormatSelection = VulkanMenu::VkFormatEnumList[n];
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Multi Sampling", MulitSamplerEnumSelection))
	{
		for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::VkSampleCountFlagBitsEnumList); n++)
		{
			bool is_selected = (MulitSamplerEnumSelection == VulkanMenu::VkSampleCountFlagBitsEnumList[n]);
			if (ImGui::Selectable(VulkanMenu::VkSampleCountFlagBitsEnumList[n], is_selected))
			{
				MulitSamplerEnumSelection = VulkanMenu::VkSampleCountFlagBitsEnumList[n];
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}
	ImGui::Separator();
	if(ImGui::InputInt("Number of MultiSampled Textures", &MultiSampledColorTextureCount));
	{
		if (MultiSampledColorTextureCount < 0)
		{
			MultiSampledColorTextureCount = 0;
		}
		if (MultiSampledColorTextureCount > AttachmentLimit)
		{
			MultiSampledColorTextureCount = AttachmentLimit;
		}
	}
	MultiSampledTextureMenuList.resize(MultiSampledColorTextureCount);
	for (int x = 0; x < MultiSampledTextureMenuList.size(); x++)
	{
		std::string name = ("MultiSampled Texture " + std::to_string(x));
		MultiSampledTextureMenuList[x].ImGuiUpdate(name, (AttachmentLimit * 1) + x);
	}

	if (ImGui::InputInt("Number of Color Textures", &ColorTextureCount));
	{
		if (ColorTextureCount < 0)
		{
			ColorTextureCount = 0;
		}
		if (ColorTextureCount > AttachmentLimit)
		{
			ColorTextureCount = AttachmentLimit;
		}
	}
	ColorTextureMenuList.resize(ColorTextureCount);
	for (int x = 0; x < ColorTextureMenuList.size(); x++)
	{
		std::string name = ("Color Texture " + std::to_string(x));
		ColorTextureMenuList[x].ImGuiUpdate(name, (AttachmentLimit * 2) + x);
	}

	if (ImGui::InputInt("Number of Output Color Textures", &OutputColorTextureCount))
	{
		if (OutputColorTextureCount < 0)
		{
			OutputColorTextureCount = 0;
		}
		if (OutputColorTextureCount > AttachmentLimit)
		{
			OutputColorTextureCount = AttachmentLimit;
		}
	}

	OutputColorTextureMenuList.resize(OutputColorTextureCount);
	for (int x = 0; x < OutputColorTextureMenuList.size(); x++)
	{
		std::string name = ("Output Color Texture " + std::to_string(x));
		OutputColorTextureMenuList[x].ImGuiUpdate(name, (AttachmentLimit * 3) + x);
	}

	std::string name = "Depth Texture";
	DepthtextureMenu.ImGuiUpdate(name, (AttachmentLimit * 2) + 1);
	ImGui::Checkbox("Use Output Depth Texture", &UseOutPutDepthTexture);
	if (UseOutPutDepthTexture)
	{
		name = "Output Depth Texture";
		OutputDepthTextureMenu.ImGuiUpdate(name, (AttachmentLimit * 2) + 2);
	}

	ImGui::Separator();

	ImGui::InputInt("Subpass Dependency", &SubpassDependencyCount);
	SubpassDependencyMenuList.resize(SubpassDependencyCount);
	for (int x = 0; x < SubpassDependencyMenuList.size(); x++)
	{
		std::string name = ("Subpass Dependency " + std::to_string(x));
		SubpassDependencyMenuList[x].ImGuiUpdate(name, (AttachmentLimit * 4) + x);
	}

	ImGui::InputInt("Pipeline Count", &AddPipelineCount);
	AddPipelineMenuList.resize(AddPipelineCount);
	for (int x = 0; x < AddPipelineMenuList.size(); x++)
	{
		std::string name = ("Pipeline Selecton " + std::to_string(x));
		AddPipelineMenuList[x].ImGuiUpdate(name, (AttachmentLimit * 6) + x);
	}


	if (ImGui::InputInt("Blending Attachments:", &AddColorBlendAttachmentStateCount));
	{
		if (AddColorBlendAttachmentStateCount < 0)
		{
			AddColorBlendAttachmentStateCount = 0;
		}
		if (AddColorBlendAttachmentStateCount > AttachmentLimit)
		{
			AddColorBlendAttachmentStateCount = AttachmentLimit;
		}
	}

	ColorBlendAttachmentMenuList.resize(AddColorBlendAttachmentStateCount);
	for (int x = 0; x < ColorBlendAttachmentMenuList.size(); x++)
	{
		std::string name = ("Color Texture " + std::to_string(x));
		ColorBlendAttachmentMenuList[x].ImGuiUpdate(name, (AttachmentLimit * 5) + x);
	}


	if (ImGui::Button("Save"))
	{
		SaveRenderPass();
	}

	ImGui::End();

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
	if (ImGui::BeginCombo("Pipeline Render Type", PipelineTypeSelection))
	{
		for (int n = 0; n < IM_ARRAYSIZE(PipelineTypeEnumList); n++)
		{
			bool is_selected = (PipelineTypeSelection == PipelineTypeEnumList[n]);
			if (ImGui::Selectable(PipelineTypeEnumList[n], is_selected))
			{
				PipelineTypeSelection = PipelineTypeEnumList[n];
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}
	

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
		for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::DepthCompareOpperationEnumList); n++)
		{
			bool is_selected = (DepthCompareOpperationsSelecton == VulkanMenu::DepthCompareOpperationEnumList[n]);
			if (ImGui::Selectable(VulkanMenu::DepthCompareOpperationEnumList[n], is_selected))
			{
				DepthCompareOpperationsSelecton = VulkanMenu::DepthCompareOpperationEnumList[n];
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
		for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::PrimitiveTopologyEnumList); n++)
		{
			bool is_selected = (TopologySelection == VulkanMenu::PrimitiveTopologyEnumList[n]);
			if (ImGui::Selectable(VulkanMenu::PrimitiveTopologyEnumList[n], is_selected))
			{
				TopologySelection = VulkanMenu::PrimitiveTopologyEnumList[n];
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
		for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::FrontFaceEnumList); n++)
		{
			bool is_selected = (FrontFace == VulkanMenu::FrontFaceEnumList[n]);
			if (ImGui::Selectable(VulkanMenu::FrontFaceEnumList[n], is_selected))
			{
				FrontFace = VulkanMenu::FrontFaceEnumList[n];
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
		for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::CullModeEnumList); n++)
		{
			bool is_selected = (CullMode == VulkanMenu::CullModeEnumList[n]);
			if (ImGui::Selectable(VulkanMenu::CullModeEnumList[n], is_selected))
			{
				CullMode = VulkanMenu::CullModeEnumList[n];
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
		for (int n = 0; n < IM_ARRAYSIZE(VulkanMenu::PolygonModeEnumList); n++)
		{
			bool is_selected = (PolygonMode == VulkanMenu::PolygonModeEnumList[n]);
			if (ImGui::Selectable(VulkanMenu::PolygonModeEnumList[n], is_selected))
			{
				PolygonMode = VulkanMenu::PolygonModeEnumList[n];
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

void VulkanPipelineEditor::SaveRenderPass()
{
	nlohmann::json json;

	for (int x = 0; x < MultiSampledTextureMenuList.size(); x++)
	{
		MultiSampledTextureMenuList[x].SaveTextureAttachmentDescription(json["MultiSampledTextureList"][x]);
	}
	for (int x = 0; x < ColorTextureMenuList.size(); x++)
	{
		ColorTextureMenuList[x].SaveTextureAttachmentDescription(json["ColorTextureList"][x]);
	}
	for (int x = 0; x < OutputColorTextureMenuList.size(); x++)
	{
		OutputColorTextureMenuList[x].SaveTextureAttachmentDescription(json["OutputColorTextureList"][x]);
	}

	std::string selection(TextureResolutionSelection);
	std::string delimiter = "x";

	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = selection.find(delimiter, pos_start)) != std::string::npos) {
		token = selection.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(selection.substr(pos_start));
	glm::vec2 textureResolution = glm::vec2(stoi(res[0]), stoi(res[1]));

	JsonConverter::to_json(json["textureResolution"], textureResolution);
	json["colorTextureFormat"] = VulkanMenu::VkFormatConverter::SelectionToEnum(ColorTextureFormatSelection);
	json["depthTextureFormat"] = VulkanMenu::VkFormatConverter::SelectionToEnum(DepthTextureFormatSelection);
	json["multiSampleCount"] = VulkanMenu::VkSampleCountFlagBitsConverter::SelectionToEnum(MulitSamplerEnumSelection);

	DepthtextureMenu.SaveTextureAttachmentDescription(json["DepthTexture"]);
	OutputDepthTextureMenu.SaveTextureAttachmentDescription(json["OutputDepthtexture"]);
	for (int x = 0; x < ColorBlendAttachmentMenuList.size(); x++)
	{
		ColorBlendAttachmentMenuList[x].SaveAddColorBlendAttachmentState(json["ColorBlendAttachment"][x]);
	}
	for (int x = 0; x < SubpassDependencyMenuList.size(); x++)
	{
		SubpassDependencyMenuList[x].SaveSubpassDependency(json["SubPassDependency"][x]);
	}
	for (int x = 0; x < AddPipelineMenuList.size(); x++)
	{
		AddPipelineMenuList[x].SavePipeline(json["Shader"][x]);
	}

	std::string renderPassName = PathConsts::RenderPassPath + RenderPassName;
	renderPassName.append(".txt");

	std::ofstream pipelineFile(renderPassName);
	pipelineFile << json;
	pipelineFile.close();
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

	JsonConverter::to_json(json["PipelineRenderType"], PipelineRenderTypeConverter::SelectionToEnum(PipelineTypeSelection));
	jsonPipeline.SavePipelineShaderStageCreateInfo(json["Shader"][0], vert, outputVert);
	jsonPipeline.SavePipelineShaderStageCreateInfo(json["Shader"][1], frag, outputFrag);

	for (int x = 0; x < BindingSelectionList.size(); x++)
	{
		jsonPipeline.SaveDescriptorBindingLayout(json["DescriptorBindingLayout"][x], GetBindingType(VulkanMenu::DescriptorBindingProperties::SelectionToEnum(BindingSelectionList[x])), VulkanMenu::DescriptorBindingProperties::SelectionToEnum(BindingSelectionList[x]));
	}

	VkPipelineDepthStencilStateCreateInfo DepthStencilStateCreateInfo{};
	DepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	DepthStencilStateCreateInfo.depthTestEnable = DepthTestEnableSelecton;
	DepthStencilStateCreateInfo.depthWriteEnable = DepthWriteEnableSelecton;
	DepthStencilStateCreateInfo.depthBoundsTestEnable = DepthBoundsTestEnableSelecton;
	DepthStencilStateCreateInfo.stencilTestEnable = StencilTestEnableSelecton;
	DepthStencilStateCreateInfo.depthCompareOp = VulkanMenu::CompareOpperation::SelectionToEnum(DepthCompareOpperationsSelecton);
	jsonPipeline.SavePipelineDepthStencilStateCreateInfo(json["PipelineDepthStencilStateCreateInfo"], DepthStencilStateCreateInfo);

	VkPipelineInputAssemblyStateCreateInfo saveinputAssembly = {};
	saveinputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	saveinputAssembly.topology = VulkanMenu::PrimitiveTopology::SelectionToEnum(TopologySelection);
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
	saverasterizer.frontFace = VulkanMenu::FrontFace::SelectionToEnum(FrontFace);
	saverasterizer.cullMode = VulkanMenu::CullMode::SelectionToEnum(CullMode);
	saverasterizer.polygonMode = VulkanMenu::PolygonMode::SelectionToEnum(PolygonMode);
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
