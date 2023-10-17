struct VSOutput
{
    [[vk::location(0)]] float3 WorldPos : POSITION0;
};

[[vk::binding(0)]] [[vk::combinedImageSampler]] Texture2D    EnvironmentMap : register(t0);
[[vk::binding(0)]] [[vk::combinedImageSampler]] SamplerState EnvironmentMapSampler : register(s0);

static float2 invAtan = float2(0.1591f, 0.3183f);
float2 SampleSphericalMap(float3 v)
{
    float2 uv = float2(atan2(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

float4 main(VSOutput input) : SV_TARGET
{
    float2 uv = SampleSphericalMap(normalize(input.WorldPos));
    
    float3 color = EnvironmentMap.Sample(EnvironmentMapSampler, uv).rgb;
    return float4(color, 1.0f);
}