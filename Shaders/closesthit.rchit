#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable

#include "VertexLayout.glsl"
#include "MeshProperties.glsl"
#include "Lights.glsl"

layout(push_constant) uniform SceneData
{
    uint MeshIndex;
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
    vec3 MeshColorID;
    vec3 AmbientLight;
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
vec3 CalcNormalDirLight(Vertex vertex, MaterialProperties material, mat3 TBN, vec3 normal, vec2 uv, int index);
vec3 CalcNormalPointLight(Vertex vertex, MaterialProperties material, mat3 TBN,  vec3 normal, vec2 uv, int index);
vec3 CalcNormalSpotLight(Vertex vertex, MaterialProperties material, mat3 TBN,  vec3 normal, vec2 uv, int index);
vec2 ParallaxMapping(Vertex vertex, MaterialProperties material, vec2 texCoords, vec3 viewDir);

void main()
{
   const uint materialID = meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MaterialBufferIndex;
   MaterialProperties material = materialBuffer[materialID].materialProperties;

	Vertex vertex = BuildVertexInfo();
       vec2 FinalUV = vertex.UV + meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.UVOffset;
        FinalUV *= meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.UVScale;

//   if(texture(TextureMap[material.AlphaMapID], FinalUV).r == 0.0f ||
//      texture(TextureMap[material.DiffuseMapID], FinalUV).a == 0.0f)
//   {
//	 discard;
//   }
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
   vec3 FragPos2  = vertex.Position;
   vec3 viewDir = normalize(ViewPos - FragPos2);
    if(material.NormalMapID != 0)
    {
        ViewPos  = TBN * sceneData.CameraPos;
        FragPos2  = TBN * vertex.Position;
    }

    if(material.NormalMapID != 0)
    {
        if(material.DepthMapID != 0)
        {
            FinalUV = ParallaxMapping(vertex, material, FinalUV,  viewDir);       
//            if(FinalUV.x > 1.0 || FinalUV.y > 1.0 || FinalUV.x < 0.0 || FinalUV.y < 0.0)
//            {
//              discard;
//            }
        }
        normal = texture(TextureMap[material.NormalMapID], FinalUV).rgb;
        normal = normalize(normal * 2.0 - 1.0);
     }


   vec3 result = vec3(0.0f);
   for(int x = 0; x < sceneData.DirectionalLightCount; x++)
   {
        result += CalcNormalDirLight(vertex, material, TBN, normal, FinalUV, x);
   }
//        for(int x = 0; x < sceneData.PointLightCount; x++)
//     {
//        result += CalcNormalPointLight(vertex, material, TBN, normal, FinalUV, x);   
//     }
//       for(int x = 0; x < sceneData.SpotLightCount; x++)
//       {
//            result += CalcNormalSpotLight(vertex, material, TBN, normal, FinalUV, x);   
//       }
    rayHitInfo.color = result;
}

vec3 CalcNormalDirLight(Vertex vertex, MaterialProperties material, mat3 TBN, vec3 normal, vec2 uv, int index)
{
    vec3 LightPos = DLight[index].directionalLight.direction;
    vec3 ViewPos = sceneData.CameraPos;
    vec3 FragPos2 = vertex.Position;
    if (material.NormalMapID != 0)
    {
        LightPos = TBN * DLight[index].directionalLight.direction;
        ViewPos = TBN * sceneData.CameraPos;
        FragPos2 = TBN *  vertex.Position;;
    }
    vec3 ViewDir = normalize(ViewPos - FragPos2);

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

    float LightDistance = length(LightPos - FragPos2);
    vec3 result = ambient;

  float tmin = 0.001;
  float tmax = length(LightPos - FragPos2);
  vec3 origin = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_HitTEXT;
  shadowed = true;  
  traceRayEXT(topLevelAS, gl_RayFlagsSkipClosestHitShaderEXT, 0xFF, 1, 0, 1, origin, tmin, lightDir, tmax, 1);
  if (shadowed) 
  {
     result += (diffuse + specular);
  }

    return result;
}

vec3 CalcNormalPointLight(Vertex vertex, MaterialProperties material, mat3 TBN,   vec3 normal, vec2 uv, int index)
{
    vec3 LightPos = PLight[index].pointLight.position;
    vec3 ViewPos = sceneData.CameraPos;
    vec3 FragPos2 = vertex.Position;
    if (material.NormalMapID != 0)
    {
        LightPos = TBN * PLight[index].pointLight.position;
        ViewPos = TBN * sceneData.CameraPos;
        FragPos2 = TBN * vertex.Position;
    }
    vec3 ViewDir = normalize(ViewPos - FragPos2);

    vec3 lightDir = normalize(LightPos - FragPos2);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + ViewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.Shininess);

    vec3 ambient = sceneData.AmbientLight * material.Diffuse.rgb;
    vec3 diffuse = PLight[index].pointLight.diffuse * diff * material.Diffuse.rgb;
    vec3 specular = PLight[index].pointLight.specular * spec * material.Specular;
    if (material.DiffuseMapID != 0)
    {
        ambient = sceneData.AmbientLight * vec3(texture(TextureMap[material.DiffuseMapID], uv));
        diffuse = PLight[index].pointLight.diffuse * diff * vec3(texture(TextureMap[material.DiffuseMapID], uv));
    }
    if (material.SpecularMapID != 0)
    {
        specular = PLight[index].pointLight.specular * spec * vec3(texture(TextureMap[material.SpecularMapID], uv));
    }

    float LightDistance = length(LightPos - FragPos2);
    float attenuation = 1.0 / (1.0f + PLight[index].pointLight.linear * LightDistance + PLight[index].pointLight.quadratic * (LightDistance * LightDistance));

    vec3 result = (ambient + diffuse + specular) * attenuation;
//    result = RTXShadow(result, specular, lightDir, 10000.0f);
//    result *= attenuation;
    return result;
}

