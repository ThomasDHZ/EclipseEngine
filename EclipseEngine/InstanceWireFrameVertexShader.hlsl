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
    [[vk::location(6)]] float4 InstanceMatrix1 : IntanceMatrix;
    [[vk::location(7)]] float4 InstanceMatrix2 : IntanceMatrix2;
    [[vk::location(8)]] float4 InstanceMatrix3 : IntanceMatrix3;
    [[vk::location(9)]] float4 InstanceMatrix4 : IntanceMatrix4;
    [[vk::location(10)]] int MaterialID : MaterialID;
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
    [[vk::location(6)]] float4x4 InststanceMatrix : IntanceMatrix;
    [[vk::location(10)]] int MaterialID : MaterialID;
};

[[vk::push_constant]] SceneProperties sceneProperties;
[[vk::binding(1, 0)]] ConstantBuffer<ModelMatrix> ModelTransformBuffer[] : register(b0, space0);

VSOutput main(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    
    float4x4 InstanceMatrix =
    {
        input.InstanceMatrix1,
        input.InstanceMatrix2,
        input.InstanceMatrix3,
        input.InstanceMatrix4
    };
    
    output.WorldPos = mul(ModelTransformBuffer[sceneProperties.TransformIndex].ModelTransfromMatrix, float4(input.Position, 1.0)).xyz;
    output.Pos = mul(sceneProperties.proj, mul(sceneProperties.view, float4(output.WorldPos, 1.0)));
    output.Color = input.Color;
    output.InststanceMatrix = InstanceMatrix;
    output.MaterialID = input.MaterialID;
    
    return output;
}