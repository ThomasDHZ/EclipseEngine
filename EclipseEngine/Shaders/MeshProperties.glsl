struct MaterialProperties
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	float Shininess;
	float Reflectivness;

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
	uint ShadowMapID;
};

struct MeshProperties
{
	mat4 MeshTransform;
	mat4 ModelTransform;
	MaterialProperties materialProperties;
};