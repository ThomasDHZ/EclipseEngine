#include "Model.h"
#include "Math.h"

uint64_t Model::ModelIDCounter = 0;

Model::Model()
{
}

Model::Model(const std::string& FilePath, uint64_t GameObjectID)
{
	GenerateID();
	LoadModel(FilePath, GameObjectID);
	VulkanRenderer::UpdateTLAS = true;
}

Model::Model(std::shared_ptr<Mesh>& mesh, uint64_t GameObjectID)
{
	GenerateID();
	ParentGameObjectID = GameObjectID;
	AddMesh(mesh);
	VulkanRenderer::UpdateTLAS = true;
}

Model::Model(std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, uint64_t GameObjectID)
{
	GenerateID();
	ParentGameObjectID = GameObjectID;
	AddMesh(VertexList, IndexList);
	VulkanRenderer::UpdateTLAS = true;
}

Model::Model(std::vector<Vertex3D>& VertexList, std::vector<uint32_t>& IndexList, std::shared_ptr<Material> materialPtr, uint64_t GameObjectID)
{
	GenerateID();
	ParentGameObjectID = GameObjectID;
	AddMesh(VertexList, IndexList);
	VulkanRenderer::UpdateTLAS = true;
}

Model::~Model()
{

}

void Model::GenerateID()
{
	ModelIDCounter++;
	ModelID = ModelIDCounter;
}

void Model::LoadMesh(const std::string& FilePath, aiNode* node, const aiScene* scene)
{
	uint32_t TotalVertex = 0;
	uint32_t TotalIndex = 0;

	for (uint32_t x = 0; x < node->mNumMeshes; x++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[x]];

		auto vertices = LoadVertices(mesh);
		auto indices = LoadIndices(mesh);
		auto boneWeights = LoadBoneWeights(mesh, vertices);

		LoadBones(scene->mRootNode, mesh, vertices);

		TotalVertex += vertices.size();
		TotalIndex += indices.size();

		for (auto nodeMap : NodeMapList)
		{
			if (nodeMap.NodeString == node->mName.C_Str())
			{
				//MeshList.back()->MeshID = nodeMap.NodeID;
			}
		}


		MeshLoadingInfo meshLoader;
		meshLoader.ModelID = ModelID;
		meshLoader.vertices = vertices;
		meshLoader.indices = indices;
		meshLoader.BoneCount = BoneList.size();
		meshLoader.BoneWeightList = boneWeights;
		meshLoader.MeshTransform = Converter::AssimpToGLMMatrixConverter(node->mTransformation);
		meshLoader.meshType = MeshTypeEnum::kPolygon;

		/*if (LoadMaterial)
		{*/
			auto material = LoadMaterial(FilePath, mesh, scene);
			meshLoader.materialPtr = material;
	/*	}*/

		AddMesh(meshLoader);
	}

	for (uint32_t x = 0; x < node->mNumChildren; x++)
	{
		LoadMesh(FilePath, node->mChildren[x], scene);
	}
}

std::vector<Vertex3D> Model::LoadVertices(aiMesh* mesh)
{
	std::vector<Vertex3D> VertexList;

	for (int x = 0; x < mesh->mNumVertices; x++)
	{
		Vertex3D vertex;
		vertex.Position = glm::vec3{ mesh->mVertices[x].x, mesh->mVertices[x].y, mesh->mVertices[x].z };
		vertex.Normal = glm::vec3{ mesh->mNormals[x].x, mesh->mNormals[x].y, mesh->mNormals[x].z };
		//vertex.Color = glm::vec3{ mesh->mColors[x]->r, mesh->mColors[x]->g, mesh->mColors[x]->b };

		if (mesh->mTangents)
		{
			vertex.Tangant = glm::vec4{ mesh->mTangents[x].x, mesh->mTangents[x].y, mesh->mTangents[x].z, 0.0f };
			vertex.BiTangant = glm::vec4{ mesh->mBitangents[x].x, mesh->mBitangents[x].y, mesh->mBitangents[x].z, 0.0f };
		}
		else
		{
			vertex.Tangant = glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f };
			vertex.BiTangant = glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f };
		}
		if (mesh->mTextureCoords[0])
		{
			vertex.UV = glm::vec2{ mesh->mTextureCoords[0][x].x, mesh->mTextureCoords[0][x].y };
		}
		else
		{
			vertex.UV = glm::vec2{ 0.0f, 0.0f };
		}

		VertexList.emplace_back(vertex);
	}

	return VertexList;
}

