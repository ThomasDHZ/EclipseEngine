#include "VulkanPipelineEditor.h"

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

	ImGui::InputText("Vertex Shader", VertexShaderLocation, IM_ARRAYSIZE(VertexShaderLocation));
	ImGui::InputText("Pixel Shader", PixelShaderLocation, IM_ARRAYSIZE(PixelShaderLocation));

	ImGui::Separator();
	ImGui::Text("Descriptor Sets:");

	ImGui::InputInt("Number of Bindings", &BindingDescriptorNum);
	BindingSelectionList.resize(BindingDescriptorNum);
	for (int x = 0; x < BindingSelectionList.size(); x++)
	{
		if (ImGui::BeginCombo(("Binding " + std::to_string(x)).c_str(), BindingSelectionList[x])) // The second parameter is the label previewed before opening the combo.
		{
			for (int n = 0; n < IM_ARRAYSIZE(PipelineDescriptorList); n++)
			{
				bool is_selected = (BindingSelectionList[x] == PipelineDescriptorList[n]); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(PipelineDescriptorList[n], is_selected))
				{
					BindingSelectionList[x] = PipelineDescriptorList[n];
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
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
	ImGui::ListBox("Depth Compare Opperation", &DepthCompareOpperationsSelecton, DepthCompareOpperations, 8);

	ImGui::Separator();
	ImGui::Text("Input Assembly State Create Info:");

	ImGui::Checkbox("Primitive Restart", &PrimitiveRestartEnableSelection);
	ImGui::ListBox("Topology", &TopologySelection, PrimitiveTopologySelection, 9);

	ImGui::Separator();
	ImGui::Text("Viewport State Create Info:");

	VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo{};
	//pipelineViewportStateCreateInfo.flags
	//pipelineViewportStateCreateInfo.

	ImGui::Separator();
	ImGui::Text("Rasterization State Create Info:");

	VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo{};

	ImGui::Separator();
	ImGui::Text("Multisampling State Create Info:");
	VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo{};

	ImGui::Separator();
	ImGui::Text("Color Blending State Create Info:");
	VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo{};
	ImGui::End();
}