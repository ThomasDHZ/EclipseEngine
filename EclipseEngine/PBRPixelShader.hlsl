#include "Vertex.hlsli"
#include "MaterialProperties.hlsli"
#include "LightProperties.hlsli"
#include "ColorCorrectionFunctions.hlsli"
#include "MeshProperties.hlsli"
#include "SceneProperties.hlsli"

struct VSOutput
{
    [[vk::location(0)]] float3 WorldPos : POSITION0;
    [[vk::location(1)]] float2 UV : TEXCOORD0;
    [[vk::location(2)]] float3 Normal : NORMAL0;
    [[vk::location(3)]] float3 Tangent : POSITION1;
    [[vk::location(4)]] float3 BiTangent : POSITION2;
    [[vk::location(5)]] float3 Color : COLOR0;
};

struct PSOutput
{
    [[vk::location(0)]] float4 Color : SV_TARGET0;
    [[vk::location(1)]] float4 BloomColor : SV_TARGET1;
};

struct ModelMatrix
{
    float4x4 model;
};

[[vk::push_constant]] SceneProperties sceneDataProperties;
[[vk::binding(0)]]  ConstantBuffer<MeshProperties> MeshPropertiesBuffer[];
[[vk::binding(1)]]  ConstantBuffer<ModelMatrix> ModelTransformBuffer[];
[[vk::binding(2)]]  ConstantBuffer<MaterialProperties> MaterialPropertiesBuffer[];
[[vk::binding(3)]]  Texture2D TextureMap[];
[[vk::binding(3)]]  SamplerState TextureMapSampler[];
[[vk::binding(4)]]  Texture2D BRDFMap;
[[vk::binding(4)]]  SamplerState BRDFMapSampler;
[[vk::binding(5)]]  TextureCube IrradianceMap;
[[vk::binding(5)]]  SamplerState IrradianceMapSampler;
[[vk::binding(6)]]  TextureCube PrefilterMap;
[[vk::binding(6)]]  SamplerState PrefilterMapSampler;
[[vk::binding(7)]]  ConstantBuffer<SunLight> SunLightBuffer[];
[[vk::binding(8)]]  ConstantBuffer<DirectionalLight> DirectionalLightBuffer[];
[[vk::binding(9)]]  ConstantBuffer<PointLight> PointLightBuffer[];
[[vk::binding(10)]] ConstantBuffer<SpotLight> SpotLightBuffer[];
[[vk::binding(11)]] Texture2D ShadowMap[];
[[vk::binding(11)]] SamplerState ShadowMapSampler[];
[[vk::binding(12)]] TextureCube PointShadowMap[];
[[vk::binding(12)]] SamplerState PointShadowSampler[];

static float PI = 3.14159265359f;
static float4x4 LightBiasMatrix = float4x4(
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.5, 0.5, 0.0, 1.0);

float ShadowCalculation(float4 fragPosLightSpace, float2 offset, int index)
{
    float shadow = 1.0f;
    if (fragPosLightSpace.z > -1.0 && fragPosLightSpace.z < 1.0)
    {
        float dist = ShadowMap[index].Sample(ShadowMapSampler[index], fragPosLightSpace.xy + offset).r;
        if (fragPosLightSpace.w > 0.0 && dist < fragPosLightSpace.z)
        {
            shadow = 0.1f;
        }
    }
    return shadow;
}

float filterPCF(float4 sc, int index)
{
    float2 texDim;
    ShadowMap[index].GetDimensions(texDim.x, texDim.y);
    
    float scale = 1.5;
    float dx = scale * 1.0 / float(texDim.x);
    float dy = scale * 1.0 / float(texDim.y);

    float shadowFactor = 0.0;
    int count = 0;
    int range = 1;

    for (int x = -range; x <= range; x++)
    {
        for (int y = -range; y <= range; y++)
        {
            shadowFactor += ShadowCalculation(sc, float2(dx * x, dy * y), index);
            count++;
        }

    }
    return shadowFactor / count;
}

