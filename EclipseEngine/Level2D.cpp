#include "Level2D.h"
#include "File.h"
#include <fstream>
#include "GLTFSceneManager.h"

Level2D::Level2D()
{
}

Level2D::Level2D(const std::string& levelPath, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelID)
{
	LoadLevel(levelPath, GameObjectMatrix, ModelMatrix, gameObjectID, modelID);
}

Level2D::Level2D(const std::string& levelName, glm::ivec2 tileSizeInPixels, glm::ivec2 levelBounds, std::string tileSetPath, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelID)
{
	LevelName = levelName;
	TileSetPath = tileSetPath;
	LevelBounds = levelBounds;

	std::vector<std::shared_ptr<Material>> materialList;

	LevelMaterial = std::make_shared<Material>(Material("MetalManTiles"));
	LevelMaterial->SetAlbedoMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile(TileSetPath), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB))));
	materialList.emplace_back(LevelMaterial);
	GLTFSceneManager::AddMaterial(LevelMaterial);

	
	auto LevelMaterial2 = std::make_shared<Material>(Material("MetalManTiles"));
	LevelMaterial2->SetAlbedoMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile("/texture/MegaMan_Alpha.png"), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB))));
	materialList.emplace_back(LevelMaterial2);
	GLTFSceneManager::AddMaterial(LevelMaterial2);

	LevelSizeInPixels = glm::ivec2(materialList[0]->GetAlbedoMap()->GetWidth(), materialList[0]->GetAlbedoMap()->GetHeight());
	TileSizeInPixels = tileSizeInPixels;
	LevelTileCount = glm::ivec2(LevelSizeInPixels.x / TileSizeInPixels.x, LevelSizeInPixels.y / TileSizeInPixels.y);
	TileUVSize = glm::vec2(1.0f / (float)LevelTileCount.x, 1.0f / (float)LevelTileCount.y);

	std::vector<glm::ivec2> animation;
	animation.emplace_back(glm::ivec2(0, 0));
	animation.emplace_back(glm::ivec2(1, 0));
	animation.emplace_back(glm::ivec2(0, 0));

	std::vector<glm::ivec2> animation2;
	animation2.emplace_back(glm::ivec2(2, 0));
	animation2.emplace_back(glm::ivec2(3, 0));
	animation2.emplace_back(glm::ivec2(4, 0));
	animation2.emplace_back(glm::ivec2(3, 0));

	std::vector<Sprite> TileList;
	//for (int x = 0; x < levelBounds.x; x++)
	//{
	//	for (int y = 0; y < levelBounds.y; y++)
	//	{
	//		TileList.emplace_back(LevelTile(glm::ivec2(0, 0), glm::ivec2(0, 0), TileUVSize, false));
	//	}
	//}


	//TileList.emplace_back(LevelTile(glm::ivec2(6, 12), glm::ivec2(7, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(7, 12), glm::ivec2(8, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(8, 12), glm::ivec2(8, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(9, 12), glm::ivec2(8, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(10, 12), glm::ivec2(8, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(11, 12), glm::ivec2(8, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(12, 12), glm::ivec2(8, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(13, 12), glm::ivec2(8, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(14, 12), glm::ivec2(8, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(15, 12), glm::ivec2(9, 0), TileUVSize));

	//TileList.emplace_back(LevelTile(glm::ivec2(15, 11), glm::ivec2(2, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(15, 10), glm::ivec2(2, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(15, 9), glm::ivec2(2, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(15, 8), glm::ivec2(2, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(15, 7), glm::ivec2(2, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(15, 6), glm::ivec2(2, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(15, 5), glm::ivec2(2, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(15, 4), glm::ivec2(2, 0), TileUVSize));

	//TileList.emplace_back(LevelTile(glm::ivec2(16, 11), glm::ivec2(3, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(16, 10), glm::ivec2(3, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(16, 9), glm::ivec2(3, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(16, 8), glm::ivec2(3, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(16, 7), glm::ivec2(3, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(16, 6), glm::ivec2(3, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(16, 5), glm::ivec2(3, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(16, 4), glm::ivec2(3, 0), TileUVSize));

	//TileList.emplace_back(LevelTile(glm::ivec2(0, 3), glm::ivec2(13, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(1, 3), glm::ivec2(13, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(2, 3), glm::ivec2(13, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(3, 3), glm::ivec2(13, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(4, 3), glm::ivec2(13, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(5, 3), glm::ivec2(13, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(6, 3), glm::ivec2(13, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(7, 3), glm::ivec2(13, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(8, 3), glm::ivec2(13, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(9, 3), glm::ivec2(13, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(10, 3), glm::ivec2(13, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(11, 3), glm::ivec2(13, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(12, 3), glm::ivec2(13, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(13, 3), glm::ivec2(13, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(14, 3), glm::ivec2(13, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(15, 3), glm::ivec2(17, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(16, 3), glm::ivec2(3, 0), TileUVSize));

	//TileList.emplace_back(LevelTile(glm::ivec2(0, 2), glm::ivec2(3, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(1, 2), glm::ivec2(4, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(2, 2), glm::ivec2(3, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(3, 2), glm::ivec2(4, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(4, 2), glm::ivec2(3, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(5, 2), glm::ivec2(4, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(6, 2), glm::ivec2(3, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(7, 2), glm::ivec2(4, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(8, 2), glm::ivec2(3, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(9, 2), glm::ivec2(4, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(10, 2), glm::ivec2(3, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(11, 2), glm::ivec2(4, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(12, 2), glm::ivec2(3, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(13, 2), glm::ivec2(4, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(14, 2), glm::ivec2(3, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(15, 2), glm::ivec2(4, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(16, 2), glm::ivec2(3, 0), TileUVSize));

	//TileList.emplace_back(LevelTile(glm::ivec2(0, 1), glm::ivec2(5, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(1, 1), glm::ivec2(6, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(2, 1), glm::ivec2(5, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(3, 1), glm::ivec2(6, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(4, 1), glm::ivec2(5, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(5, 1), glm::ivec2(6, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(6, 1), glm::ivec2(5, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(7, 1), glm::ivec2(6, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(8, 1), glm::ivec2(5, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(9, 1), glm::ivec2(6, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(10, 1), glm::ivec2(5, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(11, 1), glm::ivec2(6, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(12, 1), glm::ivec2(5, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(13, 1), glm::ivec2(6, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(14, 1), glm::ivec2(5, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(15, 1), glm::ivec2(6, 0), TileUVSize));
	//TileList.emplace_back(LevelTile(glm::ivec2(16, 1), glm::ivec2(5, 0), TileUVSize));
	TileList.emplace_back(Sprite(LevelMaterial, glm::ivec2(3, 1), glm::ivec2(0, 0), TileUVSize, animation, 1000));
	TileList.emplace_back(Sprite(LevelMaterial2, glm::ivec2(0, 1), glm::ivec2(0, 0), TileUVSize, animation2, 1000));

	//for (unsigned int x = 0; x < LevelBounds.x - 1; x++)
	//{
	//	for (unsigned int y = 0; y < LevelBounds.y - 1; y++)
	//	{

	//		const float LefttSideUV = TileList[(y*x)+ x].GetTilePositionOffset().x;
	//		const float RightSideUV = TileList[(y * x) + x].GetTilePositionOffset().x + TileList[(y * x) + x].GetTileUVs().x;

	//		const unsigned int VertexCount = VertexList.size();
	//		const Vertex2D BottomLeftVertex =  { { x * TileSize.x,                 y * TileSize.y},                {RightSideUV,  0.0f},         {1.0f, 0.0f, 0.0f, 1.0f} };
	//		const Vertex2D BottomRightVertex = { {(x * TileSize.x) + TileSize.x,   y * TileSize.y},                {LefttSideUV,  0.0f},         {0.0f, 1.0f, 0.0f, 1.0f} };
	//		const Vertex2D TopRightVertex =    { {(x * TileSize.x) + TileSize.x,  (y * TileSize.y) + TileSize.y},  {LefttSideUV,  TileUVSize.y}, {0.0f, 0.0f, 1.0f, 1.0f} };
	//		const Vertex2D TopLeftVertex =     { { x * TileSize.x,                (y * TileSize.y) + TileSize.y},  {RightSideUV,  TileUVSize.y}, {1.0f, 1.0f, 0.0f, 1.0f} };

	//		VertexList.emplace_back(BottomLeftVertex);
	//		VertexList.emplace_back(BottomRightVertex);
	//		VertexList.emplace_back(TopRightVertex);
	//		VertexList.emplace_back(TopLeftVertex);

	//		IndexList.emplace_back(VertexCount);
	//		IndexList.emplace_back(VertexCount + 1);
	//		IndexList.emplace_back(VertexCount + 2);
	//		IndexList.emplace_back(VertexCount + 2);
	//		IndexList.emplace_back(VertexCount + 3);
	//		IndexList.emplace_back(VertexCount);

	//		std::vector<Vertex2D> CollisionVertices
	//		{
	//			BottomLeftVertex,
	//			BottomRightVertex,
	//			TopRightVertex,
	//			TopLeftVertex
	//		};

	//		std::vector<uint32_t> CollisionIndices
	//		{
	//			0, 1, 2, 2, 3, 0
	//		};
	//	}
	//}

	//VertexBuffer = std::make_shared<VulkanBuffer>(VulkanBuffer(VertexList.data(), VertexList.size() * sizeof(Vertex2D), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	//IndexBuffer = std::make_shared<VulkanBuffer>(VulkanBuffer(IndexList.data(), IndexList.size() * sizeof(uint32_t), VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));

	//GLTFMeshLoader3D meshLoader;
	//meshLoader.VertexBuffer = VertexBuffer;
	//meshLoader.IndexBuffer = IndexBuffer;

	//MeshStartUp(meshLoader);

