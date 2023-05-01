#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec2 UV;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec3 Tangent;
layout(location = 4) in vec3 BiTangent;
layout(location = 5) in vec3 Color;
layout(location = 6) in flat int MaterialID;

layout(location = 0) out vec4 outColor;

struct Vertex
{
	vec3 Position;
	float PositionPadding;
	vec3 Normal;
	float NormalPadding;
	vec2 UV;
	vec2 UVPadding;
	vec3 Tangant;
	float TangantPadding;
	vec3 BiTangant;
	float BiTangentPadding;
	vec3 Color;
	float ColorPadding;
};

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
	mat4 MeshTransform;
	vec2 UVOffset;
	vec2 UVScale;
	vec2 UVFlip;
	int SelectedMesh;
	float heightScale;
	float minLayers;
	float maxLayers;
};

struct MaterialProperties
{
	vec3 Albedo;
	float Metallic;
	float Roughness;
	float AmbientOcclusion;
	vec3 Emission;
	float Alpha;

	uint AlbedoMap;
	uint MetallicRoughnessMap;
	uint AmbientOcclusionMap;
	uint NormalMap;
	uint DepthMap;
	uint AlphaMap;
	uint EmissionMap;
};

struct SunLight
{
	vec3 diffuse;
	vec3 position;
	mat4 LightSpaceMatrix;
	float intensity;
};

struct DirectionalLight
{
	vec3 diffuse;
	vec3 direction;
	mat4 LightSpaceMatrix;
	float intensity;
};

struct PointLight
{
	vec3 diffuse;
	vec3 position;
	mat4 LightSpaceMatrix;
	float intensity;
	float radius;
};

struct SpotLight
{
	vec3 diffuse;
	vec3 position;
	vec3 direction;
	mat4 LightSpaceMatrix;
	float intensity;

    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    mat4 lightSpaceMatrix;
};

layout(push_constant) uniform SceneData
{
    uint MeshIndex;
    uint PrimitiveIndex;
	uint MaterialIndex;
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
    vec3 MeshColorID;
    vec3 AmbientLight;
    uint SunLightCount;
    uint DirectionalLightCount;
    uint PointLightCount;
    uint SpotLightCount;
    float Timer;
    float PBRMaxMipLevel;
    uint frame;
    int MaxRefeflectCount;
} sceneData;

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

layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 1) buffer TransformBuffer { mat4 transform; } transformBuffer[];
layout(binding = 2) buffer MaterialPropertiesBuffer { MaterialProperties materialProperties; } materialBuffer[];
layout(binding = 3) uniform sampler2D TextureMap[];
layout(binding = 4) uniform sampler2D BRDFMap;
layout(binding = 5) uniform samplerCube IrradianceMap;
layout(binding = 6) uniform samplerCube PrefilterMap;
layout(binding = 7) buffer SunLightBuffer { SunLight sunLight; } SULight[];
layout(binding = 8) buffer DirectionalLightBuffer { DirectionalLight directionalLight; } DLight[];
layout(binding = 9) buffer PointLightBuffer { PointLight pointLight; } PLight[];
layout(binding = 10) buffer SpotLightBuffer { SpotLight spotLight; } SLight[];

const float PI = 3.14159265359;

Vertex RasterVertexBuilder();
MaterialProperties BuildPBRMaterial(vec2 UV);

void main()
{ 
    Vertex vertex = RasterVertexBuilder();
    MaterialProperties material = BuildPBRMaterial(vertex.UV);

    outColor = vec4(material.Albedo, 1.0f);
}

Vertex RasterVertexBuilder()
{
	Vertex vertex;
	vertex.Position = FragPos;
	vertex.UV = UV;
	vertex.Normal = Normal;
	vertex.Tangant = Tangent;
	vertex.BiTangant = BiTangent;
	vertex.Color = Color;

	vertex.UV = vertex.UV + meshBuffer[sceneData.MeshIndex].meshProperties.UVOffset;
	vertex.UV *= meshBuffer[sceneData.MeshIndex].meshProperties.UVScale;

	if (meshBuffer[sceneData.MeshIndex].meshProperties.UVFlip.y == 1.0f)
	{
		vertex.UV.y = 1.0f - vertex.UV.y;
	}
	if (meshBuffer[sceneData.MeshIndex].meshProperties.UVFlip.x == 1.0f)
	{
		vertex.UV.x = 1.0f - vertex.UV.x;
	}

	return vertex;
}

MaterialProperties BuildPBRMaterial(vec2 UV)
{
	MaterialProperties material = materialBuffer[sceneData.MaterialIndex].materialProperties;

  //  material.Albedo = material.Albedo;
//	if (material.AlbedoMap != 0)
//	{
	    material.Albedo = texture(TextureMap[material.AlbedoMap], UV).rgb;
	//}

   // material.Metallic = material.Metallic;
	//if (material.MetallicRoughnessMap != 0)
	//{
	    material.Metallic = texture(TextureMap[material.MetallicRoughnessMap], UV).b;
	//}

   // material.Roughness = material.Roughness;
	//if (material.MetallicRoughnessMap != 0)
	//{
	    material.Roughness = texture(TextureMap[material.MetallicRoughnessMap], UV).g;
//	}

//material.AmbientOcclusion = material.AmbientOcclusion;
	//if (material.AmbientOcclusionMap != 0)
	//{
		material.AmbientOcclusion = texture(TextureMap[material.AmbientOcclusionMap], UV).r;
	//}

	//material.Emission = material.Emission;
	//if (material.EmissionMap != 0)
	//{
		material.Emission = texture(TextureMap[material.EmissionMap], UV).rgb;
	//}

    if(texture(TextureMap[material.AlbedoMap], UV).a == 0.0f)
	{
		discard;
	}

    return material;
}