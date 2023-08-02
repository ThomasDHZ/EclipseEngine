#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable
#extension GL_EXT_multiview : enable

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in vec3 aColor;

layout(location = 0) out vec3 FragPos;
layout(location = 1) out vec2 UV;
layout(location = 2) out vec3 Normal;
layout(location = 3) out vec3 Tangent;
layout(location = 4) out vec3 BiTangent;
layout(location = 5) out vec3 Color;

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
layout(binding = 11) uniform CubeMapViewSampler 
{
    mat4 CubeMapFaceMatrix[6];
} cubeMapViewSampler;

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

void main() {

// if(gl_VertexIndex == 0)
//	{
//		debugPrintfEXT(": %i \n", sceneData.MeshIndex);
//	}
    mat4 MeshTransform = transformBuffer[sceneData.MeshIndex].transform;
    FragPos = vec3(MeshTransform * vec4(inPosition.xyz, 1.0));    
    Color = aColor;
    UV = aUV;
    Normal = mat3(MeshTransform) * aNormal;
	Tangent = aTangent;
	BiTangent = aBitangent;
    gl_Position = cubeMapViewSampler.CubeMapFaceMatrix[gl_ViewIndex] *               
                  MeshTransform * 
                  vec4(inPosition, 1.0);
}