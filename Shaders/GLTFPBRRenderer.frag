#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec2 UV;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec3 Tangent;
layout(location = 4) in vec3 BiTangent;
layout(location = 5) in vec3 Color;

layout(location = 0) out vec4 outColor;

#include "VertexLayout.glsl"
#include "MeshProperties.glsl"
#include "SceneData.glsl"
#include "Lights.glsl"

layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 1) buffer TransformBuffer { mat4 transform; } transformBuffer[];
layout(binding = 2) uniform sampler2D AlbedoMap;
layout(binding = 3) uniform sampler2D NormalMap;
layout(binding = 4) uniform sampler2D MetallicRoughnessMap;
layout(binding = 5) uniform sampler2D AmbientOcclusionMap;
layout(binding = 6) uniform sampler2D AlphaMap;
layout(binding = 7) uniform sampler2D DepthMap;
layout(binding = 8) uniform sampler2D BRDFMap;
layout(binding = 9) uniform samplerCube IrradianceMap;
layout(binding = 10) uniform samplerCube PrefilterMap;
layout(binding = 11) buffer DirectionalLightBuffer { DirectionalLight directionalLight; } DLight[];
layout(binding = 12) buffer PointLightBuffer { PointLight pointLight; } PLight[];
layout(binding = 13) buffer SpotLightBuffer { SpotLight spotLight; } SLight[];

#include "RasterVertexBuilder.glsl"

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
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
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

const mat4 LightBiasMatrix = mat4(
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.5, 0.5, 0.0, 1.0);

vec3 gridSamplingDisk[20] = vec3[]
(
    vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
    vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
    vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
    vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
    vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
    );

//float ShadowCalculation(vec4 fragPosLightSpace, vec2 offset, int index);
//float filterPCF(vec4 sc, int index);
float CubeShadowCalculation(vec3 fragPos, vec3 viewPos, int index);
mat3 getTBNFromMap(Vertex vertex);
vec3 CalcDirectionalLight(vec3 F0, vec3 V, vec3 N, Vertex vertex);
vec3 CalcPointLight(vec3 F0, vec3 V, vec3 N, Vertex vertex);
vec3 CalcSpotLight(vec3 F0, vec3 V, vec3 N, Vertex vertex);
vec2 ParallaxMapping(vec2 UV, vec3 viewDir);

