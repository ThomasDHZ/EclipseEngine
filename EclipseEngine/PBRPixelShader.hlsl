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
    [[vk::location(0)]] float4 Color : COLOR0;
    [[vk::location(1)]] float4 BloomColor : COLOR1;
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

float4 main(VSOutput input) : SV_TARGET
{
    MaterialProperties material = MaterialPropertiesBuffer[sceneDataProperties.MaterialIndex];
    return float4(TextureMap[material.AlbedoMap].Sample(TextureMapSampler[material.AlbedoMap], input.UV).rgb, 1.0f);
}