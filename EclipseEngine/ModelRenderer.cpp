#include "ModelRenderer.h"

ModelRenderer::ModelRenderer()
{
}

ModelRenderer::ModelRenderer(const std::string Name, MeshLoader3D& meshloader) : GameObject3D(Name)
{
	meshloader.ParentGameObjectID = GameObjectID;
	model = std::make_shared<Model>(Model(meshloader));
}

ModelRenderer::ModelRenderer(const std::string Name, MeshLoader3D& meshloader, const glm::vec3& position) : GameObject3D(Name, position)
{
	meshloader.ParentGameObjectID = GameObjectID;
	model = std::make_shared<Model>(Model(meshloader));
}

ModelRenderer::ModelRenderer(const std::string Name, MeshLoader3D& meshloader, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, position, rotation)
{
	meshloader.ParentGameObjectID = GameObjectID;
	model = std::make_shared<Model>(Model(meshloader));
}

ModelRenderer::ModelRenderer(const std::string Name, MeshLoader3D& meshloader, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject3D(Name, position, rotation, scale)
{
	meshloader.ParentGameObjectID = GameObjectID;
	model = std::make_shared<Model>(Model(meshloader));
}

ModelRenderer::ModelRenderer(const std::string Name, ModelLoader& modelloader) : GameObject3D(Name)
{
	modelloader.ParentGameObjectID = GameObjectID;
	modelloader.GameObjectTransform = GetGameObjectMatrix();

	model = std::make_shared<Model>(Model(modelloader));
}

ModelRenderer::ModelRenderer(const std::string Name, ModelLoader& modelloader, const glm::vec3& position) : GameObject3D(Name, position)
{
	modelloader.ParentGameObjectID = GameObjectID;
	modelloader.GameObjectTransform = GetGameObjectMatrix();

	model = std::make_shared<Model>(Model(modelloader));
}

ModelRenderer::ModelRenderer(const std::string Name, ModelLoader& modelloader, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, position, rotation)
{
	modelloader.ParentGameObjectID = GameObjectID;
	model = std::make_shared<Model>(Model(modelloader));
	modelloader.GameObjectTransform = GetGameObjectMatrix();
}

ModelRenderer::ModelRenderer(const std::string Name, ModelLoader& modelloader, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject3D(Name, position, rotation, scale)
{
	modelloader.ParentGameObjectID = GameObjectID;
	modelloader.GameObjectTransform = GetGameObjectMatrix();

	model = std::make_shared<Model>(Model(modelloader));
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

ModelRenderer::ModelRenderer(const std::string Name, std::vector<std::shared_ptr<Mesh>>& mesh, const glm::vec3& position) : GameObject3D(Name, position)
{
	model = std::make_shared<Model>(Model(mesh, GameObjectID));
}

ModelRenderer::ModelRenderer(const std::string Name, std::vector<std::shared_ptr<Mesh>>& mesh, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, position, rotation)
{
	model = std::make_shared<Model>(Model(mesh, GameObjectID));
}

ModelRenderer::ModelRenderer(const std::string Name, std::vector<std::shared_ptr<Mesh>>& mesh, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject3D(Name, position, rotation, scale)
{
	model = std::make_shared<Model>(Model(mesh, GameObjectID));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::vector<Vertex3D>& VertexList, const std::vector<uint32_t>& IndexList) : GameObject3D(Name)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	model = std::make_shared<Model>(Model(meshLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::vector<Vertex3D>& VertexList, const std::vector<uint32_t>& IndexList, const glm::vec3& position) : GameObject3D(Name, position)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	model = std::make_shared<Model>(Model(meshLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::vector<Vertex3D>& VertexList, const std::vector<uint32_t>& IndexList, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, position, rotation)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	model = std::make_shared<Model>(Model(meshLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, const  std::vector<Vertex3D>& VertexList, const std::vector<uint32_t>& IndexList, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) :
	GameObject3D(Name, position, rotation, scale)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	model = std::make_shared<Model>(Model(meshLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::vector<Vertex3D>& VertexList, const std::vector<uint32_t>& IndexList, std::shared_ptr<Material> material) : GameObject3D(Name)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MaterialPtr = material;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	model = std::make_shared<Model>(Model(meshLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::vector<Vertex3D>& VertexList, const std::vector<uint32_t>& IndexList, std::shared_ptr<Material> material, const glm::vec3& position) : GameObject3D(Name, position)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MaterialPtr = material;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	model = std::make_shared<Model>(Model(meshLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::vector<Vertex3D>& VertexList, const std::vector<uint32_t>& IndexList, std::shared_ptr<Material> material, const glm::vec3& position, const glm::vec3& rotation) :
	GameObject3D(Name, position, rotation)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MaterialPtr = material;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	model = std::make_shared<Model>(Model(meshLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::vector<Vertex3D>& VertexList, const std::vector<uint32_t>& IndexList, std::shared_ptr<Material> material, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) :
	GameObject3D(Name, position, rotation, scale)
{
	MeshLoader3D meshLoader{};
	meshLoader.ParentGameObjectID = GameObjectID;
	meshLoader.VerticeList = VertexList;
	meshLoader.IndexList = IndexList;
	meshLoader.MaterialPtr = material;
	meshLoader.MeshType = MeshTypeEnum::kPolygonInstanced;
	meshLoader.MeshSubType = MeshSubTypeEnum::kNormal;

	model = std::make_shared<Model>(Model(meshLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::vector<Vertex3D>& VertexList, const std::vector<uint32_t>& IndexList, const InstancingDataStruct& instanceData) : GameObject3D(Name)
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

ModelRenderer::ModelRenderer(const std::string Name, const std::vector<Vertex3D>& VertexList, const std::vector<uint32_t>& IndexList, const InstancingDataStruct& instanceData, const glm::vec3& position) : GameObject3D(Name, position)
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

ModelRenderer::ModelRenderer(const std::string Name, const std::vector<Vertex3D>& VertexList, const std::vector<uint32_t>& IndexList, const InstancingDataStruct& instanceData, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, position, rotation)
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

ModelRenderer::ModelRenderer(const std::string Name, const  std::vector<Vertex3D>& VertexList, const std::vector<uint32_t>& IndexList, const InstancingDataStruct& instanceData, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) :
	GameObject3D(Name, position, rotation, scale)
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

ModelRenderer::ModelRenderer(const std::string Name, const std::vector<Vertex3D>& VertexList, const std::vector<uint32_t>& IndexList, const InstancingDataStruct& instanceData, std::shared_ptr<Material> material) : GameObject3D(Name)
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

ModelRenderer::ModelRenderer(const std::string Name, const std::vector<Vertex3D>& VertexList, const std::vector<uint32_t>& IndexList, const InstancingDataStruct& instanceData, std::shared_ptr<Material> material, const glm::vec3& position) : GameObject3D(Name, position)
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

ModelRenderer::ModelRenderer(const std::string Name, const std::vector<Vertex3D>& VertexList, const std::vector<uint32_t>& IndexList, const InstancingDataStruct& instanceData, std::shared_ptr<Material> material, const glm::vec3& position, const glm::vec3& rotation) :
	GameObject3D(Name, position, rotation)
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

ModelRenderer::ModelRenderer(const std::string Name, const std::vector<Vertex3D>& VertexList, const std::vector<uint32_t>& IndexList, const InstancingDataStruct& instanceData, std::shared_ptr<Material> material, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) :
	GameObject3D(Name, position, rotation, scale)
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
	modelLoader.MeshType = MeshTypeEnum::kPolygon;
	
	model = std::make_shared<Model>(Model(modelLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::string& FilePath, const glm::vec3& position) : GameObject3D(Name, position)
{
	ModelLoader modelLoader = {};
	modelLoader.FilePath = FilePath;
	modelLoader.ParentGameObjectID = GameObjectID;
	modelLoader.GameObjectTransform = GetGameObjectMatrix();
	modelLoader.MeshType = MeshTypeEnum::kPolygon;

	model = std::make_shared<Model>(Model(modelLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::string& FilePath, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, position, rotation)
{
	ModelLoader modelLoader = {};
	modelLoader.FilePath = FilePath;
	modelLoader.ParentGameObjectID = GameObjectID;
	modelLoader.GameObjectTransform = GetGameObjectMatrix();
	modelLoader.MeshType = MeshTypeEnum::kPolygon;

	model = std::make_shared<Model>(Model(modelLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::string& FilePath, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject3D(Name, position, rotation, scale)
{
	ModelLoader modelLoader = {};
	modelLoader.FilePath = FilePath;
	modelLoader.ParentGameObjectID = GameObjectID;
	modelLoader.GameObjectTransform = GetGameObjectMatrix();
	modelLoader.MeshType = MeshTypeEnum::kPolygon;

	model = std::make_shared<Model>(Model(modelLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::string& FilePath, const InstancingDataStruct& instanceData) : GameObject3D(Name)
{
	ModelLoader modelLoader = {};
	modelLoader.FilePath = FilePath;
	modelLoader.ParentGameObjectID = GameObjectID;
	modelLoader.GameObjectTransform = GetGameObjectMatrix();
	modelLoader.instanceData = instanceData;
	modelLoader.MeshType = MeshTypeEnum::kPolygon;

	model = std::make_shared<Model>(Model(modelLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::string& FilePath, const InstancingDataStruct& instanceData, const glm::vec3& position) : GameObject3D(Name, position)
{
	ModelLoader modelLoader = {};
	modelLoader.FilePath = FilePath;
	modelLoader.ParentGameObjectID = GameObjectID;
	modelLoader.GameObjectTransform = GetGameObjectMatrix();
	modelLoader.instanceData = instanceData;
	modelLoader.MeshType = MeshTypeEnum::kPolygon;

	model = std::make_shared<Model>(Model(modelLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::string& FilePath, const InstancingDataStruct& instanceData, const glm::vec3& position, const glm::vec3& rotation) : GameObject3D(Name, position, rotation)
{
	ModelLoader modelLoader = {};
	modelLoader.FilePath = FilePath;
	modelLoader.ParentGameObjectID = GameObjectID;
	modelLoader.GameObjectTransform = GetGameObjectMatrix();
	modelLoader.instanceData = instanceData;
	modelLoader.MeshType = MeshTypeEnum::kPolygon;

	model = std::make_shared<Model>(Model(modelLoader));
}

ModelRenderer::ModelRenderer(const std::string Name, const std::string& FilePath, const InstancingDataStruct& instanceData, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : GameObject3D(Name, position, rotation, scale)
{
	ModelLoader modelLoader = {};
	modelLoader.FilePath = FilePath;
	modelLoader.ParentGameObjectID = GameObjectID;
	modelLoader.GameObjectTransform = GetGameObjectMatrix();
	modelLoader.instanceData = instanceData;
	modelLoader.MeshType = MeshTypeEnum::kPolygon;

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