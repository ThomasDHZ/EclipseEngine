#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable
#include "MeshProperties.glsl"
layout(push_constant) uniform SceneData
{
    uint MeshIndex;
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
    float Timer;
} sceneData;

struct RayPayload {
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

layout(location = 0) rayPayloadInEXT RayPayload rayHitInfo;
hitAttributeEXT vec2 attribs;

layout(binding = 0, set = 0) uniform accelerationStructureEXT topLevelAS;
layout(binding = 1, set = 0, rgba8) uniform image2D RayTracedTexture;
layout(binding = 2, scalar) buffer Vertices { Vertex v[]; } vertices[];
layout(binding = 3) buffer Indices { uint i[]; } indices[];
layout(binding = 4) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 5) uniform sampler2D TextureMap[];

Vertex BuildVertexInfo()
{
	Vertex vertex;
	const ivec3 index = ivec3(indices[gl_InstanceCustomIndexEXT].i[3 * gl_PrimitiveID],
						      indices[gl_InstanceCustomIndexEXT].i[3 * gl_PrimitiveID + 1],
							  indices[gl_InstanceCustomIndexEXT].i[3 * gl_PrimitiveID + 2]);

	const Vertex v0 = vertices[gl_InstanceCustomIndexEXT].v[index.x];
	const Vertex v1 = vertices[gl_InstanceCustomIndexEXT].v[index.y];
	const Vertex v2 = vertices[gl_InstanceCustomIndexEXT].v[index.z];

	const vec3 barycentricCoords = vec3(1.0f - attribs.x - attribs.y, attribs.x, attribs.y);

	vertex.Position = v0.Position * barycentricCoords.x + v1.Position * barycentricCoords.y + v2.Position * barycentricCoords.z;
	vertex.Position = vec3(meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform * vec4(vertex.Position, 1.0));

	vertex.UV = v0.UV * barycentricCoords.x + v1.UV * barycentricCoords.y + v2.UV * barycentricCoords.z;

	vertex.Color = v0.Color * barycentricCoords.x + v1.Color * barycentricCoords.y + v2.Color * barycentricCoords.z;

	return vertex;
}

void main()
{
	Vertex vertex = BuildVertexInfo();

	   uint diffuse = meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.materialProperties.DiffuseMapID;
   uint alpha = meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.materialProperties.AlphaMapID;
    rayHitInfo.color = texture(TextureMap[diffuse], vertex.UV).rgb;
}