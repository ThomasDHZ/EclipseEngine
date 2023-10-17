#include "Vertex.hlsli"
#include "MeshProperties.hlsli"
#include "MaterialProperties.hlsli"
#include "MaterialBuilder.hlsli"
#include "LightProperties.hlsli"

struct VSOutput
{
    [[vk::location(0)]] float3 WorldPos : POSITION0;
    [[vk::location(1)]] float2 UV : TEXCOORD0;
    [[vk::location(2)]] float3 Normal : NORMAL0;
    [[vk::location(3)]] float3 Tangent : POSITION1;
    [[vk::location(4)]] float3 BiTangent : POSITION2;
    [[vk::location(5)]] float3 Color : COLOR0;
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

Vertex RasterDepthVertexBuilder(uint MeshIndex, MeshProperties meshProperties, float3 worldPos, float2 uv)
{
    Vertex vertex;
    vertex.Position = worldPos;
    vertex.UV = uv;

    return vertex;
}

MaterialProperties BuildDepthMaterial(uint materialId, float2 UV)
{
    MaterialProperties material = MaterialPropertiesBuffer[materialId];

    if (texture(TextureMap[material.AlphaMap], UV).r == 0.0f ||
		texture(TextureMap[material.AlbedoMap], UV).a == 0.0f)
    {
        discard;
    }
    
    UV += MeshPropertiesBuffer[depthSceneData.MeshIndex].UVOffset;
    UV *= MeshPropertiesBuffer[depthSceneData.MeshIndex].UVScale;
    
    if (MeshPropertiesBuffer[depthSceneData.MeshIndex].UVFlip.y == 1.0f)
    {
        UV.y = 1.0f - UV.y;
    }
    if (MeshPropertiesBuffer[depthSceneData.MeshIndex].UVFlip.x == 1.0f)
    {
        UV.x = 1.0f - UV.x;
    }
    
    return material;
}

void main(VSOutput input) : SV_TARGET
{
    Vertex vertex = RasterDepthVertexBuilder(depthSceneData.MeshIndex, MeshPropertiesBuffer, input.WorldPos, input.UV);
    MaterialProperties material = BuildDepthMaterial(depthSceneData.MaterialIndex, vertex.UV);
}