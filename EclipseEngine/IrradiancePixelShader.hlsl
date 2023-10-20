#include "ColorCorrectionFunctions.hlsli"

struct VSOutput
{
    [[vk::location(0)]] float3 WorldPos : POSITION0;
};

struct IrradianceSamplerProperties
{
    uint CubeMapId;
    float IrradianceSampleDelta;
};

static float PI = 3.14159265359f;

[[vk::push_constant]] IrradianceSamplerProperties irradianceSamplerProperties;
[[vk::binding(0)]] TextureCube CubeMap[] : register(t0);
[[vk::binding(0)]] SamplerState CubeMapSampler[] : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float3 N = normalize(input.WorldPos);
    
    float3 irradiance = float3(0.0f, 0.0f, 0.0f);
    
    float3 up = float3(0.0, 1.0, 0.0);
    float3 right = normalize(cross(up, N));
    up = normalize(cross(N, right));
       
    float sampleDelta = irradianceSamplerProperties.IrradianceSampleDelta;
    float nrSamples = 0.0f;
    for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            float3 tangentSample = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            float3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

            irradiance += clamp(CubeMap[irradianceSamplerProperties.CubeMapId].Sample(CubeMapSampler[irradianceSamplerProperties.CubeMapId], sampleVec).rgb, 0.0f, 1.0f) * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));
    return float4(irradiance, 1.0f);

}