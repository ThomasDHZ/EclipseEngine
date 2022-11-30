#include "PBRBindingLayout.glsl"

struct PBRMaterial
{
	vec3 Albedo;
	float Metallic;
	float Roughness;
	float AmbientOcclusion;
	vec3 Emission;
	float Alpha;

	uint NormalMapID;
	uint DepthMapID;
};

PBRMaterial BuildPBRMaterial(MaterialProperties properties, vec2 UV)
{
	PBRMaterial material;

	material.Albedo = pow(properties.Albedo, vec3(2.2f));
	if (properties.AlbedoMapID != 0)
	{
		material.Albedo = pow(texture(TextureMap[properties.AlbedoMapID], UV).rgb, vec3(2.2f));
	}

	material.Metallic = properties.Matallic;
	if (properties.MetallicMapID != 0)
	{
		material.Metallic = texture(TextureMap[properties.MetallicMapID], UV).r;
	}

	material.Roughness = properties.Roughness;
	if (properties.RoughnessMapID != 0)
	{
		material.Roughness = texture(TextureMap[properties.RoughnessMapID], UV).r;
	}

	material.AmbientOcclusion = properties.AmbientOcclusion;
	if (properties.AmbientOcclusionMapID != 0)
	{
		material.AmbientOcclusion = texture(TextureMap[properties.AmbientOcclusionMapID], UV).r;
	}

	material.Emission = properties.Emission;
	if (properties.EmissionMapID != 0)
	{
		material.Emission = texture(TextureMap[properties.EmissionMapID], UV).rgb;
	}

	if (texture(TextureMap[properties.AlphaMapID], UV).r == 0.0f ||
		texture(TextureMap[properties.AlbedoMapID], UV).a == 0.0f)
	{
		discard;
	}

	material.NormalMapID = properties.NormalMapID;
	material.DepthMapID = properties.DepthMapID;

	return material;
}