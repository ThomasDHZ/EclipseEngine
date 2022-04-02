#version 460
#extension GL_EXT_ray_tracing : enable
#extension GL_EXT_scalar_block_layout : enable
#include "MeshProperties.glsl"
layout(push_constant) uniform SceneData
{
    uint MeshIndex;
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
    ivec3 MeshColorID;
    float Timer;
} sceneData;

struct RayHitInfo
{
	vec3 color;
	uint seed;
	vec3 normal;
    int reflectCount;
    bool hitGeo;
};

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

layout(location = 0) rayPayloadInEXT RayHitInfo rayPayload;
layout(binding = 0, set = 0) uniform accelerationStructureEXT topLevelAS;
layout(binding = 1, set = 0, rgba8) uniform image2D RayTracedTexture;
layout(binding = 2, scalar) buffer Vertices { Vertex v[]; } vertices[];
layout(binding = 3) buffer Indices { uint i[]; } indices[];
layout(binding = 4) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 5) uniform sampler2D TextureMap[];

void main()
{
	rayPayload.color = vec3(0.0f, 0.0f, 1.0f);
	rayPayload.normal = vec3(0.0f);
}