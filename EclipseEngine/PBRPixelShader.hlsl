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

float3x3 getTBNFromMap(VSOutput input, uint meshIndex)
{
    float3 T = normalize(mul(float3x3(MeshPropertiesBuffer[meshIndex].MeshTransform[0].xyz,
                                            MeshPropertiesBuffer[meshIndex].MeshTransform[1].xyz,
                                            MeshPropertiesBuffer[meshIndex].MeshTransform[2].xyz),
                                            float3(input.Tangent)));
    float3 B = normalize(mul(float3x3(MeshPropertiesBuffer[meshIndex].MeshTransform[0].xyz,
                                            MeshPropertiesBuffer[meshIndex].MeshTransform[1].xyz,
                                            MeshPropertiesBuffer[meshIndex].MeshTransform[2].xyz),
                                            float3(input.BiTangent)));
    float3 N = normalize(mul(float3x3(MeshPropertiesBuffer[meshIndex].MeshTransform[0].xyz,
                                            MeshPropertiesBuffer[meshIndex].MeshTransform[1].xyz,
                                            MeshPropertiesBuffer[meshIndex].MeshTransform[2].xyz),
                                            float3(input.Normal)));
    return float3x3(T, B, N);
}

PSOutput main(VSOutput input)
{
    PSOutput output = (PSOutput) 0;
    MaterialProperties material = MaterialPropertiesBuffer[sceneDataProperties.MaterialIndex];
    
    float3x3 TBN = getTBNFromMap(input, sceneDataProperties.MeshIndex);
    float2 UV = input.UV;
    float3 N = input.Normal;
    float3 V = normalize(sceneDataProperties.CameraPos - input.WorldPos);
    
    float3 ViewPos = mul(TBN, sceneDataProperties.CameraPos);
    float3 FragPos2 = mul(TBN, input.WorldPos);
    float3 viewDir = normalize(ViewPos - FragPos2);
    
    if (material.NormalMap != 0)
    {
        ViewPos = mul(TBN, sceneDataProperties.CameraPos);
        FragPos2 = mul(TBN, input.WorldPos);
        float3 viewDir = normalize(ViewPos - FragPos2);

        if (material.DepthMap != 0)
        {
            UV = ParallaxMapping(material, UV, viewDir);
           // if(UV.x > 1.0 || UV.y > 1.0 || UV.x < 0.0 || UV.y < 0.0)
       // discard;
        }
        N = TextureMap[material.NormalMap].Sample(TextureMapSampler[material.NormalMap], input.UV).rgb;
        N = normalize(N * 2.0 - 1.0);
        N = normalize(mul(TBN, N));
    }
    
    float3 R = reflect(-V, N);
    
    float3 Albedo = TextureMap[material.AlbedoMap].Sample(TextureMapSampler[material.AlbedoMap], input.UV).rgb;
    float Metallic = TextureMap[material.MetallicMap].Sample(TextureMapSampler[material.MetallicMap], input.UV).r;
    float Roughness = TextureMap[material.RoughnessMap].Sample(TextureMapSampler[material.RoughnessMap], input.UV).r;
    float AmbientOcclusion = 0.34f;
    float3 Emission = TextureMap[material.EmissionMap].Sample(TextureMapSampler[material.EmissionMap], input.UV).rgb;
   
    float3 F0 = float3(0.04f.rrr);
    F0 = lerp(F0, Albedo, Metallic);

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
        
        //vec4 LightSpace = (LightBiasMatrix * DLight[x].directionalLight.LightSpaceMatrix * meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform) * vec4(FragPos, 1.0);
        //float shadow = filterPCF(LightSpace / LightSpace.w, x);

        Lo += (kD * Albedo / PI + specular) * radiance * NdotL;
    }
    
    float3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, Roughness);
    float3 kS = F;
    float3 kD = 1.0 - kS;
    kD *= 1.0 - Metallic;
    
    float3 irradiance = IrradianceMap.Sample(IrradianceMapSampler, N).rgb;
    float3 diffuse = irradiance * Albedo;

    float3 prefilteredColor = PrefilterMap.SampleLevel(PrefilterMapSampler, R, material.Roughness * sceneDataProperties.PBRMaxMipLevel).rgb;
    float2 brdf = BRDFMap.Sample(BRDFMapSampler, float2(max(dot(N, V), 0.0), material.Roughness)).rg;
    float3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    float3 ambient = Emission + ((kD * diffuse + specular) * AmbientOcclusion);
    
    float3 color = ambient + Lo;
    color = color / (color + float3(1.0f.rrr));
    color = pow(color, float3(1.0f / 2.2f.rrr));

    output.Color = float4(color, 1.0f);

    float3 bloomColor = Emission;
    if (color.r >= 1.0f)
    {
        bloomColor.r = color.r;
    }
    if (color.g >= 1.0f)
    {
        bloomColor.g = color.g;
    }
    if (color.b >= 1.0f)
    {
        bloomColor.b = color.b;
    }

    output.BloomColor = float4(bloomColor, 1.0f);
    
    return output;
}