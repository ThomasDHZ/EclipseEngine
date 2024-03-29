static float4x4 MVP[6] =
{
    {
        { 0.000000, 0.000000, 1.010101, 1.000000 },
        { 0.000000, -1.000000, 0.000000, 0.000000 },
        { -1.000000, 0.000000, 0.000000, 0.000000 },
        { 0.000000, 0.000000, -0.101010, 0.000000 }
    },
    {
        { 0.000000, 0.000000, -1.010101, -1.000000 },
        { 0.000000, -1.000000, 0.000000, 0.000000 },
        { 1.000000, 0.000000, 0.000000, 0.000000 },
        { 0.000000, 0.000000, -0.101010, 0.000000 }
    },
    {
        { 1.000000, 0.000000, 0.000000, 0.000000 },
        { 0.000000, 0.000000, 1.010101, 1.000000 },
        { 0.000000, 1.000000, 0.000000, 0.000000 },
        { 0.000000, 0.000000, -0.101010, 0.000000 }
    },
    {
        { 1.000000, 0.000000, 0.000000, 0.000000 },
        { 0.000000, 0.000000, -1.010101, -1.000000 },
        { 0.000000, -1.000000, 0.000000, 0.000000 },
        { 0.000000, 0.000000, -0.101010, 0.000000 }
    },
    {
        { 1.000000, 0.000000, 0.000000, 0.000000 },
        { 0.000000, -1.000000, 0.000000, 0.000000 },
        { 0.000000, 0.000000, 1.010101, 1.000000 },
        { 0.000000, 0.000000, -0.101010, 0.000000 }
    },
    {
        { -1.000000, 0.000000, 0.000000, 0.000000 },
        { 0.000000, -1.000000, 0.000000, 0.000000 },
        { 0.000000, 0.000000, -1.010101, -1.000000 },
        { 0.000000, 0.000000, -0.101010, 0.000000 }
    }
};

struct VSInput
{
    [[vk::location(0)]] float3 Position : POSITION0;
};

struct VSOutput
{
    float4 Pos : SV_POSITION;
    [[vk::location(0)]] float3 WorldPos : POSITION0;
};

VSOutput main(VSInput input, uint ViewIndex : SV_ViewID)
{
    float4 pos = mul(transpose(MVP[ViewIndex]), float4(input.Position, 1.0));
    
    VSOutput output = (VSOutput) 0;
     
    output.WorldPos = input.Position;
    output.Pos = pos.xyww;
    return output;
}