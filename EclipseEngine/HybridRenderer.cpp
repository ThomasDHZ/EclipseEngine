#include "HybridRenderer.h"

std::string HybridRenderer::BaseShaderFilePath = "../Shaders/";

HybridRenderer::HybridRenderer()
{
}

HybridRenderer::~HybridRenderer()
{
}

void HybridRenderer::BuildRenderer()
{
	meshPickerRenderPass.BuildRenderPass();
	environmentToCubeRenderPass.BuildRenderPass(4096.0f / 4);
	raytraceHybridPass.BuildRenderPass(SceneManager::CubeMap);
	GBufferRenderPass.BuildRenderPass(raytraceHybridPass.RenderedShadowTexture);
	deferredRenderPass.BuildRenderPass(GBufferRenderPass.PositionTexture,
								GBufferRenderPass.TangentTexture,
								GBufferRenderPass.BiTangentTexture,
								GBufferRenderPass.TBNormalTexture,
								GBufferRenderPass.NormalTexture,
								GBufferRenderPass.AlbedoTexture,
								GBufferRenderPass.SpecularTexture,
								GBufferRenderPass.BloomTexture,
								raytraceHybridPass.RenderedShadowTexture);
	frameBufferRenderPass.BuildRenderPass(deferredRenderPass.RenderedTexture);
	AnimationRenderer.StartUp();
}

void HybridRenderer::Update()
{
	if (SceneManager::EditorModeFlag &&
		!VulkanRenderer::ImGUILayerActive &&
		Mouse::GetMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		const glm::vec2 mouseCoord = Mouse::GetMouseCoords();
		const Pixel pixel = meshPickerRenderPass.ReadPixel(mouseCoord);

		MeshRendererManager::SetSelectedMesh(MeshRendererManager::GetMeshByColorID(pixel));
	}
}

void HybridRenderer::ImGuiUpdate()
{
	if (SceneManager::EditorModeFlag)
	{
		if (ImGui::Button("Play Mode"))
		{
			SceneManager::EditorModeFlag = false;
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

void HybridRenderer::Draw(SceneProperties& sceneProperites, std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	if (SceneManager::EditorModeFlag)
	{
		CommandBufferSubmitList.emplace_back(meshPickerRenderPass.Draw());
	}

	CommandBufferSubmitList.emplace_back(GBufferRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(deferredRenderPass.Draw());
	CommandBufferSubmitList.emplace_back(raytraceHybridPass.Draw());
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
	AnimationRenderer.StartUp();
}

void HybridRenderer::Destroy()
{
	raytraceHybridPass.Destroy();
	environmentToCubeRenderPass.Destroy();
	meshPickerRenderPass.Destroy();
	GBufferRenderPass.Destroy();
	deferredRenderPass.Destroy();
	frameBufferRenderPass.Destroy();
	AnimationRenderer.Destroy();
}
