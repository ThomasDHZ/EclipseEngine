struct MaterialProperties
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	float Shininess;
	float Reflectivness;

	uint DiffuseMapID;
	uint SpecularMapID;
	uint NormalMapID;
	uint DepthMapID;
	uint AlphaMapID;
	uint EmissionMapID;
	uint ShadowMapID;
};

struct MeshProperties
{
	MaterialProperties materialProperties;
	mat4 MeshTransform;
};