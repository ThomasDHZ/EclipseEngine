#include "Renderer2D.h"

Renderer2D::Renderer2D()
{
}

Renderer2D::~Renderer2D()
{
}

void Renderer2D::StartUp()
{
	meshPickerRenderPass.StartUp();
	renderPass2D.StartUp();
	frameBufferRenderPass.StartUp(renderPass2D.renderedTexture);
}

void Renderer2D::Update()
{
	if (VulkanRenderer::EditorModeFlag &&
		!VulkanRenderer::ImGUILayerActive &&
		Mouse::GetMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		const glm::vec2 mouseCoord = Mouse::GetMouseCoords();
		meshPickerRenderPass.ReadPixel(mouseCoord);
	}
}

void Renderer2D::RebuildRenderers()
{
	meshPickerRenderPass.RebuildSwapChain();
	renderPass2D.RebuildSwapChain();
	frameBufferRenderPass.RebuildSwapChain(renderPass2D.renderedTexture);
}

void Renderer2D::Draw(SceneProperties& sceneProperites, std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	if (VulkanRenderer::EditorModeFlag)
	{
		CommandBufferSubmitList.emplace_back(meshPickerRenderPass.Draw());
	}

	CommandBufferSubmitList.emplace_back(renderPass2D.Draw());
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
}

void Renderer2D::Destroy()
{
	meshPickerRenderPass.Destroy();
	renderPass2D.Destroy();
	frameBufferRenderPass.Destroy();
}
