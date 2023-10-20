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

#include "LightBuilder.hlsli"

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

float3 getNormalFromMap(VSOutput input, uint meshIndex)
{
    MaterialProperties material = MaterialPropertiesBuffer[sceneDataProperties.MaterialIndex];
    
    float3 tangentNormal = TextureMap[material.NormalMap].Sample(TextureMapSampler[material.NormalMap], input.UV).rgb * 2.0 - 1.0;

    float3 N = normalize(input.Normal);
    float3 T = normalize(input.Tangent);
    float3 B = normalize(cross(N, T));
    float3x3 TBN = transpose(float3x3(T, B, N));
    
    return normalize(mul(TBN, tangentNormal));
}

MaterialProperties MaterialBuilder(float2 UV, uint MaterialIndex)
{
    MaterialProperties material = MaterialPropertiesBuffer[sceneDataProperties.MaterialIndex];
    
    
    if (material.AlbedoMap != -1)
    {
        material.Albedo = TextureMap[material.AlbedoMap].Sample(TextureMapSampler[material.AlbedoMap], UV).rgb;
    }
    if (material.MetallicMap != -1)
    {
        material.Metallic = TextureMap[material.MetallicMap].Sample(TextureMapSampler[material.MetallicMap], UV).r;
    }
    if (material.RoughnessMap != -1)
    {
        material.Roughness = TextureMap[material.RoughnessMap].Sample(TextureMapSampler[material.RoughnessMap], UV).r;
    }
    
    material.AmbientOcclusion = .04f;
    if (material.AmbientOcclusionMap != -1)
    {
        material.AmbientOcclusion = TextureMap[material.AmbientOcclusionMap].Sample(TextureMapSampler[material.AmbientOcclusionMap], UV).r;
    }
    
    if (material.EmissionMap != -1)
    {
        material.Emission = TextureMap[material.EmissionMap].Sample(TextureMapSampler[material.EmissionMap], UV).rgb;
    }
    if (TextureMap[material.AlbedoMap].Sample(TextureMapSampler[material.AlbedoMap], UV).a == 0.0f)
    {
        discard;
    }
    
    return material;
}


float3 PBRRenderer(VSOutput input)
{
    MaterialProperties material = MaterialPropertiesBuffer[sceneDataProperties.MaterialIndex];
    
    float2 UV = input.UV;
    float3 N = getNormalFromMap(input, sceneDataProperties.MeshIndex);
    float3 V = normalize(sceneDataProperties.CameraPos - input.WorldPos);
   
    if (material.NormalMap != 0)
    {
        if (material.DepthMap != 0)
        {
            UV = ParallaxMapping(material, UV, V);
        }
    }
    
    float3 R = reflect(-V, N);
    
    material = MaterialBuilder(input.UV, sceneDataProperties.MaterialIndex);
   
    float3 F0 = float3(0.04f.rrr);
    F0 = lerp(F0, material.Albedo.rgb, material.Metallic);

    float3 Lo = float3(0.0.rrr);
    Lo += DirectionalLightCalc(V, N, F0, material);
    Lo += PointLightCalc(V, N, F0, input.WorldPos, material);

    float3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, material.Roughness);
    float3 kS = F;
    float3 kD = 1.0 - kS;
    kD *= 1.0 - material.Metallic;
    
    float3 irradiance = IrradianceMap.Sample(IrradianceMapSampler, N).rgb;
    float3 diffuse = irradiance * material.Albedo.rgb;

    float3 prefilteredColor = PrefilterMap.SampleLevel(PrefilterMapSampler, R, material.Roughness * sceneDataProperties.PBRMaxMipLevel).rgb;
    float2 brdf = BRDFMap.Sample(BRDFMapSampler, float2(max(dot(N, V), 0.0), material.Roughness)).rg;
    float3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    float3 ambient = material.Emission + ((kD * diffuse + specular) * material.AmbientOcclusion);
    
    float3 color = ambient + Lo;
    return color;
}