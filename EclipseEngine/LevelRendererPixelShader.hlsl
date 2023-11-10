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
    [[vk::location(2)]] float3 Color : COLOR0;
    [[vk::location(3)]] float2 UVOffset : POSITION2;
    [[vk::location(4)]] int MaterialID : POSITION3;
    [[vk::location(5)]] float2 SpriteFlip : POSITION4;
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

PSOutput main(VSOutput input)
{
    PSOutput output = (PSOutput) 0;
    
    float2 UV = input.UV + input.UVOffset;
    if (input.SpriteFlip.x == 1.0f)
    {
        UV.x = 1.0 - UV.x;
    }
    if (input.SpriteFlip.y == 1.0f)
    {
        UV.y = 1.0 - UV.y;
    }

    MaterialProperties material = MaterialPropertiesBuffer[input.MaterialID];
    material.Albedo = TextureMap[material.AlbedoMap].Sample(TextureMapSampler[material.AlbedoMap], UV).rgb;

    float3 result = material.Albedo;

    float3 finalResult = float3(1.0.rrr) - exp(-result * 1.0f);
    finalResult = pow(finalResult, float3((1.0 / 2.2f).rrr));

    output.Color = float4(finalResult, 1.0f);
    output.BloomColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    return output;
}