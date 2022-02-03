#pragma once
#include "Window.h"
#include "VulkanRenderer.h"
#include "RenderPass2D.h"
#include "InterfaceRenderPass.h"
#include "FrameBufferRenderPass.h"
#include "UniformBuffer.h"
#include "Texture.h"
#include "Vertex.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "RenderedTexture.h"
#include "Texture2D.h"

class Scene
{
private:

	std::vector<GameObject> objList;
	Texture2D texture;
	OrthographicCamera camera;
	PerspectiveCamera camera2;
	SceneProperties sceneProperites{};
	InterfaceRenderPass imGuiRenderPass;
	RenderPass2D renderPass2D;
	FrameBufferRenderPass frameBufferRenderPass;

public:
	Scene();
	~Scene();

	void StartUp();
	void Update();
	void ImGuiUpdate();
	void RebuildRenderers();
	void Draw();
	void Destroy();
};

