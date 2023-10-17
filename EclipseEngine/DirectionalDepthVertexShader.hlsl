#include "MeshProperties.hlsli"
#include "MaterialProperties.hlsli"
#include "LightProperties.hlsli"

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
};

struct DepthSceneData
{
    uint MeshIndex;
    uint MaterialIndex;
    uint LightIndex;
    float4x4 proj;
    float4x4 view;
    float3 CameraPos;
};

[[vk::push_constant]] DepthSceneData depthSceneData;
[[vk::binding(0)]] StructuredBuffer<MeshProperties> MeshPropertiesBuffer;
[[vk::binding(1)]] StructuredBuffer<float4x4> ModelTransformBuffer;
[[vk::binding(2)]] StructuredBuffer<MaterialProperties> MaterialPropertiesBuffer;
[[vk::binding(3)]] [[vk::combinedImageSampler]] Texture2D TextureMap : register(t0);
[[vk::binding(3)]] [[vk::combinedImageSampler]] SamplerState TextureMapSampler : register(s0);
[[vk::binding(4)]] StructuredBuffer<DirectionalLight> DirectionalLightBuffer;

VSOutput main(VSOutput input) : SV_TARGET
{
    VSOutput output = (VSOutput) 0;
    
    output.WorldPos = mul(ModelTransformBuffer[depthSceneData.MeshIndex], float4(input.Position, 1.0)).xyz;
    output.Pos = mul(DirectionalLightBuffer[depthSceneData.LightIndex].LightSpaceMatrix, float4(output.WorldPos, 1.0));
    output.Color = input.Color;
 
    return output;
}