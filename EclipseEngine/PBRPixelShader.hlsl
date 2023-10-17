#include "Vertex.hlsli"
#include "MaterialProperties.hlsli"
#include "LightProperties.hlsli"
#include "ColorCorrectionFunctions.hlsli"

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
    [[vk::location(0)]] float4 Color : COLOR0;
    [[vk::location(1)]] float4 BloomColor : COLOR1;
};

struct SceneProperties
{
    uint MeshIndex;
    uint PrimitiveIndex;
    uint MaterialIndex;
    float4x4 proj;
    float4x4 view;
    float3 CameraPos;
    float3 MeshColorID;
    float3 AmbientLight;
    uint SunLightCount;
    uint DirectionalLightCount;
    uint PointLightCount;
    uint SpotLightCount;
    float Timer;
    float PBRMaxMipLevel;
    uint frame;
    int MaxRefeflectCount;
};

struct MeshProperties
{
    uint VertexBufferIndex;
    uint IndexBufferIndex;
    uint MaterialBufferIndex;
    uint AlbedoMapIndex;
    uint MetallicRoughnessMapIndex;
    uint AmbientOcclusionMapIndex;
    uint NormalMapIndex;
    uint DepthMapIndex;
    uint AlphaMapIndex;
    uint EmissionMapIndex;
    uint SkyBoxIndex;
    float4x4 MeshTransform;
    float2 UVOffset;
    float2 UVScale;
    float2 UVFlip;
    int SelectedMesh;
    float heightScale;
    float minLayers;
    float maxLayers;
};

struct ModelMatrix
{
    float4x4 model;
};

[[vk::push_constant]] SceneProperties sceneDataProperties;
[[vk::binding(0)]]  ConstantBuffer<MeshProperties> MeshPropertiesBuffer[] : register(b0, space0);
[[vk::binding(1)]]  ConstantBuffer<ModelMatrix> ModelTransformBuffer[] : register(b1, space0);
[[vk::binding(2)]]  ConstantBuffer<MaterialProperties> MaterialPropertiesBuffer[] : register(b2, space0);
[[vk::binding(3)]]  Texture2D TextureMap[] : register(t0);
[[vk::binding(3)]]  SamplerState TextureMapSampler[] : register(s0);
[[vk::binding(4)]]  Texture2D BRDFMap : register(t1);
[[vk::binding(4)]]  SamplerState BRDFMapSampler : register(s1);
[[vk::binding(5)]]  TextureCube IrradianceMap : register(t2);
[[vk::binding(5)]]  SamplerState IrradianceMapSampler : register(s2);
[[vk::binding(6)]]  TextureCube PrefilterMap : register(t3);
[[vk::binding(6)]]  SamplerState PrefilterMapSampler : register(s3);
[[vk::binding(7)]]  ConstantBuffer<SunLight> SunLightBuffer[] : register(b3, space0);
[[vk::binding(8)]]  ConstantBuffer<DirectionalLight> DirectionalLightBuffer[] : register(b4, space0);
[[vk::binding(9)]]  ConstantBuffer<PointLight> PointLightBuffer[] : register(b5, space0);
[[vk::binding(10)]] ConstantBuffer<SpotLight> SpotLightBuffer[] : register(b6, space0);
[[vk::binding(11)]] Texture2D ShadowMap[] : register(t4);
[[vk::binding(11)]] SamplerState ShadowMapSampler[] : register(s4);
[[vk::binding(12)]] TextureCube PointShadowMap[] : register(t5);
[[vk::binding(12)]] SamplerState PointShadowSampler[] : register(s5);


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


float3x3 getTBNFromMap(VSOutput input, uint meshIndex)
{
    float3 T = normalize(mul(float3x3(MeshPropertiesBuffer[meshIndex].MeshTransform[0].xyz, MeshPropertiesBuffer[meshIndex].MeshTransform[1].xyz, MeshPropertiesBuffer[meshIndex].MeshTransform[2].xyz), float3(input.Tangent)));
    float3 B = normalize(mul(float3x3(MeshPropertiesBuffer[meshIndex].MeshTransform[0].xyz, MeshPropertiesBuffer[meshIndex].MeshTransform[1].xyz, MeshPropertiesBuffer[meshIndex].MeshTransform[2].xyz), float3(input.BiTangent)));
    float3 N = normalize(mul(float3x3(MeshPropertiesBuffer[meshIndex].MeshTransform[0].xyz, MeshPropertiesBuffer[meshIndex].MeshTransform[1].xyz, MeshPropertiesBuffer[meshIndex].MeshTransform[2].xyz), input.Normal));
    return float3x3(T, B, N);
}

