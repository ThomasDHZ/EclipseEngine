#include "MeshProperties.hlsli"
#include "SceneProperties.hlsli"

struct ModelMatrix
{
    float4x4 ModelTransfromMatrix;
};

struct VSInput
{
    [[vk::location(0)]] float3 Position : POSITION0;
    [[vk::location(1)]] float2 UV : TEXCOORD0;
    [[vk::location(2)]] float3 Normal : NORMAL0;
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

    [[vk::push_constant]] SceneProperties sceneProperties;
    [[vk::binding(1, 0)]] ConstantBuffer<ModelMatrix> ModelTransformBuffer[] : register(b0, space0);

VSOutput main(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    
    output.WorldPos = mul(ModelTransformBuffer[sceneProperties.TransformIndex].ModelTransfromMatrix, float4(input.Position, 1.0)).xyz;
    output.Pos = mul(sceneProperties.proj, mul(sceneProperties.view, float4(output.WorldPos, 1.0)));
    output.UV = input.UV;
    output.Normal = input.Normal;
    output.Tangent = input.Tangent;
    output.BiTangent = input.BiTangent;
    output.Color = input.Color;
 
    return output;
}