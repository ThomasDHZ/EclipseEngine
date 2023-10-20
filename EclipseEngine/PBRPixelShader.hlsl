struct PSOutput
{
    [[vk::location(0)]] float4 Color : SV_TARGET0;
    [[vk::location(1)]] float4 BloomColor : SV_TARGET1;
};

#include "PBRRenderer.hlsli"

PSOutput main(VSOutput input)
{
    PSOutput output = (PSOutput) 0;
    MaterialProperties material = MaterialPropertiesBuffer[sceneDataProperties.MaterialIndex];
    
    float3 color = PBRRenderer(input);
    color = color / (color + float3(1.0f.rrr));
    color = pow(color, float3(1.0f / 2.2f.rrr));
    
    output.Color = float4(color, 1.0f);

    float3 bloomColor = material.Emission;
    if (color.r >= 1.0f &&
        color.r >= bloomColor.r)
    {
        bloomColor.r = color.r;
    }
    if (color.g >= 1.0f &&
        color.g >= bloomColor.g)
    {
        bloomColor.g = color.g;
    }
    if (color.b >= 1.0f &&
        color.b >= bloomColor.b)
    {
        bloomColor.b = color.b;
    }

    output.BloomColor = float4(bloomColor, 1.0f);
    
    return output;
}