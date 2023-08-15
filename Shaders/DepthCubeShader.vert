#version 460
#extension GL_EXT_multiview : require
#extension GL_EXT_nonuniform_qualifier : require

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

layout(set = 0, binding = 1, std430) buffer TransformBuffer
{
    mat4 transform;
} transformBuffer[];

layout(set = 0, binding = 0, std140) uniform ViewSampler
{
    mat4 CubeMapFaceMatrix[6];
} viewSampler[];

layout(set = 0, binding = 2, std430) buffer MeshPropertiesBuffer
{
    MeshProperties meshProperties;
} meshBuffer[1];

layout(set = 0, binding = 3, std430) buffer MaterialPropertiesBuffer
{
    MaterialProperties materialProperties;
} materialBuffer[1];

layout(push_constant) uniform SceneData
{
  uint MeshIndex;
  uint MaterialIndex;
  uint LightIndex;
} sceneData;


layout(location = 0) out vec3 FragPos;
layout(location = 0) in vec3 inPosition;
layout(location = 1) out vec2 UV;
layout(location = 2) in vec2 aUV;
layout(location = 1) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in vec3 aColor;

void main()
{
    mat4 MeshTransform = transformBuffer[sceneData.MeshIndex].transform;
    FragPos = vec3((MeshTransform * vec4(inPosition, 1.0)).xyz);
    UV = aUV;
    gl_Position = (viewSampler[sceneData.LightIndex].CubeMapFaceMatrix[gl_ViewIndex] * MeshTransform) * vec4(inPosition, 1.0);
}

