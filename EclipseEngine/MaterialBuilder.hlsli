MaterialProperties MaterialBuilder(float2 UV, uint MaterialIndex)
{
    MaterialProperties material = MaterialPropertiesBuffer[sceneDataProperties.MaterialIndex];
    
    
    if (material.AlbedoMap != -1)
    {
        material.Albedo = TextureMap[material.AlbedoMap].Sample(TextureMapSampler[material.AlbedoMap], UV).rgb;
    }
    if (material.MetallicMap != -1)
    {
        material.Metallic = TextureMap[material.MetallicMap].Sample(TextureMapSampler[material.MetallicMap], UV).r;
    }
    if (material.RoughnessMap != -1)
    {
        material.Roughness = TextureMap[material.RoughnessMap].Sample(TextureMapSampler[material.RoughnessMap], UV).r;
    }
    
    material.AmbientOcclusion = .04f;
    if (material.AmbientOcclusionMap != -1)
    {
        material.AmbientOcclusion = TextureMap[material.AmbientOcclusionMap].Sample(TextureMapSampler[material.AmbientOcclusionMap], UV).r;
    }
    
    if (material.EmissionMap != -1)
    {
        material.Emission = TextureMap[material.EmissionMap].Sample(TextureMapSampler[material.EmissionMap], UV).rgb;
    }
    if (TextureMap[material.AlbedoMap].Sample(TextureMapSampler[material.AlbedoMap], UV).a == 0.0f)
    {
        discard;
    }
    
    return material;
}