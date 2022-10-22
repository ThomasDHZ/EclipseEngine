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
#include "GameObject2D.h"
#include "GameObject3D.h"
#include "RenderedTexture.h"
#include "Texture2D.h"
#include "GameObjectManager.h"
#include "BlinnPhongRenderer.h"
#include "Renderer2D.h"
#include "BlinnPhongRenderer.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "RayTraceRenderer.h"
#include "LightManager.h"
#include "HybridRenderer.h"
#include "PBREditorRenderer.h"
#include "SoundEffectManager.h"
#include "SoundSource.h"
#include "PBRRealTimeRenderer.h"
#include "PBRPlayRenderer.h"

class Scene
{
private:
//	Renderer2D renderer2D;
//	BlinnPhongRenderer blinnPhongRenderer;
//	HybridRenderer hybridRenderer;
	//PBRPlayRenderer pbrRenderer;
	PBREditorRenderer pbrRenderer;
	//PBRRealTimeRenderer pbrRenderer;
//	RayTraceRenderer rayTraceRenderer;
	std::shared_ptr<GameObject2D> obj5;
	std::shared_ptr<Texture> BakeTexture;
public:
	Scene();
	~Scene();

	void StartUp();
	void Update();
	void ImGuiUpdate();
	void BuildRenderers();
	void Draw();
	void Destroy();
};

