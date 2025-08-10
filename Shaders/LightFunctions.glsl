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
    float near_plane = 1.0f;
    float far_plane = 5.0f;

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

vec3 CalcDirectionalLight(vec3 F0, vec3 V, vec3 N, MaterialProperties material)
{
    vec3 Lo = vec3(0.0);
    for (int x = 0; x < sceneData.DirectionalLightCount; x++)
    {
        vec3 L = normalize(-DLight[x].directionalLight.direction);
        vec3 H = normalize(V + L);
        float watts = DLight[x].directionalLight.intensity;
        vec3 radiance = DLight[x].directionalLight.diffuse * watts;

        float NDF = DistributionGGX(N, H, material.Roughness);
        float G = GeometrySmith(N, V, L, material.Roughness);
        vec3 F = fresnelSchlickRoughness(max(dot(H, V), 0.0), F0, material.Roughness);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - material.Metallic;

        float NdotL = max(dot(N, L), 0.0);

        vec4 LightSpace = (LightBiasMatrix * DLight[x].directionalLight.LightSpaceMatrix * meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform) * vec4(FragPos, 1.0);
        float shadow = filterPCF(LightSpace / LightSpace.w, x);


        Lo += (kD * material.Albedo / PI + specular) * radiance * NdotL;
    }
    return Lo;
}

vec3 CalcPointLight(vec3 F0, vec3 V, vec3 N, Vertex vertex, MaterialProperties material)
{
    vec3 Lo = vec3(0.0);
    for (int x = 0; x < sceneData.PointLightCount; x++)
    {
        vec3 L = normalize(PLight[x].pointLight.position - vertex.Position);
        vec3 H = normalize(V + L);

        float distance = length(PLight[x].pointLight.position - vertex.Position);
        float Kc = 1.0f;
        float Kl = 2 / PLight[x].pointLight.radius;
        float Kq = 1 / (PLight[x].pointLight.radius * PLight[x].pointLight.radius);
        float attenuation = 1.0f / (Kc + Kl * distance + Kq * (distance * distance));

        float watts = PLight[x].pointLight.intensity;
        vec3 radiance = PLight[x].pointLight.diffuse * watts * attenuation;

        float NDF = DistributionGGX(N, H, material.Roughness);
        float G = GeometrySmith(N, V, L, material.Roughness);
        vec3 F = fresnelSchlickRoughness(max(dot(H, V), 0.0), F0, material.Roughness);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - material.Metallic;

        float NdotL = max(dot(N, L), 0.0);

        float EPSILON = 0.15;
        float SHADOW_OPACITY = 0.005;

        vec3 lightVec = vertex.Position - PLight[x].pointLight.position;
        float dist = length(lightVec);

        float shadow = CubeShadowCalculation(vertex.Position, V, x);

        Lo += (kD * material.Albedo / PI + specular) * radiance * NdotL;
        Lo *= vec3(shadow);
    }
    return Lo;
}

vec3 CalcSpotLight(vec3 F0, vec3 V, vec3 N, Vertex vertex, MaterialProperties material)
{
    vec3 Lo = vec3(0.0);
    for (int x = 0; x < sceneData.SpotLightCount; x++)
    {
        vec3 L = normalize(SLight[x].spotLight.position - vertex.Position);
        vec3 H = normalize(V + L);

        float theta = dot(L, normalize(-SLight[x].spotLight.direction));
        float epsilon = SLight[x].spotLight.cutOff - SLight[x].spotLight.outerCutOff;
        float intensity = clamp((theta - SLight[x].spotLight.outerCutOff) / epsilon, 0.0, 1.0);

        float distance = length(PLight[x].pointLight.position - vertex.Position);
        float Kc = 1.0f;
        float Kl = 2 / SLight[x].spotLight.radius;
        float Kq = 1 / (SLight[x].spotLight.radius * SLight[x].spotLight.radius);
        float attenuation = 1.0f / (Kc + Kl * distance + Kq * (distance * distance));

        float watts = SLight[x].spotLight.intensity;
        vec3 radiance = SLight[x].spotLight.diffuse * watts * attenuation;

        float NDF = DistributionGGX(N, H, material.Roughness);
        float G = GeometrySmith(N, V, L, material.Roughness);
        vec3 F = fresnelSchlickRoughness(max(dot(H, V), 0.0), F0, material.Roughness);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - material.Metallic;

        float NdotL = max(dot(N, L), 0.0);

        Lo += (kD * material.Albedo / PI + specular) * radiance * NdotL;
    }

    return Lo;
}