#include "PBRRenderer.h"

PBRRenderer::PBRRenderer()
{
}

PBRRenderer::~PBRRenderer()
{
}

void PBRRenderer::BuildRenderer()
{
	SceneManager::environmentTexture = std::make_shared<EnvironmentTexture>("../texture/hdr/alps_field_4k.hdr", VK_FORMAT_R32G32B32A32_SFLOAT);

	environmentToCubeRenderPass.BuildRenderPass(4096.0f / 4);
	GLTF_BRDFRenderPass.BuildRenderPass(512);

	std::vector<std::shared_ptr<RenderedCubeMapTexture>> cubemap = { SceneManager::CubeMap };
	irradianceRenderPass.OneTimeDraw(cubemap, SceneManager::GetPreRenderedMapSize());
	SceneManager::IrradianceMap = irradianceRenderPass.IrradianceCubeMapList[0];
	prefilterRenderPass.OneTimeDraw(cubemap, SceneManager::GetPreRenderedMapSize());
	SceneManager::PrefilterMap = prefilterRenderPass.PrefilterCubeMapList[0];
	gLTFRenderPass.BuildRenderPass(model);

	model = GLTF_Temp_Model("C:/Users/dotha/Desktop/Vulkan-master/Vulkan/data/models/FlightHelmet/glTF/FlightHelmet.gltf", glm::mat4(1.0f), 0);

	irradianceRenderPass.OneTimeDraw(cubemap, SceneManager::GetPreRenderedMapSize());
	SceneManager::IrradianceMap = irradianceRenderPass.IrradianceCubeMapList[0];
	prefilterRenderPass.OneTimeDraw(cubemap, SceneManager::GetPreRenderedMapSize());
	SceneManager::PrefilterMap = prefilterRenderPass.PrefilterCubeMapList[0];
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
