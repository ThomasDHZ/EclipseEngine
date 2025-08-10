#version 460
#extension GL_EXT_ray_tracing : enable
#extension GL_EXT_scalar_block_layout : enable

#include "VertexLayout.glsl"
#include "MeshProperties.glsl"
#include "MaterialProperties.glsl"
#include "Lights.glsl"
#include "SceneData.glsl"
#include "RTXPayload.glsl"

struct RayHitInfo
{
	vec3 color;
	uint seed;
    int reflectCount;
};

layout(location = 0) rayPayloadInEXT RayHitInfo rayPayload;

layout(binding = 0, set = 0) uniform accelerationStructureEXT topLevelAS;
layout(binding = 1, set = 0, rgba8) uniform image2D RayTracedTexture;
layout(binding = 2, scalar) buffer Vertices { Vertex v[]; } vertices[];
layout(binding = 3) buffer Indices { uint i[]; } indices[];
layout(binding = 4) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 5) buffer MaterialPropertiesBuffer { MaterialProperties materialProperties; } materialBuffer[];
layout(binding = 6) buffer DirectionalLightBuffer { DirectionalLight directionalLight; } DLight[];
layout(binding = 7) buffer PointLightBuffer { PointLight pointLight; } PLight[];
layout(binding = 8) buffer SpotLightBuffer { SpotLight spotLight; } SLight[];
layout(binding = 9) uniform sampler2D TextureMap[];
layout(binding = 10) uniform samplerCube CubeMap;

void main()
{
	rayPayload.color = texture(CubeMap, gl_WorldRayDirectionEXT).rgb;
}