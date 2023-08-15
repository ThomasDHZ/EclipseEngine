const mat4 LightBiasMatrix = mat4(
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.5, 0.5, 0.0, 1.0);

vec3 gridSamplingDisk[20] = vec3[]
(
    vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
    vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
    vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
    vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
    vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
    );

float ShadowCalculation(vec4 fragPosLightSpace, vec2 offset, int index)
{
    float shadow = 1.0f;
    if (fragPosLightSpace.z > -1.0 && fragPosLightSpace.z < 1.0)
    {
        float dist = texture(ShadowMap[index], fragPosLightSpace.st + offset).r;
        if (fragPosLightSpace.w > 0.0 && dist < fragPosLightSpace.z)
        {
            shadow = 0.1f;
        }
    }
    return shadow;
}

float filterPCF(vec4 sc, int index)
{
    ivec2 texDim = textureSize(ShadowMap[index], 0);
    float scale = 1.5;
    float dx = scale * 1.0 / float(texDim.x);
    float dy = scale * 1.0 / float(texDim.y);

    float shadowFactor = 0.0;
    int count = 0;
    int range = 1;

    for (int x = -range; x <= range; x++)
    {
        for (int y = -range; y <= range; y++)
        {
            shadowFactor += ShadowCalculation(sc, vec2(dx * x, dy * y), index);
            count++;
        }

    }
    return shadowFactor / count;
}

float CubeShadowCalculation(vec3 fragPos, vec3 viewPos, int index)
{
    float far_plane = 500.0f;
    vec3 fragToLight = fragPos - PLight[index].pointLight.position;
    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for (int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(PointShadowMap[index], fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if (currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;
}

mat3 getTBNFromMap(Vertex vertex)
{
    vec3 T = normalize(mat3(meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform) * vec3(vertex.Tangant));
    vec3 B = normalize(mat3(meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform) * vec3(vertex.BiTangant));
    vec3 N = normalize(mat3(meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform) * vertex.Normal);
    return mat3(T, B, N);
}

vec3 CalcDirectionalLight(vec3 F0, vec3 V, vec3 N, PBRMaterial pbrMaterial)
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
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - pbrMaterial.Metallic;

        float NdotL = max(dot(N, L), 0.0);

        vec4 LightSpace = (LightBiasMatrix * DLight[x].directionalLight.lightSpaceMatrix * meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform) * vec4(FragPos, 1.0);
        float shadow = filterPCF(LightSpace / LightSpace.w, x);

        Lo += (kD * pbrMaterial.Albedo / PI + specular) * radiance * NdotL * shadow;
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
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - pbrMaterial.Metallic;

        float NdotL = max(dot(N, L), 0.0);

        // float shadow = CubeShadowCalculation(FragPos, V, x);
        Lo += (kD * pbrMaterial.Albedo / PI + specular) * radiance * NdotL;// * shadow;
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
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - pbrMaterial.Metallic;

        float NdotL = max(dot(N, L), 0.0);

        Lo += (kD * pbrMaterial.Albedo / PI + specular) * radiance * NdotL;
    }

    return Lo;
}


vec2 ParallaxMapping(uint depthMapID, vec2 texCoords, vec3 viewDir)
{
    const float heightScale = meshBuffer[sceneData.MeshIndex].meshProperties.heightScale;
    const float minLayers = meshBuffer[sceneData.MeshIndex].meshProperties.minLayers;
    const float maxLayers = meshBuffer[sceneData.MeshIndex].meshProperties.maxLayers;

    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;

    viewDir.y = -viewDir.y;
    vec2 P = viewDir.xy / viewDir.z * heightScale;
    vec2 deltaTexCoords = P / numLayers;

    vec2  currentTexCoords = texCoords;
    float currentDepthMapValue = texture(TextureMap[depthMapID], currentTexCoords).r;

    while (currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(TextureMap[depthMapID], currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(TextureMap[depthMapID], prevTexCoords).r - currentLayerDepth + layerDepth;

    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}