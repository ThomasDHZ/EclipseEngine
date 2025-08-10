#version 460
//#extension GL_ARB_separate_shader_objects : enable
//#extension GL_EXT_nonuniform_qualifier : enable
//#extension GL_EXT_scalar_block_layout : enable
//#extension GL_EXT_debug_printf : enable
//
//#include "MeshProperties.glsl"
//#include "MaterialProperties.glsl"
//
//layout(location = 0) in vec3 FragPos;
//layout(location = 1) in vec2 UV;
//layout(location = 2) in vec3 Normal;
//layout(location = 3) in vec3 Tangent;
//layout(location = 4) in vec3 BiTangent;
//layout(location = 5) in vec3 Color;
//
//layout(location = 0) out vec4 outColor;
//
//layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
//layout(binding = 1) buffer MaterialPropertiesBuffer { MaterialProperties materialProperties; } materialBuffer[];
//layout(binding = 2) uniform sampler2D TextureMap[];
//
//layout(push_constant) uniform SceneData
//{
//    uint MeshIndex;
//    mat4 proj;
//    mat4 view;
//    vec3 CameraPos;
//    vec3 MeshColorID;
//    vec3 AmbientLight;
//    uint DirectionalLightCount;
//    uint PointLightCount;
//    uint SpotLightCount;
//    float Timer;
//    float PBRMaxMipLevel;
//} sceneData;
//
//void main() 
//{
//   const uint materialID = meshBuffer[sceneData.MeshIndex].meshProperties.MaterialBufferIndex;
//   MaterialProperties material = materialBuffer[materialID].materialProperties;
//
//   uint alpha = material.AlphaMapID;
//   outColor = vec4(sceneData.MeshColorID, texture(TextureMap[alpha], UV).r);
//}