DrawLevelLayerMeshList.emplace_back(SpriteLayerMesh(levelName, TileList, materialList, GameObjectMatrix, ModelMatrix, gameObjectID, modelID));
	
	//SaveLevel();
}

Level2D::~Level2D()
{
}

void Level2D::LoadLevel(const std::string& levelPath, glm::mat4& GameObjectMatrix, glm::mat4& ModelMatrix, uint32_t gameObjectID, uint32_t modelID)
{
	std::string SceneInfo;
	std::ifstream SceneFile;
	SceneFile.open(File::OpenFile(levelPath));
	if (SceneFile.is_open())
	{
		while (!SceneFile.eof())
		{
			getline(SceneFile, SceneInfo);
		}
	}
	else std::cout << "Unable to open file";
	SceneFile.close();

	nlohmann::json json = nlohmann::json::parse(SceneInfo);

	JsonConverter::from_json(json["LevelName"], LevelName);
	JsonConverter::from_json(json["TileSizeInPixels"], TileSizeInPixels);
	JsonConverter::from_json(json["levelBounds"], LevelBounds);
	JsonConverter::from_json(json["TileSetPath"], TileSetPath);

	std::vector<std::shared_ptr<Material>> materialList;

	LevelMaterial = std::make_shared<Material>(Material("MetalManTiles"));
	LevelMaterial->SetAlbedoMap(GLTFSceneManager::LoadTexture2D(std::make_shared<Texture2D>(Texture2D(VulkanRenderer::OpenFile(TileSetPath), TextureTypeEnum::kAlbedoTextureMap, VK_FORMAT_R8G8B8A8_SRGB))));
	materialList.emplace_back(LevelMaterial);
	GLTFSceneManager::AddMaterial(LevelMaterial);

	LevelSizeInPixels = glm::ivec2(LevelMaterial->GetAlbedoMap()->GetWidth(), LevelMaterial->GetAlbedoMap()->GetHeight());
	LevelTileCount = glm::ivec2(LevelSizeInPixels.x / TileSizeInPixels.x, LevelSizeInPixels.y / TileSizeInPixels.y);
	TileUVSize = glm::vec2(1.0f / (float)LevelTileCount.x, 1.0f / (float)LevelTileCount.y);

	glm::ivec2 TilePositionOffset;
	glm::ivec2 TileOffset;
	for (int x = 0; x < json["Layer"].size(); x++)
	{
		std::vector<LevelTile> TileList;
		for (int y = 0; y < json["Layer"][x]["Tiles"].size(); y++)
		{
			JsonConverter::from_json(json["Layer"][x]["Tiles"][y]["TilePositionOffset"], TilePositionOffset);
			JsonConverter::from_json(json["Layer"][x]["Tiles"][y]["TileOffset"], TileOffset);

			TileList.emplace_back(LevelTile(TilePositionOffset, TileOffset, TileUVSize));
		}
		//DrawLevelLayerMeshList.emplace_back(SpriteLayerMesh(LevelName, TileList, materialList, GameObjectMatrix, ModelMatrix, gameObjectID, modelID));
	}
}

