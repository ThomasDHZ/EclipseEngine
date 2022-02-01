#pragma once
#include "Window.h"
#include "VulkanRenderer.h"
#include "RenderPass2D.h"
#include "InterfaceRenderPass.h"
#include "FrameBufferRenderPass.h"
#include "UniformBuffer.h"
#include "Texture.h"
#include "Vertex.h"
#include "Mesh.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"
#include "GameObject.h"
#include "SpriteRenderer.h"



class Scene
{
private:

	std::vector<GameObject> objList;
	Texture texture;
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
	void Draw();
};

