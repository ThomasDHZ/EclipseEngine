#include "SpriteGameObject3D.h"

SpriteGameObject3D::SpriteGameObject3D()
{
}

SpriteGameObject3D::SpriteGameObject3D(const std::string Name) : GameObject3D(Name, kSpriteRenderer)
{
	std::vector<Vertex3D> SpriteVertexList;

	Vertex3D vertex;
	vertex.Position = { -0.5f, -0.5f, 0.0f };
	vertex.Normal = { 0.0f, 0.0f, 0.0f };
	vertex.UV = { 1.0f, 0.0f };
	vertex.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex.Color = { 1.0f, 0.0f, 0.0f };
	SpriteVertexList.emplace_back(vertex);

	Vertex3D vertex2;
	vertex2.Position = { 0.5f, -0.5f, 0.0f };
	vertex2.Normal = { 0.0f, 0.0f, 0.0f };
	vertex2.UV = { 0.0f, 0.0f };
	vertex2.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex2.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex2.Color = { 0.0f, 1.0f, 0.0f };
	SpriteVertexList.emplace_back(vertex2);

	Vertex3D vertex3;
	vertex3.Position = { 0.5f,  0.5f, 0.0f };
	vertex3.Normal = { 0.0f, 0.0f, 0.0f };
	vertex3.UV = { 0.0f, 1.0f };
	vertex3.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex3.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex3.Color = { 0.0f, 0.0f, 1.0f };
	SpriteVertexList.emplace_back(vertex3);

	Vertex3D vertex4;
	vertex4.Position = { -0.5f,  0.5f, 0.0f };
	vertex4.Normal = { 0.0f, 0.0f, 0.0f };
	vertex4.UV = { 1.0f, 1.0f };
	vertex4.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex4.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex4.Color = { 1.0f, 0.0f, 1.0f };
	SpriteVertexList.emplace_back(vertex4);

	const std::vector<uint32_t> SpriteIndexList = {
	   0, 1, 2,
	   2, 3, 0
	};

	LoadRenderObject<Vertex3D>(SpriteVertexList, SpriteIndexList);
}

SpriteGameObject3D::SpriteGameObject3D(const std::string Name, const glm::vec3& position) : GameObject3D(Name, kSpriteRenderer, position)
{
	
		std::vector<Vertex3D> SpriteVertexList;

		Vertex3D vertex;
		vertex.Position = { -0.5f, -0.5f, 0.0f };
		vertex.Normal = { 0.0f, 0.0f, 0.0f };
		vertex.UV = { 1.0f, 0.0f };
		vertex.Tangant = { 0.0f, 0.0f, 0.0f };
		vertex.BiTangant = { 0.0f, 0.0f, 0.0f };
		vertex.Color = { 1.0f, 0.0f, 0.0f };
		SpriteVertexList.emplace_back(vertex);

		Vertex3D vertex2;
		vertex2.Position = { 0.5f, -0.5f, 0.0f };
		vertex2.Normal = { 0.0f, 0.0f, 0.0f };
		vertex2.UV = { 0.0f, 0.0f };
		vertex2.Tangant = { 0.0f, 0.0f, 0.0f };
		vertex2.BiTangant = { 0.0f, 0.0f, 0.0f };
		vertex2.Color = { 0.0f, 1.0f, 0.0f };
		SpriteVertexList.emplace_back(vertex2);

		Vertex3D vertex3;
		vertex3.Position = { 0.5f,  0.5f, 0.0f };
		vertex3.Normal = { 0.0f, 0.0f, 0.0f };
		vertex3.UV = { 0.0f, 1.0f };
		vertex3.Tangant = { 0.0f, 0.0f, 0.0f };
		vertex3.BiTangant = { 0.0f, 0.0f, 0.0f };
		vertex3.Color = { 0.0f, 0.0f, 1.0f };
		SpriteVertexList.emplace_back(vertex3);

		Vertex3D vertex4;
		vertex4.Position = { -0.5f,  0.5f, 0.0f };
		vertex4.Normal = { 0.0f, 0.0f, 0.0f };
		vertex4.UV = { 1.0f, 1.0f };
		vertex4.Tangant = { 0.0f, 0.0f, 0.0f };
		vertex4.BiTangant = { 0.0f, 0.0f, 0.0f };
		vertex4.Color = { 1.0f, 0.0f, 1.0f };
		SpriteVertexList.emplace_back(vertex4);

		const std::vector<uint32_t> SpriteIndexList = {
		   0, 1, 2,
		   2, 3, 0
		};

	LoadRenderObject<Vertex3D>(SpriteVertexList, SpriteIndexList);
}

