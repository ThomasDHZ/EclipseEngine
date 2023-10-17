struct MaterialProperties
{
    float3 Albedo;
    float Metallic;
    float Roughness;
    float AmbientOcclusion;
    float3 Emission;
    float Alpha;
    float3 CheckPixelForPaletteSwapColor;
    float3 PaletteSwapPixelColor;

    uint AlbedoMap;
    uint MetallicRoughnessMap;
    uint MetallicMap;
    uint RoughnessMap;
    uint AmbientOcclusionMap;
    uint NormalMap;
    uint DepthMap;
    uint AlphaMap;
    uint EmissionMap;
    uint HeightMap;
    uint CheckPixelForPaletteSwapColorMap;
    uint PaletteSwapPixelColorMap;
};