#include "GLTFRenderer.h"

GLTFRenderer::GLTFRenderer()
{
}

GLTFRenderer::~GLTFRenderer()
{
}

void GLTFRenderer::BuildRenderer()
{
	SceneManager::sceneProperites.PBRMaxMipLevel = static_cast<uint32_t>(std::floor(std::log2(std::max(SceneManager::GetPreRenderedMapSize(), SceneManager::GetPreRenderedMapSize())))) + 1;
	SceneManager::environmentTexture = std::make_shared<EnvironmentTexture>("../texture/hdr/newport_loft.hdr", VK_FORMAT_R32G32B32A32_SFLOAT);

	environmentToCubeRenderPass.BuildRenderPass(4096.0f / 4);
	GLTF_BRDFRenderPass.BuildRenderPass(SceneManager::GetPreRenderedMapSize());

	std::vector<std::shared_ptr<RenderedCubeMapTexture>> cubemap = { SceneManager::CubeMap };
	irradianceRenderPass.OneTimeDraw(cubemap, SceneManager::GetPreRenderedMapSize());
	SceneManager::IrradianceMap = irradianceRenderPass.IrradianceCubeMapList[0];
	prefilterRenderPass.OneTimeDraw(cubemap, SceneManager::GetPreRenderedMapSize());
	SceneManager::PrefilterMap = prefilterRenderPass.PrefilterCubeMapList[0];

	model = GLTF_Temp_Model("C:/Users/dotha/source/repos/EclipseEngine/Models/glTF-Sample-Models-master/2.0/Sponza/glTF/Sponza.gltf", glm::mat4(1.0f), 0);

	gLTFRenderPass.BuildRenderPass(model);
	frameBufferRenderPass.BuildRenderPass(gLTFRenderPass.RenderedTexture, gLTFRenderPass.RenderedTexture);
}

void GLTFRenderer::Update()
{
	model.Update(glm::mat4(1.0f));
}

void GLTFRenderer::ImGuiUpdate()
{
	ImGui::SliderFloat3("Position ", &model.MeshList[0]->MeshPosition.x, 0.0f, 1.0f);
	ImGui::SliderFloat3("Rotation ", &model.MeshList[0]->MeshRotation.x, 0.0f, 360.0f);
	ImGui::SliderFloat3("Scale ", &model.MeshList[0]->MeshScale.x, 0.0f, 1.0f);
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

void GLTFRenderer::Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	CommandBufferSubmitList.emplace_back(gLTFRenderPass.Draw(model));
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
}

void GLTFRenderer::Destroy()
{
	environmentToCubeRenderPass.Destroy();
	GLTF_BRDFRenderPass.Destroy();
	irradianceRenderPass.Destroy();
	prefilterRenderPass.Destroy();
	gLTFRenderPass.Destroy();
	frameBufferRenderPass.Destroy();

	model.Destroy();
}
