struct MeshProperties
{
    uint VertexBufferIndex;
    uint IndexBufferIndex;
    uint MaterialBufferIndex;
    uint AlbedoMapIndex;
    uint MetallicRoughnessMapIndex;
    uint AmbientOcclusionMapIndex;
    uint NormalMapIndex;
    uint DepthMapIndex;
    uint AlphaMapIndex;
    uint EmissionMapIndex;
    uint SkyBoxIndex;
    float4x4 MeshTransform;
    float2 UVOffset;
    float2 UVScale;
    float2 UVFlip;
    int SelectedMesh;
    float heightScale;
    float minLayers;
    float maxLayers;
};