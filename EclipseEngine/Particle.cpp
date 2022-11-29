#include "Particle.h"

Particle::Particle() : Mesh3D()
{
}

Particle::Particle(ParticleLoader3D& particaleLoader) : Mesh3D()
{
	Alive = true;
	Gravity = particaleLoader.Gravity;
	LifeSpan = particaleLoader.LifeSpan;
	ElapsedTime = 0.0f;

	ParentGameObjectID = particaleLoader.ParentGameObjectID;
	ParentModelID = particaleLoader.ParentModelID;
	GenerateID();
	GenerateColorID();

	GameObjectTransformMatrix = particaleLoader.GameObjectTransform;
	ModelTransformMatrix = particaleLoader.ModelTransform;
	MeshTransformMatrix = particaleLoader.MeshTransform;

	VertexList = particaleLoader.VerticeList;
	IndexList = particaleLoader.IndexList;
	material = particaleLoader.MaterialPtr;

	MeshType = particaleLoader.MeshType;
	MeshSubType = particaleLoader.MeshSubType;

	VertexCount = VertexList.size();
	IndexCount = IndexList.size();
	TriangleCount = static_cast<uint32_t>(IndexList.size()) / 3;

	VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(Vertex3D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	IndexBuffer.CreateBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	TransformBuffer.CreateBuffer(&LastTransform, sizeof(glm::mat4), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	RTXMeshStartUp();
	//InstancingStartUp(meshLoader.instanceData);
}

Particle::~Particle()
{
}

void Particle::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	if (Alive)
	{
		MeshPosition += Velocity * VulkanRenderer::GetFrameTimeDurationSeconds();

		Mesh3D::Update(GameObjectMatrix, ModelMatrix);
		ElapsedTime += VulkanRenderer::GetFrameTimeDurationSeconds();

		if (LifeSpan < ElapsedTime)
		{
			Alive = false;
		}
	}
}
