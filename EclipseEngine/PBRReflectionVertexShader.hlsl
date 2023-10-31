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
[[vk::binding(0)]]  ConstantBuffer<MeshProperties> MeshPropertiesBuffer[];
[[vk::binding(1)]]  ConstantBuffer<ModelMatrix> ModelTransformBuffer[];
[[vk::binding(11)]]
cbuffer viewMatrix
{
    float4x4 viewMatrix[6];
};
//viewMatrix[ ViewIndex]
VSOutput main(VSInput input, uint ViewIndex : SV_ViewID)
{
    float4 pos = mul(viewMatrix[ViewIndex], float4(input.Position, 1.0));
    
    VSOutput output = (VSOutput) 0;
     
    output.WorldPos = mul(ModelTransformBuffer[sceneProperties.TransformIndex].model, float4(input.Position, 1.0)).xyz;
    output.Pos = mul(viewMatrix[ViewIndex], float4(output.WorldPos, 1.0));
    output.UV = input.UV;
    output.Normal = mul(ModelTransformBuffer[sceneProperties.TransformIndex].model, float4(input.Normal, 1.0)).xyz;
    output.Tangent = mul(ModelTransformBuffer[sceneProperties.TransformIndex].model, float4(input.Tangent, 1.0)).xyz;
    output.BiTangent = input.BiTangent;
    output.Color = input.Color;
    return output;
}