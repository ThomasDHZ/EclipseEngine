struct TextureCreatorProperties
{
    float time;
};
[[vk::push_constant]] TextureCreatorProperties textureCreatorProperties;

float2 noise2x2(float2 p)
{
    float x = dot(p, float2(123.4, 234.5));
    float y = dot(p, float2(345.6, 456.7));
    float2 noise = float2(x, y);
    noise = sin(noise);
    noise = noise * 43758.5453;
    noise = frac(noise);
    return noise;
}

float4 main(float2 UV : TEXCOORD0) : SV_TARGET
{
    float2 uv = UV;
    uv.y = 1 - UV.y;
    
    float3 color = float3(0.0, 0.0f, 0.0f);
    
    uv = uv * 4.0;
    float2 currentGridId = floor(uv);
    float2 currentGridCoord = frac(uv);
    color = float3(currentGridCoord, 0.0);
    currentGridCoord = currentGridCoord - 0.5;
    color = float3(currentGridCoord, 0.0);
    
    float2 redGridUv = currentGridCoord;
    redGridUv = abs(redGridUv);
    float distToEdgeOfGridCell = 2.0 * max(redGridUv.x, redGridUv.y);
    
    color = float3(distToEdgeOfGridCell.rrr);
    color = float3(smoothstep(0.5f, 1.0f, distToEdgeOfGridCell).rrr);
    
    float3 redGridColor = float3(smoothstep(0.9, 1.0, distToEdgeOfGridCell), 0.0, 0.0);
    
    float pointsOnGrid = 0.0;
    float minDistFromPixel = 100.0;
    for (float u = -1.0; u <= 1.0; u++)
    {
        for (float v = -1.0; v <= 1.0; v++)
        {
            float2 adjGridCoords = float2(u, v);
            float2 pointOnAdjGrid = adjGridCoords;
            
            float2 noise = noise2x2(currentGridId + adjGridCoords);
            pointOnAdjGrid = adjGridCoords + sin(textureCreatorProperties.time * noise) * 0.5;

            float dist = length(currentGridCoord - pointOnAdjGrid);
            minDistFromPixel = min(dist, minDistFromPixel);

            pointsOnGrid += smoothstep(0.95, 0.96, 1.0 - dist);
        }
    }
    float3 pointsOnGridColor = float3(pointsOnGrid.rrr);
    color = redGridColor + pointsOnGridColor;
    color = redGridColor + pointsOnGridColor + minDistFromPixel;
    color = redGridColor + minDistFromPixel;
    
    color = float3(smoothstep(0.25, 1.0, 1.0 - minDistFromPixel).rrr);
    
    return float4(color.xyz, 1.0f);
}