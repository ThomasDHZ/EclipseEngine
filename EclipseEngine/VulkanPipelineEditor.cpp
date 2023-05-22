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
		default: throw std::runtime_error("Unkown binding type.");
	}
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

void VulkanPipelineEditor::LoadPipeline()
{
	//std::string SceneInfo;
	//std::ifstream SceneFile;
	//SceneFile.open(BasePipelineFilePath + filePath);
	//if (SceneFile.is_open())
	//{
	//	while (!SceneFile.eof())
	//	{
	//		getline(SceneFile, SceneInfo);
	//	}
	//}
	//else std::cout << "Unable to open file";
	//SceneFile.close();

	//nlohmann::json json = nlohmann::json::parse(SceneInfo);

	//std::vector<DescriptorBindingPropertiesEnum> BindingList;
	//std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	//for (int x = 0; x < json["Shader"].size(); x++)
	//{
	//	PipelineShaderStageList.emplace_back(LoadPipelineShaderStageCreateInfo(json["Shader"][x]));

	//	VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo{};
	//	PipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	//	PipelineShaderStageCreateInfo.stage = json["Shader"]["stage"];
	//	PipelineShaderStageCreateInfo.module = ReadShaderFile(json["Shader"]["shaderFile"]);
	//	PipelineShaderStageCreateInfo.pName = "main";
	//	PipelineShaderStageCreateInfo.pSpecializationInfo = nullptr;
	//	PipelineShaderStageCreateInfo.pNext = nullptr;
	//	PipelineShaderStageList.emplace_back(PipelineShaderStageCreateInfo);
	//}

	//for (int x = 0; x < json["DescriptorBindingLayout"].size(); x++)
	//{
	//	BindingList.emplace_back(json["DescriptorBindingLayout"][x]["bindingType"]);
	//}

	//DepthTestEnableSelecton = json["PipelineDepthStencilStateCreateInfo"]["depthTestEnable"];
	//DepthWriteEnableSelecton = json["PipelineDepthStencilStateCreateInfo"]["depthWriteEnable"];
	//DepthBoundsTestEnableSelecton = json["PipelineDepthStencilStateCreateInfo"]["depthBoundsTestEnable"];
	//StencilTestEnableSelecton = json["PipelineDepthStencilStateCreateInfo"]["stencilTestEnable"];
 //   DepthCompareOpperationsSelecton = json["PipelineDepthStencilStateCreateInfo"]["depthCompareOp"];

	//VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo{};
	//pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	//pipelineInputAssemblyStateCreateInfo.pNext = nullptr;
	//pipelineInputAssemblyStateCreateInfo.flags = json["PipelineInputAssemblyStateCreateInfo"]["flags"];
	//pipelineInputAssemblyStateCreateInfo.topology = json["PipelineInputAssemblyStateCreateInfo"]["topology"];
	//pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = json["PipelineInputAssemblyStateCreateInfo"]["primitiveRestartEnable"];

	//VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo{};
	//pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	//pipelineViewportStateCreateInfo.pNext = nullptr;
	//pipelineViewportStateCreateInfo.flags = json["PipelineViewportStateCreateInfo"]["flags"];
	//pipelineViewportStateCreateInfo.viewportCount = json["PipelineViewportStateCreateInfo"]["viewportCount"];
	//pipelineViewportStateCreateInfo.pViewports = nullptr;
	//pipelineViewportStateCreateInfo.viewportCount = json["PipelineViewportStateCreateInfo"]["viewportCount"];
	//pipelineViewportStateCreateInfo.pViewports = nullptr;
	//pipelineViewportStateCreateInfo.pScissors = nullptr;

	//VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo{};
	//pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	//pipelineRasterizationStateCreateInfo.pNext = nullptr;
	//pipelineRasterizationStateCreateInfo.depthClampEnable = json["PipelineRasterizationStateCreateInfo"]["depthClampEnable"];
	//pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = json["PipelineRasterizationStateCreateInfo"]["rasterizerDiscardEnable"];
	//pipelineRasterizationStateCreateInfo.polygonMode = json["PipelineRasterizationStateCreateInfo"]["polygonMode"];
	//pipelineRasterizationStateCreateInfo.cullMode = json["PipelineRasterizationStateCreateInfo"]["cullMode"];
	//pipelineRasterizationStateCreateInfo.frontFace = json["PipelineRasterizationStateCreateInfo"]["frontFace"];
	//pipelineRasterizationStateCreateInfo.depthBiasEnable = json["PipelineRasterizationStateCreateInfo"]["depthBiasEnable"];
	//pipelineRasterizationStateCreateInfo.depthBiasConstantFactor = json["PipelineRasterizationStateCreateInfo"]["depthBiasConstantFactor"];
	//pipelineRasterizationStateCreateInfo.depthBiasClamp = json["PipelineRasterizationStateCreateInfo"]["depthBiasClamp"];
	//pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = json["PipelineRasterizationStateCreateInfo"]["depthBiasSlopeFactor"];
	//pipelineRasterizationStateCreateInfo.lineWidth = json["PipelineRasterizationStateCreateInfo"]["lineWidth"];
}

void VulkanPipelineEditor::BuildPipeline()
{
}

void VulkanPipelineEditor::SavePipeline()
{
	nlohmann::json json;
	JsonGraphicsPipeline jsonPipeline{};

	std::string vertString = "../Shaders/";
	vertString.append(ShaderName);
	vertString.append("Vert.spv");

	std::string fragString = "../Shaders/";
	fragString.append(ShaderName);
	fragString.append("Frag.spv");

	std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageList;
	auto vert = CreateShader(vertString, VK_SHADER_STAGE_VERTEX_BIT);
	auto frag = CreateShader(fragString, VK_SHADER_STAGE_FRAGMENT_BIT);

	std::string outputVert = "../Shaders/";
	outputVert.append(ShaderName);
	outputVert.append("Vert.spv");

	std::string outputFrag = "../Shaders/";
	outputFrag.append(ShaderName);
	outputFrag.append("Frag.spv");

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
