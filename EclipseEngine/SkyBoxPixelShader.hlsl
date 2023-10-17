#include "ColorCorrectionFunctions.hlsli"

struct VSOutput
{
    [[vk::location(0)]] float3 UV : TEXCOORD0;
};

[[vk::binding(0)]] TextureCube CubeMap : register(t0);
[[vk::binding(0)]] SamplerState CubeMapSampler : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float3 color = CubeMap.Sample(CubeMapSampler, input.UV).rgb;
    return float4(GammaCorrection(color), 1.0f);
}