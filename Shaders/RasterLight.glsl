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

vec3 CalcNormalDirLight(MaterialProperties material, mat3 TBN, vec3 normal, vec2 uv, int index)
{
    vec3 LightPos = DLight[index].directionalLight.direction;

    vec3 ViewPos = sceneData.CameraPos;
    vec3 FragPos2 = FragPos;
    if (material.NormalMapID != 0)
    {
        LightPos = TBN * DLight[index].directionalLight.direction;
        ViewPos = TBN * sceneData.CameraPos;
        FragPos2 = TBN * FragPos;
    }
    vec3 ViewDir = normalize(ViewPos - FragPos2);

    const vec3 lightDir = normalize(-LightPos);
    const float diff = max(dot(normal, lightDir), 0.0);

    const vec3 halfwayDir = normalize(lightDir + ViewDir);
    const float spec = pow(max(dot(normal, halfwayDir), 0.0), material.Shininess);

    vec3 ambient = sceneData.AmbientLight * material.Diffuse.rgb;
    vec3 diffuse = DLight[index].directionalLight.diffuse * diff * material.Diffuse.rgb;
    vec3 specular = DLight[index].directionalLight.specular * spec * material.Specular;
    if (material.DiffuseMapID != 0)
    {
        ambient = sceneData.AmbientLight * vec3(texture(TextureMap[material.DiffuseMapID], uv));
        diffuse = DLight[index].directionalLight.diffuse * diff * vec3(texture(TextureMap[material.DiffuseMapID], uv));
    }
    if (material.SpecularMapID != 0)
    {
        specular = DLight[index].directionalLight.specular * spec * vec3(texture(TextureMap[material.SpecularMapID], uv));
    }

    float LightDistance = length(LightPos - FragPos2);


    //vec4 LightSpace = (LightBiasMatrix *  DLight[index].directionalLight.lightSpaceMatrix * meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform) * vec4(FragPos, 1.0);
  // float shadow = filterPCF(LightSpace/ LightSpace.w, index);  
    return (ambient + diffuse + specular);
}

vec3 CalcNormalPointLight(MaterialProperties material, mat3 TBN, vec3 normal, vec2 uv, int index)
{
    vec3 LightPos = PLight[index].pointLight.position;
    vec3 ViewPos = sceneData.CameraPos;
    vec3 FragPos2 = FragPos;
    if (material.NormalMapID != 0)
    {
        LightPos = TBN * PLight[index].pointLight.position;
        ViewPos = TBN * sceneData.CameraPos;
        FragPos2 = TBN * FragPos;
    }
    vec3 ViewDir = normalize(ViewPos - FragPos2);

    vec3 lightDir = normalize(LightPos - FragPos2);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + ViewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.Shininess);

    vec3 ambient = sceneData.AmbientLight * material.Diffuse.rgb;
    vec3 diffuse = PLight[index].pointLight.diffuse * diff * material.Diffuse.rgb;
    vec3 specular = PLight[index].pointLight.specular * spec * material.Specular;
    if (material.DiffuseMapID != 0)
    {
        ambient = sceneData.AmbientLight * vec3(texture(TextureMap[material.DiffuseMapID], uv));
        diffuse = PLight[index].pointLight.diffuse * diff * vec3(texture(TextureMap[material.DiffuseMapID], uv));
    }
    if (material.SpecularMapID != 0)
    {
        specular = PLight[index].pointLight.specular * spec * vec3(texture(TextureMap[material.SpecularMapID], uv));
    }

    float LightDistance = length(LightPos - FragPos2);
    float attenuation = 1.0 / (1.0f + PLight[index].pointLight.linear * LightDistance + PLight[index].pointLight.quadratic * (LightDistance * LightDistance));

    return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcNormalSpotLight(MaterialProperties material, mat3 TBN, vec3 normal, vec2 uv, int index)
{
    vec3 LightPos = SLight[index].spotLight.position;
    vec3 ViewPos = sceneData.CameraPos;
    vec3 FragPos2 = FragPos;
    if (material.NormalMapID != 0)
    {
        LightPos = TBN * SLight[index].spotLight.direction;
        ViewPos = TBN * sceneData.CameraPos;
        FragPos2 = TBN * FragPos;
    }
    vec3 ViewDir = normalize(ViewPos - FragPos2);

    vec3 lightDir = normalize(LightPos - FragPos2);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + ViewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.Shininess);

    vec3 ambient = sceneData.AmbientLight * material.Diffuse.rgb;
    vec3 diffuse = SLight[index].spotLight.diffuse * diff * material.Diffuse.rgb;
    vec3 specular = SLight[index].spotLight.specular * spec * material.Specular;
    if (material.DiffuseMapID != 0)
    {
        ambient = sceneData.AmbientLight * vec3(texture(TextureMap[material.DiffuseMapID], uv));
        diffuse = SLight[index].spotLight.diffuse * diff * vec3(texture(TextureMap[material.DiffuseMapID], uv));
    }
    if (material.SpecularMapID != 0)
    {
        specular = SLight[index].spotLight.specular * spec * vec3(texture(TextureMap[material.SpecularMapID], uv));
    }

    float theta = dot(lightDir, normalize(-SLight[index].spotLight.direction));
    float epsilon = SLight[index].spotLight.cutOff - SLight[index].spotLight.outerCutOff;
    float intensity = clamp((theta - SLight[index].spotLight.outerCutOff) / epsilon, 0.0, 1.0);

    float LightDistance = length(LightPos - FragPos2);
    float attenuation = 1.0 / (1.0f + SLight[index].spotLight.linear * LightDistance + SLight[index].spotLight.quadratic * (LightDistance * LightDistance));

    return (ambient + diffuse + specular) * attenuation;
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