Vertex RasterVertexBuilder()
{
	Vertex vertex;
	vertex.Position = FragPos;
	vertex.UV = UV;
	vertex.Normal = Normal;
	vertex.Tangant = Tangent;
	vertex.BiTangant = BiTangent;
	vertex.Color = Color;

	vertex.UV = vertex.UV + meshBuffer[sceneData.MeshIndex].meshProperties.UVOffset;
	vertex.UV *= meshBuffer[sceneData.MeshIndex].meshProperties.UVScale;

	if (meshBuffer[sceneData.MeshIndex].meshProperties.UVFlip.y == 1.0f)
	{
		vertex.UV.y = 1.0f - vertex.UV.y;
	}
	if (meshBuffer[sceneData.MeshIndex].meshProperties.UVFlip.x == 1.0f)
	{
		vertex.UV.x = 1.0f - vertex.UV.x;
	}

	return vertex;
}