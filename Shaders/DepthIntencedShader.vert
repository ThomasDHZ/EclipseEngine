#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable

#include "MeshProperties.glsl"
#include "MaterialProperties.glsl"
#include "Lights.glsl"

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in vec3 aColor;
layout (location = 6) in mat4 IntanceMatrix;
layout (location = 10) in int aMaterialID;

layout(location = 0) out vec3 FragPos;
layout(location = 1) out vec2 UV;
layout(location = 2) out int MaterialID;

layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 1) buffer DirectionalLightBuffer { DirectionalLight directionalLight; } DLight[];

layout(push_constant) uniform DepthSceneData
{
    uint MeshIndex;
    uint LightIndex;
} sceneData;

void main() {

    FragPos = vec3(DLight[sceneData.LightIndex].directionalLight.lightSpaceMatrix *  vec4(inPosition.xyz, 1.0));    
    UV = aUV;
    gl_Position = DLight[sceneData.LightIndex].directionalLight.lightSpaceMatrix *                
                  IntanceMatrix * 
                  vec4(inPosition, 1.0);
   MaterialID = aMaterialID;
}