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

#include "RTXRandom.glsl"
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

const float PI = 3.14159265359;

vec3 Irradiate(vec3 N, float roughness, int count);
mat3 getTBNFromMap(Vertex vertex);
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);
vec3 CalcDirectionalLight(vec3 F0, vec3 V, vec3 N, vec3 albedo, float roughness, float metallic);
vec3 CalcPointLight(Vertex vertex, vec3 F0, vec3 V, vec3 N, vec3 albedo, float roughness, float metallic);
vec3 CalcSpotLight(Vertex vertex, vec3 F0, vec3 V, vec3 N, vec3 albedo, float roughness, float metallic);
vec2 ParallaxMapping(Vertex vertex, MaterialProperties material, vec2 texCoords, vec3 viewDir);

void main()
{		
   Vertex vertex = BuildVertexInfo();
    const uint materialID = meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MaterialBufferIndex;
    MaterialProperties material = materialBuffer[materialID].materialProperties;

    vec2 FinalUV = vertex.UV + meshBuffer[sceneData.MeshIndex].meshProperties.UVOffset;
         FinalUV *= meshBuffer[sceneData.MeshIndex].meshProperties.UVScale;

   if(meshBuffer[sceneData.MeshIndex].meshProperties.UVFlip.y == 1.0f)
   {
        FinalUV.y = 1.0f - FinalUV.y;
   }
   if(meshBuffer[sceneData.MeshIndex].meshProperties.UVFlip.x == 1.0f)
   {
        FinalUV.x = 1.0f - FinalUV.x;
   }

   vec3 albedo = pow(material.Albedo, vec3(2.2));
   if (material.AlbedoMapID != 0)
   {
       albedo = pow(texture(TextureMap[material.AlbedoMapID], FinalUV).rgb, vec3(2.2));
   }

   float metallic =  material.Matallic;
   if (material.MetallicMapID != 0)
   {
     metallic = texture(TextureMap[material.MetallicMapID], FinalUV).r;
   }

   float roughness =  material.Roughness;
   if (material.RoughnessMapID != 0)
   {
     roughness = texture(TextureMap[material.RoughnessMapID], FinalUV).r;
   }

   float ao = material.AmbientOcclusion;
   if (material.AmbientOcclusionMapID != 0)
   {
     ao = texture(TextureMap[material.AmbientOcclusionMapID], FinalUV).r;
   }

   vec3 emission = material.Emission;
   if (material.EmissionMapID != 0)
   {
       emission = texture(TextureMap[material.EmissionMapID], FinalUV).rgb;
   }

   mat3 TBN = getTBNFromMap(vertex);
   vec3 N = vertex.Normal;

   vec3 ViewPos  = sceneData.CameraPos;
   vec3 FragPos  = vertex.Position;
   vec3 viewDir = normalize(ViewPos - FragPos);
   if (material.NormalMapID != 0)
   {
        ViewPos  = TBN * sceneData.CameraPos;
        FragPos  = TBN * vertex.Position;

        if(material.DepthMapID != 0)
        {
            FinalUV = ParallaxMapping(vertex, material, FinalUV,  viewDir);       
        }
        N = texture(TextureMap[material.NormalMapID], FinalUV).rgb;
        N = normalize(N * 2.0 - 1.0);
        N = normalize(TBN * N);
   }

    vec3 V = normalize(sceneData.CameraPos - FragPos);
    vec3 R = reflect(-V, N); 

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    Lo += CalcDirectionalLight(F0, V, N, albedo, roughness, metallic);
    Lo += CalcPointLight(vertex, F0, V, N, albedo, roughness, metallic);
    Lo += CalcSpotLight(vertex, F0, V, N, albedo, roughness, metallic);

    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  
    

    vec3 irradiance = vec3(0.0f);
    if(rayHitInfo.reflectCount <= sceneData.MaxRefeflectCount)
    {
        vec3 hitPos = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_RayTmaxEXT;
        vec3 origin   = hitPos.xyz + N * 0.001f;
        vec3 rayDir   = reflect(origin, N);

        rayHitInfo.reflectCount++;
        traceRayEXT(topLevelAS, gl_RayFlagsNoneEXT, 0xff, 0, 0, 0, origin, 0.001f, rayDir, 10000.0f, 0);
		irradiance += rayHitInfo.color; 
        rayHitInfo.reflectCount += 1;
    }
    
    rayHitInfo.reflectCount = 0;
    vec3 diffuse = irradiance * albedo;

    vec3 specular = vec3(0.0f);    
//    if(metallic > 0.0f &&
//       rayHitInfo.reflectCount != sceneData.MaxRefeflectCount)
//    {
//        vec3 hitPos = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_RayTmaxEXT;
//        vec3 origin   = hitPos.xyz + N * 0.001f;
//        vec3 rayDir   = reflect(origin, N);
//
//        rayHitInfo.reflectCount++;
//        traceRayEXT(topLevelAS, gl_RayFlagsNoneEXT, 0xff, 0, 0, 0, origin, 0.001f, rayDir, 10000.0f, 0);
//		specular += rayHitInfo.color; 
//        rayHitInfo.reflectCount = sceneData.MaxRefeflectCount + 1;
//	}

    vec3 ambient = emission + ((kD * diffuse + specular) * ao);
    
    vec3 color = ambient + Lo;
    color = color / (color + vec3(1.0f));
    color = pow(color, vec3(1.0f/2.2f)); 
    rayHitInfo.color = color;
}

