MaterialProperties BuildMaterial(uint materialId, vec2 UV)
{
	MaterialProperties material = materialBuffer[materialId].materialProperties;

	//  material.Albedo = material.Albedo;
   //	if (material.AlbedoMap != 0)
   //	{
	material.Albedo = texture(TextureMap[material.AlbedoMap], UV).rgb;
	//}

   // material.Metallic = material.Metallic;
	if (material.MetallicRoughnessMap != 0)
	{
		material.Metallic = texture(TextureMap[material.MetallicMap], UV).r;
		material.Roughness = texture(TextureMap[material.RoughnessMap], UV).r;
	}
	else
	{
		material.Metallic = texture(TextureMap[material.MetallicRoughnessMap], UV).b;
		material.Roughness = texture(TextureMap[material.MetallicRoughnessMap], UV).g;
	}

	// material.Roughness = material.Roughness;
	 //if (material.MetallicRoughnessMap != 0)
	 //{

 //	}

 //material.AmbientOcclusion = material.AmbientOcclusion;
	 //if (material.AmbientOcclusionMap != 0)
	 //{
	material.AmbientOcclusion = 0.1f;
	//}

	//material.Emission = material.Emission;
	//if (material.EmissionMap != 0)
	//{
	material.Emission = texture(TextureMap[material.EmissionMap], UV).rgb;
	//}

	if (texture(TextureMap[material.AlbedoMap], UV).a == 0.0f)
	{
		discard;
	}

	return material;
}

MaterialProperties BuildDepthMaterial(uint materialId, vec2 UV)
{
	MaterialProperties material = materialBuffer[materialId].materialProperties;

	if (texture(TextureMap[material.AlphaMap], UV).r == 0.0f ||
		texture(TextureMap[material.AlphaMap], UV).a == 0.0f)
	{
		discard;
	}
	return material;
}