std::vector<uint32_t> Model::LoadIndices(aiMesh* mesh)
{
	std::vector<uint32_t> IndexList;

	for (int x = 0; x < mesh->mNumFaces; x++)
	{
		aiFace Faces = mesh->mFaces[x];
		for (int y = 0; y < Faces.mNumIndices; y++)
		{
			IndexList.emplace_back(Faces.mIndices[y]);
		}
	}

	return IndexList;
}

void Model::LoadBones(const aiNode* RootNode, const aiMesh* mesh, std::vector<Vertex3D>& VertexList)
{
	for (int x = 0; x < mesh->mNumBones; x++)
	{
		bool Exists = false;
		auto node = RootNode->FindNode(mesh->mBones[x]->mName.data);
		//BoneList.emplace_back(std::make_shared<Bone>(mesh->mBones[x]->mName.data, x, Converter::AssimpToGLMMatrixConverter(mesh->mBones[x]->mOffsetMatrix)));
	}
}

std::vector<MeshBoneWeights> Model::LoadBoneWeights(aiMesh* mesh, std::vector<Vertex3D>& VertexList)
{
	std::vector<MeshBoneWeights> BoneWeightList(VertexList.size());
	for (int x = 0; x < mesh->mNumBones; x++)
	{
		std::vector<unsigned int> AffectedVertices;
		AffectedVertices.resize(BoneWeightList.size(), 0);

		aiBone* bone = mesh->mBones[x];
		for (int y = 0; y < bone->mNumWeights; y++)
		{
			unsigned int vertexID = bone->mWeights[y].mVertexId;
			float weight = bone->mWeights[y].mWeight;

			for (unsigned int z = 0; z < MAX_BONE_VERTEX_COUNT; z++)
			{
				if (BoneWeightList[vertexID].BoneWeights[z] == 0.0)
				{
					BoneWeightList[vertexID].BoneID[z] = x;
					BoneWeightList[vertexID].BoneWeights[z] = weight;
				}
			}
		}
	}

	for (int x = 0; x < BoneWeightList.size(); x++)
	{
		float Weight = BoneWeightList[x].BoneWeights.x +
			BoneWeightList[x].BoneWeights.y +
			BoneWeightList[x].BoneWeights.z +
			BoneWeightList[x].BoneWeights.w;
		if (Weight != 1.0f)
		{
			BoneWeightList[x].BoneWeights = glm::vec4(
				BoneWeightList[x].BoneWeights.x / Weight,
				BoneWeightList[x].BoneWeights.y / Weight,
				BoneWeightList[x].BoneWeights.z / Weight,
				BoneWeightList[x].BoneWeights.w / Weight);
		}
	}

	return BoneWeightList;
}

