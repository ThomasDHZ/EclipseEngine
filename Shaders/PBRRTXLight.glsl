mat3 getTBNFromMap(Vertex vertex)
{
    const vec3 T = normalize(mat3(meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform) * vec3(vertex.Tangant));
    const vec3 B = normalize(mat3(meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform) * vec3(vertex.BiTangant));
    const vec3 N = normalize(mat3(meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform) * vertex.Normal);
    mat3 TBN = mat3(T, B, N);

    return TBN;
}

vec3 CalcDirectionalLight(vec3 F0, vec3 V, vec3 N, Vertex vertex, PBRMaterial pbrMaterial)
{
    vec3 Lo = vec3(0.0);
    for (int x = 0; x < sceneData.DirectionalLightCount; x++)
    {
        vec3 L = normalize(-DLight[x].directionalLight.direction);
        vec3 H = normalize(V + L);
        vec3 radiance = DLight[x].directionalLight.diffuse;

        float NDF = DistributionGGX(N, H, pbrMaterial.Roughness);
        float G = GeometrySmith(N, V, L, pbrMaterial.Roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.0001f;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - pbrMaterial.Metallic;

        float NdotL = max(dot(N, L), 0.0);

        float tmin = 0.001f;
        float tmax = length(L - vertex.Position);
        vec3 origin = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_HitTEXT;

        shadowed = false;
        traceRayEXT(topLevelAS, gl_RayFlagsSkipClosestHitShaderEXT, 0xFF, 1, 0, 1, origin, tmin, L, tmax, 1);

        if (shadowed)
        {
            Lo += (kD * pbrMaterial.Albedo / PI + specular) * radiance * NdotL;
        }
        else
        {
            Lo += (kD * pbrMaterial.Albedo / PI + specular) * radiance * NdotL * 0.3f;
        }
    }
    return Lo;
}

vec3 CalcPointLight(vec3 F0, vec3 V, vec3 N, Vertex vertex, PBRMaterial pbrMaterial)
{
    vec3 Lo = vec3(0.0);
    for (int x = 0; x < sceneData.PointLightCount; x++)
    {
        vec3 L = normalize(PLight[x].pointLight.position - vertex.Position);
        vec3 H = normalize(V + L);
        float distance = length(PLight[x].pointLight.position - vertex.Position);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = PLight[x].pointLight.diffuse * attenuation;

        float NDF = DistributionGGX(N, H, pbrMaterial.Roughness);
        float G = GeometrySmith(N, V, L, pbrMaterial.Roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.0001f;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - pbrMaterial.Metallic;

        float NdotL = max(dot(N, L), 0.0);

        float tmin = 0.001f;
        float tmax = length(L - vertex.Position);
        vec3 origin = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_HitTEXT;

        shadowed = false;
        traceRayEXT(topLevelAS, gl_RayFlagsSkipClosestHitShaderEXT, 0xFF, 1, 0, 1, origin, tmin, L, tmax, 1);

        if (shadowed)
        {
            Lo += (kD * pbrMaterial.Albedo / PI + specular) * radiance * NdotL;
        }
        else
        {
            Lo += (kD * pbrMaterial.Albedo / PI + specular) * radiance * NdotL * 0.3f;
        }
    }

    return Lo;
}

vec3 CalcSpotLight(vec3 F0, vec3 V, vec3 N, Vertex vertex, PBRMaterial pbrMaterial)
{
    vec3 Lo = vec3(0.0);
    for (int x = 0; x < sceneData.SpotLightCount; x++)
    {
        vec3 L = normalize(SLight[x].spotLight.position - vertex.Position);
        vec3 H = normalize(V + L);

        float theta = dot(L, normalize(-SLight[x].spotLight.direction));
        float epsilon = SLight[x].spotLight.cutOff - SLight[x].spotLight.outerCutOff;
        float intensity = clamp((theta - SLight[x].spotLight.outerCutOff) / epsilon, 0.0, 1.0);

        float distance = length(SLight[x].spotLight.position - vertex.Position);
        float attenuation = 1.0 / (distance * distance);
        attenuation *= intensity;
        vec3 radiance = SLight[x].spotLight.diffuse * attenuation;

        float NDF = DistributionGGX(N, H, pbrMaterial.Roughness);
        float G = GeometrySmith(N, V, L, pbrMaterial.Roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.0001f;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - pbrMaterial.Metallic;

        float NdotL = max(dot(N, L), 0.0);

        Lo += (kD * pbrMaterial.Albedo / PI + specular) * radiance * NdotL;
    }

    return Lo;
}

vec2 ParallaxMapping(MaterialProperties material, vec2 texCoords, vec3 viewDir)
{
    const float heightScale = meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.heightScale;
    const float minLayers = meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.minLayers;
    const float maxLayers = meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.maxLayers;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;

    viewDir.y = -viewDir.y;
    vec2 P = viewDir.xy / viewDir.z * heightScale;
    vec2 deltaTexCoords = P / numLayers;

    vec2  currentTexCoords = texCoords;
    float currentDepthMapValue = texture(TextureMap[material.DepthMapID], currentTexCoords).r;

    while (currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(TextureMap[material.DepthMapID], currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(TextureMap[material.DepthMapID], prevTexCoords).r - currentLayerDepth + layerDepth;

    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

