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

[[vk::push_constant]] SceneProperties sceneDataProperties;
[[vk::binding(0)]]  ConstantBuffer<MeshProperties> MeshPropertiesBuffer[];
[[vk::binding(1)]]  ConstantBuffer<ModelMatrix> ModelTransformBuffer[];
[[vk::binding(11)]]
cbuffer viewMatrix
{
    float4x4 viewMatrix[6];
};

VSOutput main(VSInput input, uint ViewIndex : SV_ViewID)
{
    float4 pos = mul(viewMatrix[ViewIndex], float4(input.Position, 1.0));
    
    VSOutput output = (VSOutput) 0;
     
    //mat4 MeshTransform = transformBuffer[sceneData.MeshIndex].transform;
    //FragPos = vec3(MeshTransform * vec4(inPosition.xyz, 1.0));
    //Color = aColor;
    //UV = aUV;
    //Normal = mat3(MeshTransform) * aNormal;
    //Tangent = aTangent;
    //BiTangent = aBitangent;
    //gl_Position = cubeMapViewSampler.CubeMapFaceMatrix[gl_ViewIndex] *
    //              MeshTransform *
    //              vec4(inPosition, 1.0);
//}
    
    output.WorldPos = mul(viewMatrix[ViewIndex], float4(input.Position, 1.0)).xyz;
    output.Pos = mul(sceneDataProperties.proj, mul(sceneDataProperties.view, float4(output.WorldPos, 1.0)));
    output.UV = input.UV;
    output.Normal = mul(viewMatrix[ViewIndex], float4(input.Normal, 1.0)).xyz;
    output.Tangent = mul(viewMatrix[ViewIndex], float4(input.Tangent, 1.0)).xyz;
    output.BiTangent = input.BiTangent;
    output.Color = input.Color;
    return output;
}