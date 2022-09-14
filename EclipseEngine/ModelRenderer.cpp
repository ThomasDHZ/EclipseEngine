#include "ModelRenderer.h"

ModelRenderer::ModelRenderer()
{
}

ModelRenderer::ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh) : GameObject3D(Name)
{
	model = std::make_shared<Model>(Model(mesh, GameObjectID));
}

ModelRenderer::ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh, const glm::vec3& position) : GameObject3D(Name, position)
{
	model = std::make_shared<Model>(Model(mesh, GameObjectID));
}

ModelRenderer::ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, position, rotation)
{
	model = std::make_shared<Model>(Model(mesh, GameObjectID));
}

ModelRenderer::ModelRenderer(const std::string Name, std::shared_ptr<Mesh> mesh, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject3D(Name, position, rotation, scale)
{
	model = std::make_shared<Model>(Model(mesh, GameObjectID));
}

ModelRenderer::ModelRenderer(const std::string Name, std::vector<std::shared_ptr<Mesh>>& mesh) : GameObject3D(Name)
{
	model = std::make_shared<Model>(Model(mesh, GameObjectID));
}

ModelRenderer::ModelRenderer(const std::string Name, std::vector<std::shared_ptr<Mesh>>& mesh, glm::vec3 position) : GameObject3D(Name, position)
{
	model = std::make_shared<Model>(Model(mesh, GameObjectID));
}

ModelRenderer::ModelRenderer(const std::string Name, std::vector<std::shared_ptr<Mesh>>& mesh, glm::vec3 position, glm::vec3 rotation) : GameObject3D(Name, position, rotation)
{
	model = std::make_shared<Model>(Model(mesh, GameObjectID));
}

ModelRenderer::ModelRenderer(const std::string Name, std::vector<std::shared_ptr<Mesh>>& mesh, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : GameObject3D(Name, position, rotation, scale)
{
	model = std::make_shared<Model>(Model(mesh, GameObjectID));
}

ModelRenderer::ModelRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData) : GameObject3D(Name)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.instanceData = instanceData;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	model = std::make_shared<Model>(Model(meshLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData, glm::vec3 position) : GameObject3D(Name, position)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.instanceData = instanceData;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	model = std::make_shared<Model>(Model(meshLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData, glm::vec3 position, glm::vec3 rotation) : GameObject3D(Name, position, rotation)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.instanceData = instanceData;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	model = std::make_shared<Model>(Model(meshLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : GameObject3D(Name, position, rotation, scale)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.instanceData = instanceData;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	model = std::make_shared<Model>(Model(meshLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData, std::shared_ptr<Material> material) : GameObject3D(Name)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.instanceData = instanceData;
	meshLoader.MaterialPtr = material;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	model = std::make_shared<Model>(Model(meshLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData, std::shared_ptr<Material> material, glm::vec3 position) : GameObject3D(Name, position)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.instanceData = instanceData;
	meshLoader.MaterialPtr = material;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	model = std::make_shared<Model>(Model(meshLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData, std::shared_ptr<Material> material, glm::vec3 position, glm::vec3 rotation) : GameObject3D(Name, position, rotation)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.instanceData = instanceData;
	meshLoader.MaterialPtr = material;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	model = std::make_shared<Model>(Model(meshLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, InstancingDataStruct& instanceData, std::shared_ptr<Material> material, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : GameObject3D(Name, position, rotation, scale)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.instanceData = instanceData;
	meshLoader.MaterialPtr = material;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	model = std::make_shared<Model>(Model(meshLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::string& FilePath) : GameObject3D(Name)
{
	ModelLoader modelLoader = {};
	modelLoader.FilePath = FilePath;
	modelLoader.ParentGameObjectID = GameObjectID;
	modelLoader.GameObjectTransform = GetGameObjectMatrix();
	
	model = std::make_shared<Model>(Model(modelLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::string& FilePath, const glm::vec3& position) : GameObject3D(Name, position)
{
	ModelLoader modelLoader = {};
	modelLoader.FilePath = FilePath;
	modelLoader.ParentGameObjectID = GameObjectID;
	modelLoader.GameObjectTransform = GetGameObjectMatrix();

	model = std::make_shared<Model>(Model(modelLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::string& FilePath, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, position, rotation)
{
	ModelLoader modelLoader = {};
	modelLoader.FilePath = FilePath;
	modelLoader.ParentGameObjectID = GameObjectID;
	modelLoader.GameObjectTransform = GetGameObjectMatrix();

	model = std::make_shared<Model>(Model(modelLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::string& FilePath, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject3D(Name, position, rotation, scale)
{
	ModelLoader modelLoader = {};
	modelLoader.FilePath = FilePath;
	modelLoader.ParentGameObjectID = GameObjectID;
	modelLoader.GameObjectTransform = GetGameObjectMatrix();

	model = std::make_shared<Model>(Model(modelLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::string& FilePath, InstancingDataStruct& instanceData) : GameObject3D(Name)
{
	ModelLoader modelLoader = {};
	modelLoader.FilePath = FilePath;
	modelLoader.ParentGameObjectID = GameObjectID;
	modelLoader.GameObjectTransform = GetGameObjectMatrix();
	modelLoader.instanceData = instanceData;

	model = std::make_shared<Model>(Model(modelLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::string& FilePath, InstancingDataStruct& instanceData, glm::vec3 position) : GameObject3D(Name, position)
{
	ModelLoader modelLoader = {};
	modelLoader.FilePath = FilePath;
	modelLoader.ParentGameObjectID = GameObjectID;
	modelLoader.GameObjectTransform = GetGameObjectMatrix();
	modelLoader.instanceData = instanceData;

	model = std::make_shared<Model>(Model(modelLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::string& FilePath, InstancingDataStruct& instanceData, glm::vec3 position, glm::vec3 rotation) : GameObject3D(Name, position, rotation)
{
	ModelLoader modelLoader = {};
	modelLoader.FilePath = FilePath;
	modelLoader.ParentGameObjectID = GameObjectID;
	modelLoader.GameObjectTransform = GetGameObjectMatrix();
	modelLoader.instanceData = instanceData;

	model = std::make_shared<Model>(Model(modelLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::string& FilePath, InstancingDataStruct& instanceData, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : GameObject3D(Name, position, rotation, scale)
{
	ModelLoader modelLoader = {};
	modelLoader.FilePath = FilePath;
	modelLoader.ParentGameObjectID = GameObjectID;
	modelLoader.GameObjectTransform = GetGameObjectMatrix();
	modelLoader.instanceData = instanceData;

	model = std::make_shared<Model>(Model(modelLoader));
}

ModelRenderer::ModelRenderer(nlohmann::json json) : GameObject3D(json)
{
	from_json(json);
}

ModelRenderer::~ModelRenderer()
{
}

void ModelRenderer::LoadPrefab(nlohmann::json& json)
{
	model->LoadPrefab(json);
}

void ModelRenderer::SaveAsPrefab(nlohmann::json& json)
{
	//model->m(json);
}

void ModelRenderer::Update(float DeltaTime)
{
	GameObject::Update(DeltaTime);
	model->Update(GameObjectTransform);
}

void ModelRenderer::Destroy()
{
	model->Destroy();
}