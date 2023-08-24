Vertex2D SpriteVertexBuilder()
{
    Vertex2D vertex;
    vertex.Position = FragPos;
    vertex.UV = UV;
    vertex.Color = Color;
    vertex.UV += meshBuffer[sceneData.MeshIndex].meshProperties.UVOffset;
    vertex.UV *= meshBuffer[sceneData.MeshIndex].meshProperties.UVScale;
    if (meshBuffer[sceneData.MeshIndex].meshProperties.UVFlip.y == 1.0)
    {
        vertex.UV.y = 1.0 - vertex.UV.y;
    }
    if (meshBuffer[sceneData.MeshIndex].meshProperties.UVFlip.x == 1.0)
    {
        vertex.UV.x = 1.0 - vertex.UV.x;
    }
    return vertex;
}
