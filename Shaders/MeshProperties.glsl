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

struct MeshProperties
{
	MaterialProperties materialProperties;
	mat4 MeshTransform;
	mat4 MeshReflectionMatrix[6];
	vec2 UVOffset;
	vec2 UVScale;
	vec2 UVFlip;
	int SelectedMesh;
	float heightScale;
	float minLayers;
	float maxLayers;
};