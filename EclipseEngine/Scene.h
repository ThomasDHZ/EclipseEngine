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
#include "Renderer2D.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "RayTraceRenderer.h"
#include "LightManager.h"
#include "PBRRenderer.h"
#include "SoundEffectManager.h"
#include "SoundSource.h"
#include "PBRRealTimeRenderer.h"
#include "RayTracePBRRenderer.h"
#include "ParticleSystem.h"
#include "GLTFRenderer.h"
#include "GLTFRenderer2D.h"

class Scene
{
private:
	//GLTFRenderer pbrRenderer;
	GLTFRenderer2D spriteRenderer;
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