SpriteGameObject3D::SpriteGameObject3D(const std::string Name, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, kSpriteRenderer, position, rotation)
{
	
		std::vector<Vertex3D> SpriteVertexList;

		Vertex3D vertex;
		vertex.Position = { -0.5f, -0.5f, 0.0f };
		vertex.Normal = { 0.0f, 0.0f, 0.0f };
		vertex.UV = { 1.0f, 0.0f };
		vertex.Tangant = { 0.0f, 0.0f, 0.0f };
		vertex.BiTangant = { 0.0f, 0.0f, 0.0f };
		vertex.Color = { 1.0f, 0.0f, 0.0f };
		SpriteVertexList.emplace_back(vertex);

		Vertex3D vertex2;
		vertex2.Position = { 0.5f, -0.5f, 0.0f };
		vertex2.Normal = { 0.0f, 0.0f, 0.0f };
		vertex2.UV = { 0.0f, 0.0f };
		vertex2.Tangant = { 0.0f, 0.0f, 0.0f };
		vertex2.BiTangant = { 0.0f, 0.0f, 0.0f };
		vertex2.Color = { 0.0f, 1.0f, 0.0f };
		SpriteVertexList.emplace_back(vertex2);

		Vertex3D vertex3;
		vertex3.Position = { 0.5f,  0.5f, 0.0f };
		vertex3.Normal = { 0.0f, 0.0f, 0.0f };
		vertex3.UV = { 0.0f, 1.0f };
		vertex3.Tangant = { 0.0f, 0.0f, 0.0f };
		vertex3.BiTangant = { 0.0f, 0.0f, 0.0f };
		vertex3.Color = { 0.0f, 0.0f, 1.0f };
		SpriteVertexList.emplace_back(vertex3);

		Vertex3D vertex4;
		vertex4.Position = { -0.5f,  0.5f, 0.0f };
		vertex4.Normal = { 0.0f, 0.0f, 0.0f };
		vertex4.UV = { 1.0f, 1.0f };
		vertex4.Tangant = { 0.0f, 0.0f, 0.0f };
		vertex4.BiTangant = { 0.0f, 0.0f, 0.0f };
		vertex4.Color = { 1.0f, 0.0f, 1.0f };
		SpriteVertexList.emplace_back(vertex4);

		const std::vector<uint32_t> SpriteIndexList = {
		   0, 1, 2,
		   2, 3, 0
		};

	LoadRenderObject<Vertex3D>(SpriteVertexList, SpriteIndexList);
}

