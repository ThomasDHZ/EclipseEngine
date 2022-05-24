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
#include "GameObjectManager.h"
#include "BlinnPhongRenderer.h"
#include "Renderer2D.h"
#include "BlinnPhongRenderer.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "RayTraceRenderer.h"
#include "MeshRenderer.h"
#include "LightManager.h"
#include "HybridRenderer.h"
#include "PBRRenderer.h"

class Scene
{
private:
	SceneProperties sceneProperites;
	ConstSkyBoxView cubeMapInfo;

	OrthographicCamera camera;
	PerspectiveCamera camera2;

	//Renderer2D renderer2D;
	BlinnPhongRenderer blinnPhongRenderer;
	//HybridRenderer hybridRenderer;
	//PBRRenderer pbrRenderer;
	RayTraceRenderer rayTraceRenderer;

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