vec3 CalcNormalSpotLight(Vertex vertex, MaterialProperties material, mat3 TBN,  vec3 normal, vec2 uv, int index)
{
    vec3 LightPos = SLight[index].spotLight.position;
    vec3 ViewPos = sceneData.CameraPos;
    vec3 FragPos2 = vertex.Position;
    if (material.NormalMapID != 0)
    {
        LightPos = TBN * SLight[index].spotLight.direction;
        ViewPos = TBN * sceneData.CameraPos;
        FragPos2 = TBN * vertex.Position;
    }
    vec3 ViewDir = normalize(ViewPos - FragPos2);

    vec3 lightDir = normalize(LightPos - FragPos2);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + ViewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.Shininess);

    vec3 ambient = sceneData.AmbientLight * material.Diffuse.rgb;
    vec3 diffuse = SLight[index].spotLight.diffuse * diff * material.Diffuse.rgb;
    vec3 specular = SLight[index].spotLight.specular * spec * material.Specular;
    if (material.DiffuseMapID != 0)
    {
        ambient = sceneData.AmbientLight* vec3(texture(TextureMap[material.DiffuseMapID], uv));
        diffuse = SLight[index].spotLight.diffuse * diff * vec3(texture(TextureMap[material.DiffuseMapID], uv));
    }
    if (material.SpecularMapID != 0)
    {
        specular = SLight[index].spotLight.specular * spec * vec3(texture(TextureMap[material.SpecularMapID], uv));
    }

    float theta = dot(lightDir, normalize(-SLight[index].spotLight.direction)); 
    float epsilon = SLight[index].spotLight.cutOff - SLight[index].spotLight.outerCutOff;
    float intensity = clamp((theta - SLight[index].spotLight.outerCutOff) / epsilon, 0.0, 1.0);

    float LightDistance = length(LightPos - FragPos2);
    float attenuation = 1.0 / (1.0f + SLight[index].spotLight.linear * LightDistance + SLight[index].spotLight.quadratic * (LightDistance * LightDistance));

    return (ambient + diffuse + specular) * attenuation;
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