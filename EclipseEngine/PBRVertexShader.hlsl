#include "MeshProperties.hlsli"
#include "SceneProperties.hlsli"

struct VSInput
{
    [[vk::location(0)]] float3 Position : POSITION0;
    [[vk::location(1)]] float3 Normal : NORMAL0;
    [[vk::location(2)]] float2 UV : TEXCOORD0;
    [[vk::location(3)]] float3 Tangent : POSITION1;
    [[vk::location(4)]] float3 BiTangent : POSITION2;
    [[vk::location(5)]] float3 Color : COLOR0;
};

struct VSOutput
{
    float4 Pos : SV_POSITION;
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

[[vk::push_constant]] SceneProperties sceneProperties;
[[vk::binding(0, 0)]] ConstantBuffer<MeshProperties> meshPropertiesBuffer[];
[[vk::binding(1, 0)]] ConstantBuffer<ModelMatrix> ModelTransformBuffer[];

VSOutput main(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    
    output.WorldPos = mul(ModelTransformBuffer[sceneProperties.MeshIndex].model, float4(input.Position, 1.0)).xyz;
    output.Pos = mul(sceneProperties.proj, mul(sceneProperties.view, float4(output.WorldPos, 1.0)));
    output.UV = input.UV;
    output.Normal = mul(ModelTransformBuffer[sceneProperties.MeshIndex].model, float4(input.Normal, 1.0)).xyz;
    output.Tangent = input.Tangent;
    output.BiTangent = input.BiTangent;
    output.Color = input.Color;
 
    return output;
}