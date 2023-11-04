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
			debugPrintfEXT(": %f, %f, %f, %f \n", meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform[3][0], meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform[3][1], meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform[3][2], meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform[3][3]);
	vertex.Normal = v0.Normal * barycentricCoords.x + v1.Normal * barycentricCoords.y + v2.Normal * barycentricCoords.z;

	vertex.UV = v0.UV * barycentricCoords.x + v1.UV * barycentricCoords.y + v2.UV * barycentricCoords.z;
	vertex.UV += meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.UVOffset;

	vertex.Tangant = v0.Tangant * barycentricCoords.x + v1.Tangant * barycentricCoords.y + v2.Tangant * barycentricCoords.z;
	vertex.BiTangant = v0.BiTangant * barycentricCoords.x + v1.BiTangant * barycentricCoords.y + v2.BiTangant * barycentricCoords.z;

	return vertex;
}

PBRMaterial BuildPBRMaterial(MaterialProperties properties, vec2 UV)
{
	PBRMaterial material;

	material.Albedo = pow(properties.Albedo, vec3(2.2f));
	if (properties.AlbedoMap != 0)
	{
		material.Albedo = pow(texture(TextureMap[properties.AlbedoMap], UV).rgb, vec3(2.2f));
	}

	material.Metallic = properties.Metallic;
	if (properties.MetallicMap != 0)
	{
		material.Metallic = texture(TextureMap[properties.MetallicMap], UV).r;
	}

	material.Roughness = properties.Roughness;
	if (properties.RoughnessMap != 0)
	{
		material.Roughness = texture(TextureMap[properties.RoughnessMap], UV).r;
	}

	material.AmbientOcclusion = properties.AmbientOcclusion;
	if (properties.AmbientOcclusionMap != 0)
	{
		material.AmbientOcclusion = texture(TextureMap[properties.AmbientOcclusionMap], UV).r;
	}

	material.Emission = properties.Emission;
	if (properties.EmissionMap != 0)
	{
		material.Emission = texture(TextureMap[properties.EmissionMap], UV).rgb;
	}

	if (texture(TextureMap[properties.AlphaMap], UV).r == 0.0f ||
		texture(TextureMap[properties.AlbedoMap], UV).a == 0.0f)
	{
		//discard;
	}

	material.NormalMapID = properties.NormalMap;
	material.DepthMapID = properties.DepthMap;

	return material;
}

void main()
{		

    Vertex vertex = BuildVertexInfo();
    const uint materialID = meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MaterialBufferIndex;
    MaterialProperties material = materialBuffer[materialID].materialProperties;
    PBRMaterial pbrMaterial = BuildPBRMaterial(material, vertex.UV);

	const vec3 barycentricCoords = vec3(1.0f - attribs.x - attribs.y, attribs.x, attribs.y);

    rayHitInfo.color = pow(texture(TextureMap[material.AlbedoMap], vertex.UV).rgb, vec3(2.2f));


	

	debugPrintfEXT(": %i \n", meshBuffer[2].meshProperties.MaterialBufferIndex);

}