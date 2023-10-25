#include "ColorCorrectionFunctions.hlsli"

struct VSOutput
{
    [[vk::location(0)]] float3 UV : TEXCOORD0;
};

[[vk::binding(0)]] TextureCube CubeMap : register(t0);
[[vk::binding(0)]] SamplerState CubeMapSampler : register(s0);

static float3x3 aces_input_matrix =
{
    0.59719f, 0.35458f, 0.04823f,
    0.07600f, 0.90834f, 0.01566f,
    0.02840f, 0.13383f, 0.83777f
};

static float3x3 aces_output_matrix =
{
    1.60475f, -0.53108f, -0.07367f,
   -0.10208f, 1.10813f, -0.00605f,
   -0.00327f, -0.07276f, 1.07602f
};

float3 rtt_and_odt_fit(float3 v)
{
    float3 a = v * (v + 0.0245786f) - 0.000090537f;
    float3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
    return a / b;
}

float3 aces_fitted(float3 v)
{
    v = mul(aces_input_matrix, v);
    v = rtt_and_odt_fit(v);
    return mul(aces_output_matrix, v);
}

float4 main(VSOutput input) : SV_TARGET
{
    float3 color = CubeMap.Sample(CubeMapSampler, input.UV).rgb;
    return float4(aces_fitted(color), 1.0f);
}