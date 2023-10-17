struct MaterialProperties
{
	vec3 Albedo;
	float Metallic;
	float Roughness;
	float AmbientOcclusion;
	vec3 Emission;
	float Alpha;
	vec3 CheckPixelForPaletteSwapColor;
	vec3 PaletteSwapPixelColor;

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