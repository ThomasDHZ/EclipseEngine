struct MaterialProperties
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	float Shininess;
	float Reflectivness;

	vec3 Albedo;
	float Matallic;
	float Roughness;
	float AmbientOcclusion;
	vec3 Emission;
	float Alpha;

	uint DiffuseMapID;
	uint SpecularMapID;
	uint AlbedoMapID;
	uint MetallicMapID;
	uint RoughnessMapID;
	uint AmbientOcclusionMapID;
	uint NormalMapID;
	uint DepthMapID;
	uint AlphaMapID;
	uint EmissionMapID;
};