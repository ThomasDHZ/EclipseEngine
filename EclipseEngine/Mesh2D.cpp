//#include "Mesh2D.h"
//#include "EngineMath.h"
//
//
//Mesh2D::Mesh2D() : Mesh(MeshTypeEnum::kMeshSprite, MeshSubTypeEnum::kNormal, -1)
//{
//}
//
//Mesh2D::Mesh2D(MeshSubTypeEnum meshSubType, uint64_t parentGameObjectID) : Mesh(MeshTypeEnum::kMeshSprite, meshSubType, parentGameObjectID)
//{
//	MeshStartUp();
//}
//
////Mesh2D::Mesh2D(std::shared_ptr<Material> materialPtr, MeshSubTypeEnum meshSubType, uint64_t parentGameObjectID) : Mesh(MeshTypeEnum::kMeshSprite, meshSubType, parentGameObjectID)
////{
////	MeshStartUp(materialPtr);
////}
//
//Mesh2D::~Mesh2D()
//{
//}
//
//void Mesh2D::MeshStartUp()
//{
//	GenerateID();
//	GenerateColorID();
//
//	VertexList = SpriteVertexList;
//	IndexList = SpriteIndexList;
//
//	//material = MaterialManager::GetDefaultMaterial();
//	MeshType = MeshTypeEnum::kMeshPolygon;
//	ParentModelID = -1;
//	ParentGameObjectID = -1;
//	VertexCount = VertexList.size();
//	IndexCount = IndexList.size();
//
//	VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(Vertex2D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
//	IndexBuffer.CreateBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
//}
//
////void Mesh2D::MeshStartUp(std::shared_ptr<Material> materialPtr)
////{
////	GenerateID();
////	GenerateColorID();
////
////	VertexList = SpriteVertexList;
////	IndexList = SpriteIndexList;
////
////	ParentModelID = -1;
////	VertexCount = VertexList.size();
////	IndexCount = IndexList.size();
////	material = materialPtr;
////
////	VertexBuffer.CreateBuffer(VertexList.data(), VertexList.size() * sizeof(Vertex2D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
////	IndexBuffer.CreateBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
////}
//
//void Mesh2D::Update(const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
//{
//	GameObjectTransformMatrix = GameObjectMatrix;
//	ModelTransformMatrix = ModelMatrix;
//
//	glm::mat4 TransformMatrix = glm::mat4(1.0f);
//	TransformMatrix = glm::translate(TransformMatrix, glm::vec3(MeshPosition.x, MeshPosition.y, 0.0f));
//	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(MeshRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
//	TransformMatrix = glm::rotate(TransformMatrix, glm::radians(MeshRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
//	TransformMatrix = glm::scale(TransformMatrix, glm::vec3(MeshScale.x, MeshScale.y, 1.0f));
//
//	if (SelectedMesh)
//	{
//		meshProperties.SelectedObjectBufferIndex = 1;
//	}
//	else
//	{
//		meshProperties.SelectedObjectBufferIndex = 0;
//	}
//
//	meshProperties.MeshTransform = GameObjectTransformMatrix * ModelTransformMatrix * TransformMatrix;
//	//meshProperties.MaterialBufferIndex = material->GetMaterialBufferIndex();
//	MeshPropertiesBuffer.Update(meshProperties);
//}
//
//void Mesh2D::SetPosition(float x, float y)
//{
//	MeshPosition = glm::vec2(x, y);
//}
//
//void Mesh2D::SetPosition(const glm::vec2& position)
//{
//	MeshPosition = position;
//}
//
//void Mesh2D::SetDepth(float Depth)
//{
//	MeshDepth = Depth;
//}
//
//void Mesh2D::SetRotation(float x, float y)
//{
//	MeshRotation = glm::vec2(x, y);
//}
//
//void Mesh2D::SetRotation(const glm::vec2& rotation)
//{
//	MeshRotation = rotation;
//}
//
//void Mesh2D::SetScale(float x, float y)
//{
//	MeshScale = glm::vec2(x, y);
//}
//
//void Mesh2D::SetScale(const glm::vec2& Scale)
//{
//	MeshScale = Scale;
//}
