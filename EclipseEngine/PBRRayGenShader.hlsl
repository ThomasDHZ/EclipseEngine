#include "Vertex.hlsli"
#include "SceneProperties.hlsli"
#include "MeshProperties.hlsli"
#include "MaterialProperties.hlsli"
#include "LightProperties.hlsli"

struct RayPayload
{
    float3 color;
    uint seed;
    int reflectCount;
    int reflectCount2;
};

struct IndicesBuffer
{
    float Index;
};

struct ModelMatrix
{
    float4x4 model;
};

[[vk::push_constant]] SceneProperties sceneDataProperties;
[[vk::binding(0)]]  RaytracingAccelerationStructure topLevelAS;
[[vk::binding(1)]]  RWTexture2D<float4> image;
[[vk::binding(2)]]  ConstantBuffer<Vertex> Vertices[];
[[vk::binding(3)]]  ConstantBuffer<IndicesBuffer> Indices[];
[[vk::binding(4)]]  ConstantBuffer<MeshProperties> MeshPropertiesBuffer[];
[[vk::binding(5)]]  ConstantBuffer<ModelMatrix> ModelTransformBuffer[];
[[vk::binding(6)]]  ConstantBuffer<MaterialProperties> MaterialPropertiesBuffer[];
[[vk::binding(7)]]  Texture2D TextureMap[];
[[vk::binding(7)]]  SamplerState TextureMapSampler[];
[[vk::binding(8)]]  Texture2D BRDFMap;
[[vk::binding(8)]]  SamplerState BRDFMapSampler;
[[vk::binding(9)]]  TextureCube IrradianceMap;
[[vk::binding(9)]]  SamplerState IrradianceMapSampler;
[[vk::binding(10)]]  TextureCube PrefilterMap;
[[vk::binding(10)]]  SamplerState PrefilterMapSampler;
[[vk::binding(11)]]  ConstantBuffer<SunLight> SunLightBuffer[];
[[vk::binding(12)]]  ConstantBuffer<DirectionalLight> DirectionalLightBuffer[];
[[vk::binding(13)]]  ConstantBuffer<PointLight> PointLightBuffer[];
[[vk::binding(14)]] ConstantBuffer<SpotLight> SpotLightBuffer[];

[shader("raygeneration")]
void main()
{
    const float2 pixelCenter = float2(DispatchRaysIndex().xy) + float2(0.5.xx);
    const float2 inUV = pixelCenter / float2(DispatchRaysDimensions().xy);
    float2 d = inUV * 2.0 - 1.0;

    float4 origin = mul(sceneDataProperties.view, float4(0, 0, 0, 1));
    float4 target = mul(sceneDataProperties.proj, float4(d.x, d.y, 1, 1));
    float4 direction = mul(sceneDataProperties.view, float4(normalize(target.xyz / target.w), 0));

    RayPayload rayPayload;
    rayPayload.color = float3(1.0f.rrr);
    rayPayload.reflectCount = 1;
    rayPayload.reflectCount2 = 1;
    
    RayDesc rayDesc;
    rayDesc.Origin = mul(sceneDataProperties.view, float4(0, 0, 0, 1)).xyz;
    rayDesc.Direction = mul(sceneDataProperties.view, float4(normalize(target.xyz), 0)).xyz;
    rayDesc.TMin = 0.001;
    rayDesc.TMax = 10000.0;
    
    float3 BaseColor = float3(0.0f.rrr);
    float3 finalColor = float3(0.0f.rrr);
    TraceRay(topLevelAS, RAY_FLAG_FORCE_OPAQUE, 0xff, 0, 0, 0, rayDesc, rayPayload);

    float3 finalResult = rayPayload.color / (rayPayload.color + float3(1.0f.rrr));
    finalResult = pow(finalResult, float3((1.0f / 2.2f).rrr));

    image[int2(DispatchRaysIndex().xy)] = float4(finalResult, 1.0);
}
