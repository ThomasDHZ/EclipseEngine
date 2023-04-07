#include "GLTFRenderer2D.h"
#include "SpriteRenderer.h"

GLTFRenderer2D::GLTFRenderer2D()
{
	 //std::shared_ptr<SpriteRenderer> obj = std::make_shared<SpriteRenderer>(SpriteRenderer("Testobject", material, glm::vec2(0.0f), 0));
  //   std::shared_ptr<SpriteRenderer> obj2 = std::make_shared<SpriteRenderer>(SpriteRenderer("Testobject2", material2, glm::vec2(1.0f, 0.0f), 0));
  //   std::shared_ptr<SpriteRenderer> obj3 = std::make_shared<SpriteRenderer>(SpriteRenderer("Testobject3", material, glm::vec2(2.0f, 0.0f), 0));
  //   std::shared_ptr<SpriteRenderer> obj4 = std::make_shared<SpriteRenderer>(SpriteRenderer("Testobject4", material2, glm::vec2(3.0f, 0.0f), 0));

}

GLTFRenderer2D::~GLTFRenderer2D()
{
}

void GLTFRenderer2D::BuildRenderer()
{

	//frameBufferRenderPass.BuildRenderPass(renderPass2D.renderedTexture);
}

void GLTFRenderer2D::Update()
{
}

void GLTFRenderer2D::ImGuiUpdate()
{
}

void GLTFRenderer2D::Draw(SceneProperties& sceneProperites, std::vector<VkCommandBuffer>& CommandBufferSubmitList)
{
	//CommandBufferSubmitList.emplace_back(frameBufferRenderPass.Draw());
}

void GLTFRenderer2D::Destroy()
{
	frameBufferRenderPass.Destroy();
}