std::shared_ptr<Material> Model::LoadMaterial(const std::string& FilePath, aiMesh* mesh, const aiScene* scene)
{
	MaterialProperties MaterialInfo;

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	auto directory = FilePath.substr(0, FilePath.find_last_of('/')) + '/';

	aiColor3D color(0.f, 0.f, 0.f);
	material->Get(AI_MATKEY_COLOR_AMBIENT, color);
	MaterialInfo.Ambient.r = color.r;
	MaterialInfo.Ambient.g = color.g;
	MaterialInfo.Ambient.b = color.b;

	material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	MaterialInfo.Diffuse.r = color.r;
	MaterialInfo.Diffuse.g = color.g;
	MaterialInfo.Diffuse.b = color.b;

	material->Get(AI_MATKEY_COLOR_SPECULAR, color);
	MaterialInfo.Specular.r = color.r;
	MaterialInfo.Specular.g = color.g;
	MaterialInfo.Specular.b = color.b;

	if (AI_SUCCESS != aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &MaterialInfo.Shininess) ||
		MaterialInfo.Shininess == 0.0f)
	{
		MaterialInfo.Shininess = 0.1f;
	}

	if (AI_SUCCESS == aiGetMaterialFloat(material, AI_MATKEY_REFLECTIVITY, &MaterialInfo.Reflectivness))
	{
		MaterialInfo.Reflectivness = 0.0f;
	}

	aiString TextureLocation;
	for (unsigned int x = 0; x < material->GetTextureCount(aiTextureType_DIFFUSE); x++)
	{
		material->GetTexture(aiTextureType_DIFFUSE, x, &TextureLocation);

		MaterialInfo.DiffuseMap = TextureManager::LoadTexture2D(directory + TextureLocation.C_Str(), TextureTypeEnum::kDiffuseTextureMap, VK_FORMAT_R8G8B8A8_SRGB);
		MaterialInfo.AlbedoMap = TextureManager::LoadTexture2D(directory + TextureLocation.C_Str(), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB);
	}

	for (unsigned int x = 0; x < material->GetTextureCount(aiTextureType_METALNESS); x++)
	{
		material->GetTexture(aiTextureType_METALNESS, x, &TextureLocation);
		MaterialInfo.MetallicMap = TextureManager::LoadTexture2D(directory + TextureLocation.C_Str(), TextureTypeEnum::kMetallicTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
	}

	for (unsigned int x = 0; x < material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS); x++)
	{
		material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, x, &TextureLocation);
		MaterialInfo.RoughnessMap = TextureManager::LoadTexture2D(directory + TextureLocation.C_Str(), TextureTypeEnum::kRoughnessTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
	}

	for (unsigned int x = 0; x < material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION); x++)
	{
		material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, x, &TextureLocation);
		MaterialInfo.AmbientOcclusionMap = TextureManager::LoadTexture2D(directory + TextureLocation.C_Str(), TextureTypeEnum::kAmbientOcclusionTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
	}

	for (unsigned int x = 0; x < material->GetTextureCount(aiTextureType_SPECULAR); x++)
	{
		material->GetTexture(aiTextureType_SPECULAR, x, &TextureLocation);
		MaterialInfo.SpecularMap = TextureManager::LoadTexture2D(directory + TextureLocation.C_Str(), TextureTypeEnum::kSpecularTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
	}

	for (unsigned int x = 0; x < material->GetTextureCount(aiTextureType_NORMALS); x++)
	{
		material->GetTexture(aiTextureType_NORMALS, x, &TextureLocation);
		MaterialInfo.NormalMap = TextureManager::LoadTexture2D(directory + TextureLocation.C_Str(), TextureTypeEnum::kNormalTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
	}

	for (unsigned int x = 0; x < material->GetTextureCount(aiTextureType_HEIGHT); x++)
	{
		material->GetTexture(aiTextureType_HEIGHT, x, &TextureLocation);
		MaterialInfo.DepthMap = TextureManager::LoadTexture2D(directory + TextureLocation.C_Str(), TextureTypeEnum::kDepthTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
	}

	for (unsigned int x = 0; x < material->GetTextureCount(aiTextureType_OPACITY); x++)
	{
		material->GetTexture(aiTextureType_OPACITY, x, &TextureLocation);
		MaterialInfo.AlphaMap = TextureManager::LoadTexture2D(directory + TextureLocation.C_Str(), TextureTypeEnum::kAlphaTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
	}

	for (unsigned int x = 0; x < material->GetTextureCount(aiTextureType_EMISSIVE); x++)
	{
		material->GetTexture(aiTextureType_EMISSIVE, x, &TextureLocation);
		MaterialInfo.EmissionMap = TextureManager::LoadTexture2D(directory + TextureLocation.C_Str(), TextureTypeEnum::kEmissionTextureMap, VK_FORMAT_R8G8B8A8_UNORM);
	}

	uint64_t materialID = MaterialManager::AddMaterial(material->GetName().C_Str(), MaterialTypeEnum::kMaterialBlinnPhong, MaterialInfo);
	return MaterialManager::GetMaterialByID(materialID);
}

void Model::LoadModel(const std::string& FilePath, uint64_t GameObjectID)
{
	ModelFilePath = FilePath;

	Assimp::Importer ModelImporter;

	const aiScene* Scene = ModelImporter.ReadFile(FilePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
	{
		std::cout << "Error loading model: " << ModelImporter.GetErrorString() << std::endl;
		return;
	}

	ParentGameObjectID = GameObjectID;
	//glm::mat4 GlobalInverseTransformMatrix = AssimpToGLMMatrixConverter(Scene->mRootNode->mTransformation.Inverse());
	//LoadNodeTree(Scene->mRootNode);
	//LoadAnimations(Scene);
	LoadMesh(FilePath, Scene->mRootNode, Scene);
	VulkanRenderer::UpdateTLAS = true;
	//if (AnimationList.size() > 0)
	//{
	//	AnimatedModel = true;
	//	AnimationPlayer = AnimationPlayer3D(BoneList, NodeMapList, GlobalInverseTransformMatrix, AnimationList[0]);
	//	AnimationRenderer = AnimatorCompute(MeshList[0]);
	//}

	//ModelTransform = Converter::AssimpToGLMMatrixConverter(Scene->mRootNode->mTransformation.Inverse());
}

void Model::AddMesh(std::shared_ptr<Mesh> mesh)
{
	mesh->SetParentModel(ModelID);
	mesh->SetParentGameObjectID(ParentGameObjectID);
	MeshList.emplace_back(mesh);
}

void Model::AddMesh(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh3D>(Mesh3D(vertices, indices));
	
	mesh->SetParentModel(ModelID);
	mesh->SetParentGameObjectID(ParentGameObjectID);
	MeshList.emplace_back(mesh);
}

void Model::AddMesh(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices, std::shared_ptr<Material> materialPtr)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh3D>(Mesh3D(vertices, indices, materialPtr));
	
	mesh->SetParentModel(ModelID);
	mesh->SetParentGameObjectID(ParentGameObjectID);
	MeshList.emplace_back(mesh);
}

void Model::AddMesh(MeshLoadingInfo& meshLoader)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh3D>(Mesh3D(meshLoader));

	mesh->SetParentModel(ModelID);
	mesh->SetParentGameObjectID(ParentGameObjectID);
	MeshList.emplace_back(mesh);
}

void Model::DeleteMesh(std::shared_ptr<Mesh> mesh)
{
}

Model::Model(nlohmann::json& json, uint64_t GameObjectID)
{
	GenerateID();
	ParentGameObjectID = GameObjectID;
	from_json(json, GameObjectID);
}

void Model::RemoveMesh(std::shared_ptr<Mesh> mesh)
{
}

void Model::Update(const glm::mat4& GameObjectMatrix)
{
	const glm::mat4 LastTransform = ModelTransform;
	ModelTransform = glm::mat4(1.0f);
	ModelTransform = glm::translate(ModelTransform, ModelPosition);
	ModelTransform = glm::rotate(ModelTransform, glm::radians(ModelRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	ModelTransform = glm::rotate(ModelTransform, glm::radians(ModelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	ModelTransform = glm::rotate(ModelTransform, glm::radians(ModelRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	ModelTransform = glm::scale(ModelTransform, ModelScale);

	if (LastTransform != ModelTransform)
	{
		VulkanRenderer::UpdateTLAS = true;
	}

	if (BoneList.size() > 0)
	{
		//AnimationPlayer.Update();
		for (auto& mesh : MeshList)
		{
			mesh->Update(GameObjectMatrix, ModelTransform, BoneList);
		}
	}
	else
	{
		for (auto& mesh : MeshList)
		{
			mesh->Update(GameObjectMatrix, ModelTransform);
		}
	}
}

void Model::UpdateMeshTopLevelAccelerationStructure(std::vector<VkAccelerationStructureInstanceKHR>& AccelerationStructureInstanceList)
{
	for (auto& mesh : GetMeshList())
	{
		glm::mat4 GLMTransformMatrix2 = ModelTransform;
		VkTransformMatrixKHR transformMatrix = EngineMath::GLMToVkTransformMatrix(GLMTransformMatrix2);

		VkAccelerationStructureInstanceKHR AccelerationStructureInstance{};
		AccelerationStructureInstance.transform = transformMatrix;
		AccelerationStructureInstance.instanceCustomIndex = mesh->GetMeshBufferIndex();
		AccelerationStructureInstance.mask = 0xFF;
		AccelerationStructureInstance.instanceShaderBindingTableRecordOffset = 0;
		AccelerationStructureInstance.flags = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
		AccelerationStructureInstance.accelerationStructureReference = mesh->GetBLASBufferDeviceAddress();
		AccelerationStructureInstanceList.emplace_back(AccelerationStructureInstance);
	}
}

void Model::Destroy()
{
	for (auto& mesh : MeshList)
	{
		mesh->Destroy();
	}
}

bool Model::DoesMeshExistInModel(std::shared_ptr<Mesh> mesh)
{
	for (auto& meshInModel : MeshList)
	{
		if (meshInModel->GetMeshID() == mesh->GetMeshID())
		{
			return true;
		}
	}
	return false;
}

void Model::SetModelPosition(float x, float y, float z)
{
	ModelPosition = glm::vec3(x, y, z);
}

void Model::SetModelRotation(float x, float y, float z)
{
	ModelRotation = glm::vec3(x, y, z);
}

void Model::SetModelScale(float x, float y, float z)
{
	ModelScale = glm::vec3(x, y, z);
}

void Model::SetModelPosition(glm::vec3 modelPosition)
{
	ModelPosition = modelPosition;
}

void Model::SetModelRotation(glm::vec3 modelRotation)
{
	ModelRotation = modelRotation;
}

void Model::SetModelScale(glm::vec3 modelScale)
{
	ModelScale = modelScale;
}

glm::mat4 Model::TransposeModelMatrix()
{
	return glm::transpose(ModelTransform);
}
