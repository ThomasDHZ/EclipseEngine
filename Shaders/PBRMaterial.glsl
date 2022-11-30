struct PBRMaterial
{
	vec3 Albedo;
	float Matallic;
	float Roughness;
	float AmbientOcclusion;
	vec3 Emission;
	float Alpha;
};

PBRMaterial BuildPBRMaterial(MaterialProperties properties, vec2 UV)
{
	PBRMaterial material;

	vec3 material.albedo = pow(properties.Albedo, vec3(2.2f));
	if (material.AlbedoMapID != 0)
	{
		material.albedo = pow(texture(TextureMap[properties.AlbedoMapID], UV).rgb, vec3(2.2f));
	}

	float metallic = properties.Matallic;
	if (material.MetallicMapID != 0)
	{
		material.metallic = texture(TextureMap[properties.MetallicMapID], UV).r;
	}

	float roughness = properties.Roughness;
	if (material.RoughnessMapID != 0)
	{
		material.roughness = texture(TextureMap[properties.RoughnessMapID], UV).r;
	}

	float material.ao = properties.AmbientOcclusion;
	if (material.AmbientOcclusionMapID != 0)
	{
		material.ao = texture(TextureMap[properties.AmbientOcclusionMapID], UV).r;
	}

	vec3 material.emission = properties.Emission;
	if (material.EmissionMapID != 0)
	{
		material.emission = texture(TextureMap[properties.EmissionMapID], UV).rgb;
	}

	return material;
}