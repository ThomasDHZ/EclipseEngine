#include "VertexLayout.glsl"
#include "MeshProperties.glsl"
#include "MaterialProperties.glsl"
#include "Lights.glsl"

layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 1) buffer MaterialPropertiesBuffer { MaterialProperties materialProperties; } materialBuffer[];
layout(binding = 2) buffer DirectionalLightBuffer { DirectionalLight directionalLight; } DLight[];
layout(binding = 3) buffer PointLightBuffer { PointLight pointLight; } PLight[];
layout(binding = 4) buffer SpotLightBuffer { SpotLight spotLight; } SLight[];
layout(binding = 5) uniform sampler2D TextureMap[];
layout(binding = 6) uniform samplerCube IrradianceMap[];
layout(binding = 7) uniform samplerCube PrefilterMap[];
layout(binding = 8) uniform sampler2D BRDFMap;
layout(binding = 9) uniform sampler2D ShadowMap[];
layout(binding = 10) uniform samplerCube PointShadowMap[];