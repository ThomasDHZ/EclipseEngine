#version 460
#extension GL_EXT_ray_tracing : enable
#extension GL_EXT_scalar_block_layout : enable

struct RayHitInfo
{
	vec3 color;
	uint seed;
    int reflectCount;
};

#include "RTXPayload.glsl"
#include "Vertex.glsl"
#include "MeshProperties.glsl"
#include "MaterialProperties.glsl"
#include "GLTFLights.glsl"

layout(location = 0) rayPayloadInEXT RayHitInfo rayPayload;

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
layout(binding = 15) uniform samplerCube CubeMap;

void main()
{
	rayPayload.color = texture(CubeMap, gl_WorldRayDirectionEXT).rgb;
}