Vertex RasterVertexBuilder()
{
	Vertex vertex;
	vertex.Position = FragPos;
	vertex.UV = UV;
	vertex.Normal = Normal;
	vertex.Tangant = Tangent;
	vertex.BiTangant = BiTangent;
	vertex.Color = Color;
	return vertex;
}