float3 CalcDirectionalLight(float3 F0, float3 V, float3 N, MaterialProperties material)
{
    float3 Lo = float3(0.0f, 0.0f, 0.0f);
    for (int x = 0; x < sceneDataProperties.DirectionalLightCount; x++)
    {
        float3 L = normalize(-DirectionalLightBuffer[x].direction);
        float3 H = normalize(V + L);
        float watts = DirectionalLightBuffer[x].intensity;
        float3 radiance = DirectionalLightBuffer[x].diffuse * watts;

        float NDF = DistributionGGX(N, H, material.Roughness);
        float G = GeometrySmith(N, V, L, material.Roughness);
        float3 F = fresnelSchlickRoughness(max(dot(H, V), 0.0), F0, material.Roughness);

        float3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        float3 specular = numerator / denominator;

        float3 kS = F;
        float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
        kD *= 1.0 - material.Metallic;

        float NdotL = max(dot(N, L), 0.0);

       /// float4 LightSpace = (LightBiasMatrix * DirectionalLightBuffer[x].LightSpaceMatrix * meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform) * float4(FragPo, 1.0);
       // float shadow = filterPCF(LightSpace / LightSpace.w, x);

        Lo += (kD * material.Albedo / PI + specular) * radiance * NdotL;
       // Lo *= shadow;
    }
    return Lo;
}

float4 main(VSOutput input) : SV_TARGET
{
    //MaterialProperties material = BuildMaterial(input, sceneDataProperties.MeshIndex, sceneDataProperties.MaterialIndex);
    
    MaterialProperties material = MaterialPropertiesBuffer[sceneDataProperties.MaterialIndex];
    material.Albedo = TextureMap[material.AlbedoMap].Sample(TextureMapSampler[material.AlbedoMap], input.UV).rgb;
    material.Metallic = TextureMap[material.MetallicMap].Sample(TextureMapSampler[material.MetallicMap], input.UV).r;
    material.Roughness = TextureMap[material.RoughnessMap].Sample(TextureMapSampler[material.RoughnessMap], input.UV).r;
    
    float3x3 TBN = getTBNFromMap(input, sceneDataProperties.MeshIndex);
    float3 N = input.Normal;
    
    float3 ViewPos = sceneDataProperties.CameraPos;
    float3 viewDir = normalize(ViewPos - input.WorldPos);
    if (material.NormalMap != 0)
    {
        ViewPos = mul(TBN, sceneDataProperties.CameraPos);
        input.WorldPos = mul(TBN, input.WorldPos);

    //    /*if(material.DepthMap != 0)
    //    {
    //        vertex.UV = ParallaxMapping(material.DepthMap, vertex.UV,  viewDir);       
    //        if(vertex.UV.x > 1.0 || vertex.UV.y > 1.0 || vertex.UV.x < 0.0 || vertex.UV.y < 0.0)
    //        {
    //          discard;
    //        }
    //    }*/
        N = TextureMap[material.NormalMap].Sample(TextureMapSampler[material.NormalMap], input.UV).rgb;
        N = normalize(input.Normal * 2.0 - 1.0);
        N = normalize(mul(TBN, N));
    }
    
    float3 V = normalize(ViewPos - input.WorldPos);
    float3 R = reflect(-V, input.Normal);

    float3 F0 = float3(0.04f, 0.04f, 0.04f);
    F0 = lerp(F0, material.Albedo, material.Metallic);
    
    float3 Lo = float3(0.0f, 0.0f, 0.0f);
 //// Lo += CalcSunLight(F0, V, N, vertex, material);
    Lo += CalcDirectionalLight(F0, V, input.Normal, material);
 ////  Lo += CalcPointLight(F0, V, N, vertex, material);
 // // Lo += CalcSpotLight(F0, V, N, vertex, material);
    
    float3 F = fresnelSchlickRoughness(max(dot(input.Normal, V), 0.0), F0, material.Roughness);
    float3 kS = F;
    float3 kD = 1.0f - kS;
    kD *= 1.0f - material.Metallic;
    
    float3 irradiance = IrradianceMap.Sample(IrradianceMapSampler, N).rgb;
    float3 diffuse = irradiance * material.Albedo;
    
    float3 prefilteredColor = PrefilterMap.SampleLevel(PrefilterMapSampler, R, material.Roughness * sceneDataProperties.PBRMaxMipLevel).rgb;
    float2 brdf = BRDFMap.Sample(BRDFMapSampler, float2(max(dot(N, V), 0.0), material.Roughness)).rg;
    float3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    float3 ambient = material.Emission + ((kD * diffuse + specular) * material.AmbientOcclusion);
    
    float3 color = ambient + Lo;
    color = color / (color + float3(1.0f, 1.0f, 1.0f));
    color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));
 //   float3 bloomColor = material.Emission;
 //   if (color.r >= 1.0f)
 //   {
 //       bloomColor.r = color.r;
 //   }
 //   if (color.g >= 1.0f)
 //   {
 //       bloomColor.g = color.g;
 //   }
 //   if (color.b >= 1.0f)
 //   {
 //       bloomColor.b = color.b;
 //   }

  
    return float4(TextureMap[material.AlbedoMap].Sample(TextureMapSampler[material.AlbedoMap], input.UV).rgb, 1.0f);
}