#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable

#include "MeshProperties.glsl"
#include "Lights.glsl"

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in vec3 aColor;

layout(push_constant) uniform SceneData
{
    uint MeshIndex;
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
    vec3 MeshColorID;
    float Timer;
} sceneData;

layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 1) buffer DirectionalLightBuffer { DirectionalLight directionalLight; } DLight[];

void main() {

    gl_Position = DLight[0].directionalLight.lightSpaceMatrix * 
                  meshBuffer[sceneData.MeshIndex].meshProperties.GameObjectTransform * 
                  meshBuffer[sceneData.MeshIndex].meshProperties.ModelTransform * 
                  meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform * 
                  vec4(inPosition, 1.0);

}