#include "MeshProperties.hlsli"
#include "SceneProperties.hlsli"

struct VSInput
{
    [[vk::location(0)]] float3 Position : POSITION0;
    [[vk::location(1)]] float2 UV : TEXCOORD0;
    [[vk::location(2)]] float3 Color : COLOR0;
    [[vk::location(3)]] float4x4 InstanceMatrix : POSITION1;
    [[vk::location(7)]] float2 UVOffset : POSITION2;
    [[vk::location(8)]] int MaterialID : POSITION3;
    [[vk::location(9)]] float2 SpriteFlip : POSITION4;
};

struct VSOutput
{
    float4 Pos : SV_POSITION;
    [[vk::location(0)]] float3 WorldPos : POSITION0;
    [[vk::location(1)]] float2 UV : TEXCOORD0;
    [[vk::location(2)]] float3 Color : COLOR0;
    [[vk::location(3)]] float2 UVOffset : POSITION2;
    [[vk::location(4)]] int MaterialID : POSITION3;
    [[vk::location(5)]] float2 SpriteFlip : POSITION4;
};

struct ModelMatrix
{
    float4x4 model;
};


[[vk::push_constant]] SceneProperties sceneProperties;
[[vk::binding(0, 0)]] ConstantBuffer<MeshProperties> meshPropertiesBuffer[];
[[vk::binding(1, 0)]] ConstantBuffer<ModelMatrix> ModelTransformBuffer[];

VSOutput main(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    
    output.WorldPos = mul(input.InstanceMatrix, float4(input.Position.xy, 0.0f, 1.0)).xyz;
    output.Pos = mul(sceneProperties.proj, mul(sceneProperties.view, float4(output.WorldPos, 1.0)));
    output.UV = input.UV;
    output.Color = input.Color;
    output.UVOffset = input.UVOffset;
    output.SpriteFlip = input.SpriteFlip;
 
    return output;
}