void Level2D::Update(float DeltaTime, const glm::mat4& GameObjectMatrix, const glm::mat4& ModelMatrix)
{
	for (auto& layerMesh : DrawLevelLayerMeshList)
	{
		layerMesh.Update(DeltaTime, GameObjectMatrix, ModelMatrix);
	}
}

void Level2D::SaveLevel()
{
	/*nlohmann::json json;

	JsonConverter::to_json(json["LevelName"], LevelName);
	JsonConverter::to_json(json["TileSizeInPixels"], TileSizeInPixels);
	JsonConverter::to_json(json["levelBounds"], LevelBounds);
	JsonConverter::to_json(json["TileSetPath"], TileSetPath);
	for (int x = 0; x < DrawLevelLayerMeshList.size(); x++)
	{
		for(int y = 0; y < DrawLevelLayerMeshList[x].GetLevelTile().size(); y++)
		{
			DrawLevelLayerMeshList[x].GetLevelTile()[y].SaveTile(json["Layer"][x]["Tiles"][y]);
		}
	}

	std::string fileName = "/Levels/" + LevelName;
	std::string renderPassName = File::OpenFile(fileName);
	renderPassName.append(".txt");

	std::ofstream pipelineFile = std::ofstream(renderPassName);
	pipelineFile << json;
	pipelineFile.close();*/
}

void Level2D::DrawLevelLayer(VkCommandBuffer& commandBuffer, VkDescriptorSet descriptorSet, VkPipelineLayout shaderPipelineLayout, SceneProperties& constBuffer)
{
	for (auto& layerMesh : DrawLevelLayerMeshList)
	{
		layerMesh.DrawLevelLayer(commandBuffer, descriptorSet, shaderPipelineLayout, constBuffer);
	}
}
