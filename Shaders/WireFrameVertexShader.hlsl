#include "MeshProperties.hlsli"
#include "SceneDataProperties.hlsli"

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

[[vk::binding(0)]] StructuredBuffer<MeshProperties> meshPropertiesBuffer;
[[vk::binding(1)]] StructuredBuffer<float4x4> ModelTransformBuffer;
[[vk::push_constant]] SceneDataProperties sceneDataProperties;

VSOutput main(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    
    output.WorldPos = mul(ModelTransformBuffer[sceneDataProperties.MeshIndex], float4(input.Position, 1.0)).xyz;
    output.Pos = mul(sceneDataProperties.proj, mul(sceneDataProperties.view, float4(output.WorldPos, 1.0)));
    output.Color = input.Color;
 
    return output;
}