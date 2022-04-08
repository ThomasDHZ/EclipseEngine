#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable

#include "MeshProperties.glsl"
#include "Lights.glsl"

layout(push_constant) uniform SceneData
{
    uint MeshIndex;
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
    vec3 MeshColorID;
    uint DirectionalLightCount;
    uint PointLightCount;
    uint SpotLightCount;
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
//layout(binding = 5) buffer DirectionalLightBuffer { DirectionalLight directionalLight; } DLight[];
//layout(binding = 6) buffer PointLightBuffer { PointLight pointLight; } PLight[];
//layout(binding = 7) buffer SpotLightBuffer { SpotLight spotLight; } SLight[];
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
//vec3 CalcNormalDirLight(MaterialProperties material, mat3 TBN, vec3 normal, vec2 uv, int index);
//vec2 ParallaxMapping(MaterialProperties material, vec2 texCoords, vec3 viewDir);

void main()
{
	Vertex vertex = BuildVertexInfo();

	   uint diffuse = meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.materialProperties.DiffuseMapID;
   uint alpha = meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.materialProperties.AlphaMapID;
    rayHitInfo.color = texture(TextureMap[diffuse], vertex.UV).rgb;
}

//vec3 CalcNormalDirLight(MaterialProperties material, mat3 TBN, vec3 normal, vec2 uv, int index)
//{
//    vec3 LightPos = DLight[index].directionalLight.direction;
//    vec3 ViewPos = sceneData.CameraPos;
//    vec3 FragPos2 = FragPos;
//    if (material.NormalMapID != 0)
//    {
//        LightPos = TBN * DLight[index].directionalLight.direction;
//        ViewPos = TBN * sceneData.CameraPos;
//        FragPos2 = TBN * FragPos;
//    }
//    vec3 ViewDir = normalize(ViewPos - FragPos2);
//
//    const vec3 lightDir = normalize(-LightPos);
//    const float diff = max(dot(normal, lightDir), 0.0);
//
//    const vec3 halfwayDir = normalize(lightDir + ViewDir);
//    const float spec = pow(max(dot(normal, halfwayDir), 0.0), material.Shininess);
//
//    vec3 ambient = DLight[index].directionalLight.ambient * material.Diffuse.rgb;
//    vec3 diffuse = DLight[index].directionalLight.diffuse * diff * material.Diffuse.rgb;
//    vec3 specular = DLight[index].directionalLight.specular * spec * material.Specular;
//    if (material.DiffuseMapID != 0)
//    {
//        ambient = DLight[index].directionalLight.ambient * vec3(texture(TextureMap[material.DiffuseMapID], uv));
//        diffuse = DLight[index].directionalLight.diffuse * diff * vec3(texture(TextureMap[material.DiffuseMapID], uv));
//    }
//    if (material.SpecularMapID != 0)
//    {
//        specular = DLight[index].directionalLight.specular * spec * vec3(texture(TextureMap[material.SpecularMapID], uv));
//    }
//
//    float LightDistance = length(LightPos - FragPos2);
//
////    vec4 LightSpace = (LightBiasMatrix *  DLight[index].directionalLight.lightSpaceMatrix * meshBuffer[Mesh.MeshIndex].meshProperties.ModelTransform * meshBuffer[Mesh.MeshIndex].meshProperties.MeshTransform) * vec4(FragPos, 1.0);
////    float shadow = filterPCF(LightSpace/ LightSpace.w, index);  
//    return (ambient + diffuse + specular);
//}
//
//vec2 ParallaxMapping(MaterialProperties material, vec2 texCoords, vec3 viewDir)
//{
//    const float heightScale = meshBuffer[sceneData.MeshIndex].meshProperties.heightScale;
//    const float minLayers = meshBuffer[sceneData.MeshIndex].meshProperties.minLayers;
//    const float maxLayers = meshBuffer[sceneData.MeshIndex].meshProperties.maxLayers;
//    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
//    float layerDepth = 1.0 / numLayers;
//    float currentLayerDepth = 0.0;
//
//    viewDir.y = -viewDir.y;
//    vec2 P = viewDir.xy / viewDir.z * heightScale;
//    vec2 deltaTexCoords = P / numLayers;
//
//    vec2  currentTexCoords = texCoords;
//    float currentDepthMapValue = texture(TextureMap[material.DepthMapID], currentTexCoords).r;
//
//    while (currentLayerDepth < currentDepthMapValue)
//    {
//        currentTexCoords -= deltaTexCoords;
//        currentDepthMapValue = texture(TextureMap[material.DepthMapID], currentTexCoords).r;
//        currentLayerDepth += layerDepth;
//    }
//
//    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;
//
//    float afterDepth = currentDepthMapValue - currentLayerDepth;
//    float beforeDepth = texture(TextureMap[material.DepthMapID], prevTexCoords).r - currentLayerDepth + layerDepth;
//
//    float weight = afterDepth / (afterDepth - beforeDepth);
//    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
//
//    return finalTexCoords;
//}