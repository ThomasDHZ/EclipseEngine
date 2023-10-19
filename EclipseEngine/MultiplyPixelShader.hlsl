struct VSOutput
{
    [[vk::location(0)]] float2 UV : TEXCOORD0;
};

[[vk::binding(0)]]  Texture2D TextureMap1;
[[vk::binding(0)]]  SamplerState TextureMap1Sampler;
[[vk::binding(1)]]  Texture2D TextureMap2;
[[vk::binding(1)]]  SamplerState TextureMap2Sampler;

float4 main(float2 UV : TEXCOORD0) : SV_TARGET
{
    float3 result = TextureMap1.Sample(TextureMap1Sampler, UV).rgb * TextureMap2.Sample(TextureMap2Sampler, UV).rgb;
    return float4(result.rgb, 1.0f);
}