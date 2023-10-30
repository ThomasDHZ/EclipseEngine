#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable

#include "RTXPayload.glsl"
#include "Vertex.glsl"
#include "MeshProperties.glsl"
#include "MaterialProperties.glsl"
#include "GLTFLights.glsl"

layout(location = 0) rayPayloadInEXT RayPayload rayHitInfo;
layout(location = 1) rayPayloadEXT bool shadowed;
hitAttributeEXT vec2 attribs;

layout(binding = 0, set = 0) uniform accelerationStructureEXT topLevelAS;
layout(binding = 1, set = 0, rgba8) uniform image2D RayTracedTexture;
layout(binding = 2, scalar) buffer Vertices { Vertex v[]; } vertices[];
layout(binding = 3) buffer Indices { uint i[]; } indices[];
layout(binding = 4) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 5) buffer TransformBuffer { mat4 transform; } transformBuffer[];
layout(binding = 6) buffer MaterialPropertiesBuffer { MaterialProperties materialProperties; } materialBuffer[];
layout(binding = 7) uniform sampler2D TextureMap[];
layout(binding = 8) uniform sampler2D BRDFMap;
layout(binding = 9) uniform samplerCube IrradianceMap;
layout(binding = 10) uniform samplerCube PrefilterMap;
layout(binding = 11) buffer SunLightBuffer { SunLight sunLight; } SULight[];
layout(binding = 12) buffer DirectionalLightBuffer { DirectionalLight directionalLight; } DLight[];
layout(binding = 13) buffer PointLightBuffer { PointLight pointLight; } PLight[];
layout(binding = 14) buffer SpotLightBuffer { SpotLight spotLight; } SLight[];


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

	vertex.pos = v0.pos * barycentricCoords.x + v1.pos * barycentricCoords.y + v2.pos * barycentricCoords.z;
	vertex.pos = vec3(meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.ModelTransform * meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform * vec4(vertex.pos, 1.0));

	vertex.normal = v0.normal * barycentricCoords.x + v1.normal * barycentricCoords.y + v2.normal * barycentricCoords.z;

	vertex.uv = v0.uv * barycentricCoords.x + v1.uv * barycentricCoords.y + v2.uv * barycentricCoords.z;
	vertex.uv += meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.UVOffset;

	vertex.tangent = v0.tangent * barycentricCoords.x + v1.tangent * barycentricCoords.y + v2.tangent * barycentricCoords.z;
	vertex.BiTangant = v0.BiTangant * barycentricCoords.x + v1.BiTangant * barycentricCoords.y + v2.BiTangant * barycentricCoords.z;

	return vertex;
}

void main()
{		
    const uint materialID = meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MaterialBufferIndex;
    MaterialProperties material = materialBuffer[materialID].materialProperties;

    rayHitInfo.color = vec3(1.0f, 1.0f, 0.0f);
}