vec3 Irradiate(vec3 N, float roughness, int count)
{
    vec3 irradiance = vec3(0.0f);

        vec3 hitPos = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_RayTmaxEXT;
        vec3 origin   = hitPos.xyz + N * 0.0001f;
        vec3 rayDir   = reflect(gl_WorldRayDirectionEXT, N + roughness * gl_LaunchIDEXT.y * gl_LaunchSizeEXT.x + gl_LaunchIDEXT.x);
        traceRayEXT(topLevelAS, gl_RayFlagsNoneEXT, 0xff, 0, 0, 0, origin, 0.001f, rayDir, 10000.0f, 0);

        rayHitInfo.reflectCount++;
        irradiance += rayHitInfo.color;
        return irradiance;
}

mat3 getTBNFromMap(Vertex vertex)
{
   const vec3 T = normalize(mat3(meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform) * vec3(vertex.Tangant));
   const vec3 B = normalize(mat3(meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform) * vec3(vertex.BiTangant));
   const vec3 N = normalize(mat3(meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform) * vertex.Normal);
    mat3 TBN = mat3(T, B, N);

    return TBN;
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}  

vec3 CalcDirectionalLight(vec3 F0, vec3 V, vec3 N, vec3 albedo, float roughness, float metallic)
{
   vec3 Lo = vec3(0.0);

   for(int x = 0; x < sceneData.DirectionalLightCount; x++)
   {
        vec3 L = normalize(-DLight[x].directionalLight.direction);
        vec3 H = normalize(V + L);
        vec3 radiance = DLight[x].directionalLight.diffuse;

        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);    
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);        
        
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.0001f;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;	                
            
        float NdotL = max(dot(N, L), 0.0);        

        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }
    return Lo;
}

vec3 CalcPointLight(Vertex vertex, vec3 F0, vec3 V, vec3 N, vec3 albedo, float roughness, float metallic)
{
   vec3 Lo = vec3(0.0);
     for(int x = 0; x < sceneData.PointLightCount; x++)
   {
        vec3 L = normalize(PLight[x].pointLight.position - vertex.Position);
        vec3 H = normalize(V + L);
        float distance = length(PLight[x].pointLight.position - vertex.Position);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = PLight[x].pointLight.diffuse * attenuation;

        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);    
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);        
        
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.0001f;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;	                
            
        float NdotL = max(dot(N, L), 0.0);        

        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
   }

    return Lo;
}

vec3 CalcSpotLight(Vertex vertex, vec3 F0, vec3 V, vec3 N, vec3 albedo, float roughness, float metallic)
{
   vec3 Lo = vec3(0.0);
   for(int x = 0; x < sceneData.SpotLightCount; x++)
   {
        vec3 L = normalize(SLight[x].spotLight.position - vertex.Position);
        vec3 H = normalize(V + L);

        float theta = dot(L, normalize(-SLight[x].spotLight.direction)); 
        float epsilon = SLight[x].spotLight.cutOff - SLight[x].spotLight.outerCutOff;
        float intensity = clamp((theta - SLight[x].spotLight.outerCutOff) / epsilon, 0.0, 1.0);

        float distance = length(SLight[x].spotLight.position - vertex.Position);
        float attenuation = 1.0 / (distance * distance) ;
        attenuation *= intensity;
        vec3 radiance = SLight[x].spotLight.diffuse * attenuation;

        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);    
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);        
        
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.0001f;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;	                
            
        float NdotL = max(dot(N, L), 0.0);        

        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
   }

    return Lo;
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