float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0)) + 1.0;
    denom = (3.1415927410125732421875 * denom) * denom;
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    float nom = NdotV;
    float denom = (NdotV * (1.0 - k)) + k;
    return nom / denom;
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float param = NdotV;
    float param_1 = roughness;
    float ggx2 = GeometrySchlickGGX(param, param_1);
    float param_2 = NdotL;
    float param_3 = roughness;
    float ggx1 = GeometrySchlickGGX(param_2, param_3);
    return ggx1 * ggx2;
}

float3 fresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
    return F0 + ((max(float3(1.0 - roughness, 1.0 - roughness, 1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0));
}

float2 ParallaxMapping(MaterialProperties material, float2 texCoords, float3 viewDir)
{
    const float heightScale = 0.6f;
    const float minLayers = 3;
    const float maxLayers = 6;
    float numLayers = lerp(maxLayers, minLayers, abs(dot(float3(0.0, 0.0, 1.0), viewDir)));
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;

    viewDir.y = -viewDir.y;
    float2 P = viewDir.xy / viewDir.z * heightScale;
    float2 deltaTexCoords = P / numLayers;

    float2 currentTexCoords = texCoords;
    float currentDepthMapValue = TextureMap[material.HeightMap].Sample(TextureMapSampler[material.HeightMap], currentTexCoords).r;

    while (currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = TextureMap[material.HeightMap].Sample(TextureMapSampler[material.HeightMap], currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    float2 prevTexCoords = currentTexCoords + deltaTexCoords;

    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = TextureMap[material.HeightMap].Sample(TextureMapSampler[material.HeightMap], prevTexCoords).r - currentLayerDepth + layerDepth;

    float weight = afterDepth / (afterDepth - beforeDepth);
    float2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

float3 getTBNFromMap(VSOutput input, uint meshIndex)
{
    MaterialProperties material = MaterialPropertiesBuffer[sceneDataProperties.MaterialIndex];
    
    float3 tangentNormal = TextureMap[material.NormalMap].Sample(TextureMapSampler[material.NormalMap], input.UV).rgb * 2.0 - 1.0;

    float3 N = normalize(input.Normal);
    float3 T = normalize(input.Tangent);
    float3 B = normalize(cross(N, T));
    float3x3 TBN = transpose(float3x3(T, B, N));
    
    return normalize(mul(TBN, tangentNormal));
}

PSOutput main(VSOutput input)
{
    PSOutput output = (PSOutput) 0;
    MaterialProperties material = MaterialPropertiesBuffer[sceneDataProperties.MaterialIndex];
    
    float2 UV = input.UV;
    float3 N = getTBNFromMap(input, sceneDataProperties.MeshIndex);
    float3 V = normalize(sceneDataProperties.CameraPos - input.WorldPos);
   
    if (material.NormalMap != 0)
    {
        if (material.DepthMap != 0)
        {
            UV = ParallaxMapping(material, UV, V);
        }
    }
    
    float3 R = reflect(-V, N);
    
    float4 Albedo = TextureMap[material.AlbedoMap].Sample(TextureMapSampler[material.AlbedoMap], UV);
    float Metallic = TextureMap[material.MetallicMap].Sample(TextureMapSampler[material.MetallicMap], UV).r;
    float Roughness = TextureMap[material.RoughnessMap].Sample(TextureMapSampler[material.RoughnessMap], UV).r;
    //float AmbientOcclusion = TextureMap[material.AmbientOcclusionMap].Sample(TextureMapSampler[material.AmbientOcclusionMap], UV).r;
    float AmbientOcclusion = .04f;
    float3 Emission = TextureMap[material.EmissionMap].Sample(TextureMapSampler[material.EmissionMap], UV).rgb;
    if(Albedo.a == 0.0f)
    {
        discard;
    }
   
    float3 F0 = float3(0.04f.rrr);
    F0 = lerp(F0, Albedo.rgb, Metallic);

    float3 Lo = float3(0.0.rrr);
    for (int x = 0; x < sceneDataProperties.DirectionalLightCount; x++)
    {
        float3 L = normalize(-DirectionalLightBuffer[x].direction);
        float3 H = normalize(V + L);
        float watts = DirectionalLightBuffer[x].intensity;
        float3 radiance = DirectionalLightBuffer[x].diffuse * watts;

        float NDF = DistributionGGX(N, H, Roughness);
        float G = GeometrySmith(N, V, L, Roughness);
        float3 F = fresnelSchlickRoughness(max(dot(H, V), 0.0), F0, Roughness);

        float3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        float3 specular = numerator / denominator;

        float3 kS = F;
        float3 kD = float3(1.0.rrr) - kS;
        kD *= 1.0 - Metallic;

        float NdotL = max(dot(N, L), 0.0);
        
        //float4 LightSpaceMatrix = mul(MeshPropertiesBuffer[sceneDataProperties.MeshIndex].MeshTransform, float4(input.WorldPos, 1.0));
        //       LightSpaceMatrix = mul(DirectionalLightBuffer[x].LightSpaceMatrix, LightSpaceMatrix);
        //       LightSpaceMatrix = mul(LightBiasMatrix, LightSpaceMatrix);
        
       // float shadow = filterPCF(LightSpaceMatrix / LightSpaceMatrix.w, x);

        Lo += (kD * Albedo.rgb / PI + specular) * radiance * NdotL;
       // Lo *= shadow;

    }

    for (int x = 0; x < sceneDataProperties.PointLightCount; x++)
    {
        float3 L = normalize(PointLightBuffer[x].position - input.WorldPos);
        float3 H = normalize(V + L);

        float distance = length(PointLightBuffer[x].position - input.WorldPos);
        float Kc = 1.0f;
        float Kl = 2 / PointLightBuffer[x].radius;
        float Kq = 1 / (PointLightBuffer[x].radius * PointLightBuffer[x].radius);
        float attenuation = 1.0f / (Kc + Kl * distance + Kq * (distance * distance));

        float watts = PointLightBuffer[x].intensity;
        float3 radiance = PointLightBuffer[x].diffuse * watts * attenuation;

        float NDF = DistributionGGX(N, H, material.Roughness);
        float G = GeometrySmith(N, V, L, material.Roughness);
        float3 F = fresnelSchlickRoughness(max(dot(H, V), 0.0), F0, material.Roughness);

        float3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        float3 specular = numerator / denominator;

        float3 kS = F;
        float3 kD = float3(1.0.rrr) - kS;
        kD *= 1.0 - material.Metallic;

        float NdotL = max(dot(N, L), 0.0);

        float EPSILON = 0.15;
        float SHADOW_OPACITY = 0.005;

        float3 lightVec = input.WorldPos - PointLightBuffer[x].position;
        float dist = length(lightVec);

        //float shadow = CubeShadowCalculation(vertex.Position, V, x);

        Lo += (kD * material.Albedo / PI + specular) * radiance * NdotL;
        //Lo *= vec3(shadow);
    }

    float3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, Roughness);
    float3 kS = F;
    float3 kD = 1.0 - kS;
    kD *= 1.0 - Metallic;
    
    float3 irradiance = IrradianceMap.Sample(IrradianceMapSampler, N).rgb;
    float3 diffuse = irradiance * Albedo.rgb;

    float3 prefilteredColor = PrefilterMap.SampleLevel(PrefilterMapSampler, R, material.Roughness * sceneDataProperties.PBRMaxMipLevel).rgb;
    float2 brdf = BRDFMap.Sample(BRDFMapSampler, float2(max(dot(N, V), 0.0), material.Roughness)).rg;
    float3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    float3 ambient = Emission + ((kD * diffuse + specular) * AmbientOcclusion);
    
    float3 color = ambient + Lo;
    color = color / (color + float3(1.0f.rrr));
    color = pow(color, float3(1.0f / 2.2f.rrr));

    output.Color = float4(color, 1.0f);

    float3 bloomColor = Emission;
    if (color.r >= 1.0f &&
        color.r >= bloomColor.r)
    {
        bloomColor.r = color.r;
    }
    if (color.g >= 1.0f &&
        color.g >= bloomColor.g)
    {
        bloomColor.g = color.g;
    }
    if (color.b >= 1.0f &&
        color.b >= bloomColor.b)
    {
        bloomColor.b = color.b;
    }

    output.BloomColor = float4(bloomColor, 1.0f);
    
    return output;
}