SpriteGameObject3D::SpriteGameObject3D(const std::string Name, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject3D(Name, kSpriteRenderer, position, rotation, scale)
{
	std::vector<Vertex3D> SpriteVertexList;

	Vertex3D vertex;
	vertex.Position = { -0.5f, -0.5f, 0.0f };
	vertex.Normal = { 0.0f, 0.0f, 0.0f };
	vertex.UV = { 1.0f, 0.0f };
	vertex.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex.Color = { 1.0f, 0.0f, 0.0f };
	SpriteVertexList.emplace_back(vertex);

	Vertex3D vertex2;
	vertex2.Position = { 0.5f, -0.5f, 0.0f };
	vertex2.Normal = { 0.0f, 0.0f, 0.0f };
	vertex2.UV = { 0.0f, 0.0f };
	vertex2.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex2.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex2.Color = { 0.0f, 1.0f, 0.0f };
	SpriteVertexList.emplace_back(vertex2);

	Vertex3D vertex3;
	vertex3.Position = { 0.5f,  0.5f, 0.0f };
	vertex3.Normal = { 0.0f, 0.0f, 0.0f };
	vertex3.UV = { 0.0f, 1.0f };
	vertex3.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex3.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex3.Color = { 0.0f, 0.0f, 1.0f };
	SpriteVertexList.emplace_back(vertex3);

	Vertex3D vertex4;
	vertex4.Position = { -0.5f,  0.5f, 0.0f };
	vertex4.Normal = { 0.0f, 0.0f, 0.0f };
	vertex4.UV = { 1.0f, 1.0f };
	vertex4.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex4.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex4.Color = { 1.0f, 0.0f, 1.0f };
	SpriteVertexList.emplace_back(vertex4);

	const std::vector<uint32_t> SpriteIndexList = {
	   0, 1, 2,
	   2, 3, 0
	};

	LoadRenderObject<Vertex3D>(SpriteVertexList, SpriteIndexList);
}

SpriteGameObject3D::SpriteGameObject3D(const std::string Name, std::shared_ptr<GLTFMaterial> material) : GameObject3D(Name, kSpriteRenderer)
{
	std::vector<Vertex3D> SpriteVertexList;

	Vertex3D vertex;
	vertex.Position = { -0.5f, -0.5f, 0.0f };
	vertex.Normal = { 0.0f, 0.0f, 0.0f };
	vertex.UV = { 1.0f, 0.0f };
	vertex.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex.Color = { 1.0f, 0.0f, 0.0f };
	SpriteVertexList.emplace_back(vertex);

	Vertex3D vertex2;
	vertex2.Position = { 0.5f, -0.5f, 0.0f };
	vertex2.Normal = { 0.0f, 0.0f, 0.0f };
	vertex2.UV = { 0.0f, 0.0f };
	vertex2.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex2.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex2.Color = { 0.0f, 1.0f, 0.0f };
	SpriteVertexList.emplace_back(vertex2);

	Vertex3D vertex3;
	vertex3.Position = { 0.5f,  0.5f, 0.0f };
	vertex3.Normal = { 0.0f, 0.0f, 0.0f };
	vertex3.UV = { 0.0f, 1.0f };
	vertex3.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex3.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex3.Color = { 0.0f, 0.0f, 1.0f };
	SpriteVertexList.emplace_back(vertex3);

	Vertex3D vertex4;
	vertex4.Position = { -0.5f,  0.5f, 0.0f };
	vertex4.Normal = { 0.0f, 0.0f, 0.0f };
	vertex4.UV = { 1.0f, 1.0f };
	vertex4.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex4.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex4.Color = { 1.0f, 0.0f, 1.0f };
	SpriteVertexList.emplace_back(vertex4);

	const std::vector<uint32_t> SpriteIndexList = {
	   0, 1, 2,
	   2, 3, 0
	};

	LoadRenderObject<Vertex3D>(SpriteVertexList, SpriteIndexList, material);
}

SpriteGameObject3D::SpriteGameObject3D(const std::string Name, std::shared_ptr<GLTFMaterial> material, const glm::vec3& position) : GameObject3D(Name, kSpriteRenderer, position)
{
	std::vector<Vertex3D> SpriteVertexList;

	Vertex3D vertex;
	vertex.Position = { -0.5f, -0.5f, 0.0f };
	vertex.Normal = { 0.0f, 0.0f, 0.0f };
	vertex.UV = { 1.0f, 0.0f };
	vertex.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex.Color = { 1.0f, 0.0f, 0.0f };
	SpriteVertexList.emplace_back(vertex);

	Vertex3D vertex2;
	vertex2.Position = { 0.5f, -0.5f, 0.0f };
	vertex2.Normal = { 0.0f, 0.0f, 0.0f };
	vertex2.UV = { 0.0f, 0.0f };
	vertex2.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex2.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex2.Color = { 0.0f, 1.0f, 0.0f };
	SpriteVertexList.emplace_back(vertex2);

	Vertex3D vertex3;
	vertex3.Position = { 0.5f,  0.5f, 0.0f };
	vertex3.Normal = { 0.0f, 0.0f, 0.0f };
	vertex3.UV = { 0.0f, 1.0f };
	vertex3.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex3.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex3.Color = { 0.0f, 0.0f, 1.0f };
	SpriteVertexList.emplace_back(vertex3);

	Vertex3D vertex4;
	vertex4.Position = { -0.5f,  0.5f, 0.0f };
	vertex4.Normal = { 0.0f, 0.0f, 0.0f };
	vertex4.UV = { 1.0f, 1.0f };
	vertex4.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex4.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex4.Color = { 1.0f, 0.0f, 1.0f };
	SpriteVertexList.emplace_back(vertex4);

	const std::vector<uint32_t> SpriteIndexList = {
	   0, 1, 2,
	   2, 3, 0
	};

	LoadRenderObject<Vertex3D>(SpriteVertexList, SpriteIndexList, material);
}

SpriteGameObject3D::SpriteGameObject3D(const std::string Name, std::shared_ptr<GLTFMaterial> material, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, kSpriteRenderer, position, rotation)
{
	std::vector<Vertex3D> SpriteVertexList;

	Vertex3D vertex;
	vertex.Position = { -0.5f, -0.5f, 0.0f };
	vertex.Normal = { 0.0f, 0.0f, 0.0f };
	vertex.UV = { 1.0f, 0.0f };
	vertex.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex.Color = { 1.0f, 0.0f, 0.0f };
	SpriteVertexList.emplace_back(vertex);

	Vertex3D vertex2;
	vertex2.Position = { 0.5f, -0.5f, 0.0f };
	vertex2.Normal = { 0.0f, 0.0f, 0.0f };
	vertex2.UV = { 0.0f, 0.0f };
	vertex2.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex2.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex2.Color = { 0.0f, 1.0f, 0.0f };
	SpriteVertexList.emplace_back(vertex2);

	Vertex3D vertex3;
	vertex3.Position = { 0.5f,  0.5f, 0.0f };
	vertex3.Normal = { 0.0f, 0.0f, 0.0f };
	vertex3.UV = { 0.0f, 1.0f };
	vertex3.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex3.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex3.Color = { 0.0f, 0.0f, 1.0f };
	SpriteVertexList.emplace_back(vertex3);

	Vertex3D vertex4;
	vertex4.Position = { -0.5f,  0.5f, 0.0f };
	vertex4.Normal = { 0.0f, 0.0f, 0.0f };
	vertex4.UV = { 1.0f, 1.0f };
	vertex4.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex4.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex4.Color = { 1.0f, 0.0f, 1.0f };
	SpriteVertexList.emplace_back(vertex4);

	const std::vector<uint32_t> SpriteIndexList = {
	   0, 1, 2,
	   2, 3, 0
	};

	LoadRenderObject<Vertex3D>(SpriteVertexList, SpriteIndexList, material);
}

SpriteGameObject3D::SpriteGameObject3D(const std::string Name, std::shared_ptr<GLTFMaterial> material, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject3D(Name, kSpriteRenderer, position, rotation, scale)
{
	std::vector<Vertex3D> SpriteVertexList;

	Vertex3D vertex;
	vertex.Position = { -0.5f, -0.5f, 0.0f };
	vertex.Normal = { 0.0f, 0.0f, 0.0f };
	vertex.UV = { 1.0f, 0.0f };
	vertex.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex.Color = { 1.0f, 0.0f, 0.0f };
	SpriteVertexList.emplace_back(vertex);

	Vertex3D vertex2;
	vertex2.Position = { 0.5f, -0.5f, 0.0f };
	vertex2.Normal = { 0.0f, 0.0f, 0.0f };
	vertex2.UV = { 0.0f, 0.0f };
	vertex2.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex2.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex2.Color = { 0.0f, 1.0f, 0.0f };
	SpriteVertexList.emplace_back(vertex2);

	Vertex3D vertex3;
	vertex3.Position = { 0.5f,  0.5f, 0.0f };
	vertex3.Normal = { 0.0f, 0.0f, 0.0f };
	vertex3.UV = { 0.0f, 1.0f };
	vertex3.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex3.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex3.Color = { 0.0f, 0.0f, 1.0f };
	SpriteVertexList.emplace_back(vertex3);

	Vertex3D vertex4;
	vertex4.Position = { -0.5f,  0.5f, 0.0f };
	vertex4.Normal = { 0.0f, 0.0f, 0.0f };
	vertex4.UV = { 1.0f, 1.0f };
	vertex4.Tangant = { 0.0f, 0.0f, 0.0f };
	vertex4.BiTangant = { 0.0f, 0.0f, 0.0f };
	vertex4.Color = { 1.0f, 0.0f, 1.0f };
	SpriteVertexList.emplace_back(vertex4);

	const std::vector<uint32_t> SpriteIndexList = {
	   0, 1, 2,
	   2, 3, 0
	};

	LoadRenderObject<Vertex3D>(SpriteVertexList, SpriteIndexList, material);
}

SpriteGameObject3D::~SpriteGameObject3D()
{
}
