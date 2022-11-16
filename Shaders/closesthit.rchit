#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable

#include "VertexLayout.glsl"
#include "MeshProperties.glsl"
#include "Lights.glsl"
#include "SceneData.glsl"
#include "RTXPayload.glsl"

layout(location = 0) rayPayloadInEXT RayPayload rayHitInfo;
layout(location = 1) rayPayloadEXT bool shadowed;
hitAttributeEXT vec2 attribs;

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

vec3 CalcNormalDirLight(Vertex vertex, MaterialProperties material, mat3 TBN, vec3 normal, vec2 uv, int index);
vec2 ParallaxMapping(Vertex vertex, MaterialProperties material, vec2 texCoords, vec3 viewDir);

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
	vertex.Normal = v0.Normal * barycentricCoords.x + v1.Normal * barycentricCoords.y + v2.Normal * barycentricCoords.z;
    vertex.Tangant = v0.Tangant * barycentricCoords.x + v1.Tangant * barycentricCoords.y + v2.Tangant * barycentricCoords.z;
	vertex.BiTangant = v0.BiTangant * barycentricCoords.x + v1.BiTangant * barycentricCoords.y + v2.BiTangant * barycentricCoords.z;
	vertex.Color = v0.Color * barycentricCoords.x + v1.Color * barycentricCoords.y + v2.Color * barycentricCoords.z;

	return vertex;
}

void main() 
{
	Vertex vertex = BuildVertexInfo();
	const uint materialID = meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MaterialBufferIndex;
    MaterialProperties material = materialBuffer[materialID].materialProperties;

   vec2 FinalUV = vertex.UV + meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.UVOffset;
   FinalUV *= meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.UVScale;

   if(meshBuffer[gl_PrimitiveID].meshProperties.UVFlip.y == 1.0f)
   {
        FinalUV.y = 1.0f - FinalUV.y;
   }
   if(meshBuffer[gl_PrimitiveID].meshProperties.UVFlip.x == 1.0f)
   {
        FinalUV.x = 1.0f - FinalUV.x;
   }

   vec3 T = normalize(mat3(meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform) * vec3(vertex.Tangant));
   vec3 B = normalize(mat3(meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform) * vec3(vertex.BiTangant));
   vec3 N = normalize(mat3(meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform) * vertex.Normal);
   mat3 TBN = transpose(mat3(T, B, N));
   
   vec3 normal = vertex.Normal;
   vec3 ViewPos  = sceneData.CameraPos;
   vec3 FragPos  = vertex.Position;
   vec3 viewDir = normalize(ViewPos - FragPos);
    if(material.NormalMapID != 0)
    {
        ViewPos  = TBN * sceneData.CameraPos;
        FragPos  = TBN * vertex.Position;
        if(material.DepthMapID != 0)
        {
            FinalUV = ParallaxMapping(vertex, material, FinalUV,  viewDir);       
        }
        normal = texture(TextureMap[material.NormalMapID], FinalUV).rgb;
        normal = normalize(normal * 2.0 - 1.0);
    }

   vec3 baseColor = vec3(0.0f);
   for(int x = 0; x < sceneData.DirectionalLightCount; x++)
   {
        baseColor += CalcNormalDirLight(vertex, material, TBN, normal, FinalUV, x);
   }
//        for(int x = 0; x < sceneData.PointLightCount; x++)
//     {
//        baseColor += CalcNormalPointLight(vertex, material, TBN, normal, FinalUV, x);   
//     }
//       for(int x = 0; x < sceneData.SpotLightCount; x++)
//       {
//            baseColor += CalcNormalSpotLight(vertex, material, TBN, normal, FinalUV, x);   
//       }

    vec3 result = vec3(0.0f);
    if(material.Reflectivness > 0.0f &&
       rayHitInfo.reflectCount != sceneData.MaxRefeflectCount)
    {
        vec3 hitPos = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_RayTmaxEXT;
        vec3 origin   = hitPos.xyz + normal * 0.001f;
        vec3 rayDir   = reflect(gl_WorldRayDirectionEXT, normal);

        rayHitInfo.reflectCount++;
        traceRayEXT(topLevelAS, gl_RayFlagsNoneEXT, 0xff, 0, 0, 0, origin, 0.001f, rayDir, 10000.0f, 0);
		result = mix(baseColor, rayHitInfo.color, material.Reflectivness); 
    }
    else
	{
        result = baseColor;
        rayHitInfo.reflectCount = sceneData.MaxRefeflectCount + 1;
	}

    rayHitInfo.color = result;
}

vec3 CalcNormalDirLight(Vertex vertex, MaterialProperties material, mat3 TBN, vec3 normal, vec2 uv, int index)
{
    vec3 ViewPos  = sceneData.CameraPos;
    vec3 FragPos  = vertex.Position;
    vec3 LightPos = DLight[index].directionalLight.direction;
    if(material.NormalMapID != 0)
    {
        ViewPos  = TBN * sceneData.CameraPos;
        FragPos  = TBN * vertex.Position;
        LightPos = TBN * DLight[index].directionalLight.direction;
    }
	vec3 ViewDir = normalize(ViewPos - FragPos);

    const vec3 lightDir = normalize(-LightPos);
    const float diff = max(dot(normal, lightDir), 0.0);

    const vec3 halfwayDir = normalize(lightDir + ViewDir);
    const float spec = pow(max(dot(normal, halfwayDir), 0.0), material.Shininess);

    vec3 ambient = sceneData.AmbientLight * material.Diffuse.rgb;
    vec3 diffuse = DLight[index].directionalLight.diffuse * diff * material.Diffuse.rgb;
    vec3 specular = DLight[index].directionalLight.specular * spec * material.Specular;
    if (material.DiffuseMapID != 0)
    {
        ambient = sceneData.AmbientLight * vec3(texture(TextureMap[material.DiffuseMapID], uv));
        diffuse = DLight[index].directionalLight.diffuse * diff * vec3(texture(TextureMap[material.DiffuseMapID], uv));
    }
    if (material.SpecularMapID != 0)
    {
        specular = DLight[index].directionalLight.specular * spec * vec3(texture(TextureMap[material.SpecularMapID], uv));
    }
    vec3 result = ambient;

  float tmin = 0.001f;
  float tmax = length(LightPos - FragPos);
  vec3 origin = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_HitTEXT;

  shadowed = false;  
  traceRayEXT(topLevelAS, gl_RayFlagsSkipClosestHitShaderEXT, 0xFF, 1, 0, 1, origin, tmin, lightDir, tmax, 1);
  if (shadowed) 
  {
     result += (diffuse + specular);
  }

  return result;
}

vec2 ParallaxMapping(Vertex vertex, MaterialProperties material, vec2 texCoords, vec3 viewDir)
{
    const float heightScale = meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.heightScale;
    const float minLayers = meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.minLayers;
    const float maxLayers = meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.maxLayers;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;

    viewDir.y = -viewDir.y;
    vec2 P = viewDir.xy / viewDir.z * heightScale;
    vec2 deltaTexCoords = P / numLayers;

    vec2  currentTexCoords = texCoords;
    float currentDepthMapValue = texture(TextureMap[material.DepthMapID], currentTexCoords).r;

    while (currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(TextureMap[material.DepthMapID], currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(TextureMap[material.DepthMapID], prevTexCoords).r - currentLayerDepth + layerDepth;

    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

