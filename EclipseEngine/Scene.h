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
#include "Camera.h"



class Scene
{
private:

	std::vector<Vertex> vertices = {
   {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
   {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
   {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
   {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
	};

	std::vector<uint32_t> indices = {
	   0, 1, 2, 2, 3, 0
	};

	Mesh mesh;
	Texture texture;
	Camera camera;
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