void main()
{ 
    Vertex vertex = RasterVertexBuilder();
    const vec3 Albedo = texture(AlbedoMap, vertex.UV).rgb;
	const vec3 Normal = texture(NormalMap, vertex.UV).rgb;
	const float Metalic = texture(MetallicRoughnessMap, vertex.UV).g;
	const float Roughness = texture(MetallicRoughnessMap, vertex.UV).b;
	const float AmbientOcclusion = texture(AmbientOcclusionMap, vertex.UV).r;
    const float Depth = texture(DepthMap, vertex.UV).r;
	const float Alpha = texture(AlphaMap, vertex.UV).r;
	const vec2 BRDF = texture(BRDFMap, vertex.UV).rg;

   mat3 TBN = getTBNFromMap(vertex);
   vec3 N = vertex.Normal;

   vec3 ViewPos  = sceneData.CameraPos;
   vec3 FragPos2  = vertex.Position;
   vec3 viewDir = normalize(ViewPos - FragPos2);
   if (Normal.r == 0.0f &&
       Normal.g == 0.0f &&
       Normal.b == 0.0f)
   {
        ViewPos  = TBN * sceneData.CameraPos;
        FragPos2  = TBN * vertex.Position;

          if (Depth == 0.0f)
           {
            vertex.UV = ParallaxMapping(vertex.UV,  viewDir);       
            if(vertex.UV.x > 1.0 || vertex.UV.y > 1.0 || vertex.UV.x < 0.0 || vertex.UV.y < 0.0)
            {
              discard;
            }
        }
        N = Normal;
        N = normalize(N * 2.0 - 1.0);
        N = normalize(TBN * N);
   }

    vec3 V = normalize(sceneData.CameraPos - vertex.Position);
    vec3 R = reflect(-V, N); 

    vec3 F0 = vec3(0.04f); 
    F0 = mix(F0, Albedo, Metalic);

    vec3 Lo = vec3(0.0);
    Lo += CalcDirectionalLight(F0, V, N, vertex);
    Lo += CalcPointLight(F0, V, N, vertex);
    Lo += CalcSpotLight(F0, V, N, vertex);

    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, Roughness);
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - Metalic;	  
    
    vec3 irradiance = texture(IrradianceMap, N).rgb;
    vec3 diffuse    = irradiance * Albedo;

    vec3 prefilteredColor = textureLod(PrefilterMap, R, Roughness * sceneData.PBRMaxMipLevel).rgb;    
    vec2 brdf  = texture(BRDFMap, vec2(max(dot(N, V), 0.0), Roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = ((kD * diffuse + specular) * AmbientOcclusion);
    
    vec3 color = ambient + Lo;
    color = color / (color + vec3(1.0f));
    color = pow(color, vec3(1.0f/2.2f)); 

    if(meshBuffer[sceneData.MeshIndex].meshProperties.SelectedMesh == 1)
    {
        color = mix(color, vec3(1.0f, 0.0f, 0.0f), .35f);
    }
    outColor = vec4(color, 1.0f);	
}

//float ShadowCalculation(vec4 fragPosLightSpace, vec2 offset, int index)
//{
//    float shadow = 1.0f;
//    if (fragPosLightSpace.z > -1.0 && fragPosLightSpace.z < 1.0)
//    {
//        float dist = texture(ShadowMap[index], fragPosLightSpace.st + offset).r;
//        if (fragPosLightSpace.w > 0.0 && dist < fragPosLightSpace.z)
//        {
//            shadow = 0.1f;
//        }
//    }
//    return shadow;
//}
//
//float filterPCF(vec4 sc, int index)
//{
//    ivec2 texDim = textureSize(ShadowMap[index], 0);
//    float scale = 1.5;
//    float dx = scale * 1.0 / float(texDim.x);
//    float dy = scale * 1.0 / float(texDim.y);
//
//    float shadowFactor = 0.0;
//    int count = 0;
//    int range = 1;
//
//    for (int x = -range; x <= range; x++)
//    {
//        for (int y = -range; y <= range; y++)
//        {
//            shadowFactor += ShadowCalculation(sc, vec2(dx * x, dy * y), index);
//            count++;
//        }
//
//    }
//    return shadowFactor / count;
//}
//
mat3 getTBNFromMap(Vertex vertex)
{
    vec3 T = normalize(mat3(meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform) * vec3(vertex.Tangant));
    vec3 N = normalize(mat3(meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform) * vertex.Normal);
    vec3 B = normalize(cross(N, T));
    return mat3(T, B, N);
}

vec3 CalcDirectionalLight(vec3 F0, vec3 V, vec3 N, Vertex vertex)
{
    const vec3 Albedo = texture(AlbedoMap, vertex.UV).rgb;
    const float Metalic = texture(MetallicRoughnessMap, vertex.UV).r;
	const float Roughness = texture(MetallicRoughnessMap, vertex.UV).g;

    vec3 Lo = vec3(0.0);
    for (int x = 0; x < sceneData.DirectionalLightCount; x++)
    {
        vec3 L = normalize(-DLight[x].directionalLight.direction);
        vec3 H = normalize(V + L);
        vec3 radiance = DLight[x].directionalLight.diffuse;

        float NDF = DistributionGGX(N, H, Roughness);
        float G = GeometrySmith(N, V, L, Roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - Metalic;

        float NdotL = max(dot(N, L), 0.0);

        //vec4 LightSpace = (LightBiasMatrix * DLight[x].directionalLight.lightSpaceMatrix * meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform) * vec4(FragPos, 1.0);
       // float shadow = filterPCF(LightSpace / LightSpace.w, x);

        Lo += (kD * Albedo / PI + specular) * radiance * NdotL;
    }
    return Lo;
}

vec3 CalcPointLight(vec3 F0, vec3 V, vec3 N, Vertex vertex)
{
    const vec3 Albedo = texture(AlbedoMap, vertex.UV).rgb;
    const float Metalic = texture(MetallicRoughnessMap, vertex.UV).r;
	const float Roughness = texture(MetallicRoughnessMap, vertex.UV).g;

    vec3 Lo = vec3(0.0);
    for (int x = 0; x < sceneData.PointLightCount; x++)
    {
        vec3 L = normalize(PLight[x].pointLight.position - vertex.Position);
        vec3 H = normalize(V + L);
        float distance = length(PLight[x].pointLight.position - vertex.Position);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = PLight[x].pointLight.diffuse * attenuation;

        float NDF = DistributionGGX(N, H, Roughness);
        float G = GeometrySmith(N, V, L, Roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - Metalic;

        float NdotL = max(dot(N, L), 0.0);

        // float shadow = CubeShadowCalculation(FragPos, V, x);
        Lo += (kD * Albedo / PI + specular) * radiance * NdotL;
    }

    return Lo;
}

vec3 CalcSpotLight(vec3 F0, vec3 V, vec3 N, Vertex vertex)
{
    const vec3 Albedo = texture(AlbedoMap, vertex.UV).rgb;
    const float Metalic = texture(MetallicRoughnessMap, vertex.UV).r;
	const float Roughness = texture(MetallicRoughnessMap, vertex.UV).g;

    vec3 Lo = vec3(0.0);
    for (int x = 0; x < sceneData.SpotLightCount; x++)
    {
        vec3 L = normalize(SLight[x].spotLight.position - vertex.Position);
        vec3 H = normalize(V + L);

        float theta = dot(L, normalize(-SLight[x].spotLight.direction));
        float epsilon = SLight[x].spotLight.cutOff - SLight[x].spotLight.outerCutOff;
        float intensity = clamp((theta - SLight[x].spotLight.outerCutOff) / epsilon, 0.0, 1.0);

        float distance = length(SLight[x].spotLight.position - vertex.Position);
        float attenuation = 1.0 / (distance * distance);
        attenuation *= intensity;
        vec3 radiance = SLight[x].spotLight.diffuse * attenuation;

        float NDF = DistributionGGX(N, H, Roughness);
        float G = GeometrySmith(N, V, L, Roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - Metalic;

        float NdotL = max(dot(N, L), 0.0);

        Lo += (kD * Albedo / PI + specular) * radiance * NdotL;
    }

    return Lo;
}


vec2 ParallaxMapping(vec2 UV, vec3 viewDir)
{
    const float heightScale = meshBuffer[sceneData.MeshIndex].meshProperties.heightScale;
    const float minLayers = meshBuffer[sceneData.MeshIndex].meshProperties.minLayers;
    const float maxLayers = meshBuffer[sceneData.MeshIndex].meshProperties.maxLayers;

    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;

    viewDir.y = -viewDir.y;
    vec2 P = viewDir.xy / viewDir.z * heightScale;
    vec2 deltaTexCoords = P / numLayers;

    vec2  currentTexCoords = UV;
    float currentDepthMapValue = texture(DepthMap, currentTexCoords).r;

    while (currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(DepthMap, currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(DepthMap, prevTexCoords).r - currentLayerDepth + layerDepth;

    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}