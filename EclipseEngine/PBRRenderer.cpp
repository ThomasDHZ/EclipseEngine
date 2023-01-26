#include "PBRRenderer.h"

PBRRenderer::PBRRenderer()
{
}

PBRRenderer::~PBRRenderer()
{
}

void PBRRenderer::BuildRenderer()
{
	model = GLTF_Temp_Model("C:/Users/dotha/Desktop/Vulkan-master/Vulkan/data/models/FlightHelmet/glTF/FlightHelmet.gltf", glm::mat4(1.0f), 0);
	gLTFRenderPass.BuildRenderPass(model);
	frameBufferRenderPass.BuildRenderPass(gLTFRenderPass.RenderedTexture, gLTFRenderPass.RenderedTexture);
}

void PBRRenderer::Update()
{
	model.Update(glm::mat4(1.0f));
}

void PBRRenderer::ImGuiUpdate()
{
	if (SceneManager::EditorModeFlag)
	{
		if (ImGui::Button("Play Mode"))
		{
			SceneManager::EditorModeFlag = false;
		}
		if (ImGui::Button("Update Renderer"))
		{
			UpdateRenderer = true;
		}
		if (ImGui::Button("Bake"))
		{
			//BakeTextures("TestBake.bmp");
		}
	}
	else
	{
		if (ImGui::Button("Editor Mode"))
		{
			SceneManager::EditorModeFlag = true;
		}
	}
}

void PBRRenderer::Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	CommandBufferSubmitList.emplace_back(gLTFRenderPass.Draw(model));
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
}

void PBRRenderer::Destroy()
{
	frameBufferRenderPass.Destroy();
}
