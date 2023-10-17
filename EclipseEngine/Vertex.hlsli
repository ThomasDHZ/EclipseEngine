struct Vertex
{
    float3 Position;
    float PositionPadding;
    float3 Normal;
    float NormalPadding;
    float2 UV;
    float2 UVPadding;
    float3 Tangant;
    float TangantPadding;
    float3 BiTangant;
    float BiTangentPadding;
    float3 Color;
    float ColorPadding;
};

struct Vertex2D
{
    float2 Position;
    float2 UV;
    float4 Color;
};