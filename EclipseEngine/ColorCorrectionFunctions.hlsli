
float3 GammaCorrection(float3 Color)
{
    float gammaCorrection = 1.0f / 2.2f;
    float3 color = 0.0f;
    color = Color / (Color + float3(1.0f, 1.0f, 1.0f));
    color = pow(color, float3(gammaCorrection, gammaCorrection, gammaCorrection));
    return color;
}