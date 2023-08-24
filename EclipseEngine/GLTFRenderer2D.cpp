#include "GLTFRenderer2D.h"
#include "SpriteGameObject2D.h"
#include "OrthographicCamera.h"

GLTFRenderer2D::GLTFRenderer2D()
{
}

GLTFRenderer2D::~GLTFRenderer2D()
{
}

void GLTFRenderer2D::BuildRenderer()
{
	GLTFSceneManager::StartUp();
	GLTFSceneManager::ActiveCamera = std::make_shared<OrthographicCamera>(OrthographicCamera("camera", VulkanRenderer::GetSwapChainResolutionVec2().x, VulkanRenderer::GetSwapChainResolutionVec2().y, 10.5f));

	std::shared_ptr<Material> material = std::make_shared<Material>(Material("TestMaterial"));
	material->AlbedoMap = GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D("C:/Users/dotha/source/repos/VulkanGraphics/texture/Brick_diffuseOriginal.bmp", TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB)));
	//material->AlphaMap = std::make_shared<Texture2D>(Texture2D(mario2, TextureTypeEnum::kAlphaTextureMap, VK_FORMAT_R8G8B8A8_UNORM));
	
	GLTFSceneManager::UpdateBufferIndex();
	material->UpdateBuffer();
	GLTFSceneManager::AddMaterial(material);

	//GLTFSceneManager::AddSpriteGameObject2D("Testobject", material, glm::vec2(0.0f), 0);
	//GLTFSceneManager::AddSpriteGameObject2D("Testobject2", material, glm::vec2(1.0f, 0.0f), 0);
	//GLTFSceneManager::AddSpriteGameObject2D("Testobject3", material, glm::vec2(2.0f, 0.0f), 0);
	//GLTFSceneManager::AddSpriteGameObject2D("Testobject4", material, glm::vec2(3.0f, 0.0f), 0);
	//GLTFSceneManager::AddSpriteGameObject2D("Testobject5", material, glm::vec2(0.0f, 1.0f), 1);
	//GLTFSceneManager::AddSpriteGameObject2D("Testobject6", material, glm::vec2(1.0f, 1.0f), 1);
	//GLTFSceneManager::AddSpriteGameObject2D("Testobject7", material, glm::vec2(2.0f, 1.0f), 1);
	//GLTFSceneManager::AddSpriteGameObject2D("Testobject8", material, glm::vec2(3.0f, 1.0f), 1);
	//GLTFSceneManager::AddLineGameObject2D("sponza", glm::vec2(0.0f), glm::vec2(25.0f), 0);
	GLTFSceneManager::AddGridGameObject2D("sponza", 50, 50, 50.0f, 50.0f, 0);
	renderPass2D.BuildRenderPass();
	frameBufferRenderPass.BuildRenderPass(renderPass2D.renderedTexture, renderPass2D.renderedTexture);

	GLTFSceneManager::Update();
	VulkanRenderer::UpdateRendererFlag = true;
}

void GLTFRenderer2D::Update()
{
	GLTFSceneManager::Update();
}

void GLTFRenderer2D::ImGuiUpdate()
{
}

void GLTFRenderer2D::Draw(std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	CommandBufferSubmitList.emplace_back(renderPass2D.Draw(GLTFSceneManager::GameObjectList));
	CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
}

void GLTFRenderer2D::Destroy()
{
	GLTFSceneManager::Destroy();
	renderPass2D.Destroy();
	frameBufferRenderPass.Destroy();
}
