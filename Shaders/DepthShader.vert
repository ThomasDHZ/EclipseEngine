#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable

#include "MeshProperties.glsl"
#include "MaterialProperties.glsl"
#include "LightProperties.glsl"

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in vec3 aColor;

layout(location = 0) out vec3 FragPos;
layout(location = 1) out vec2 UV;

layout(push_constant) uniform DepthSceneData
{
    uint MeshIndex;
    uint MaterialIndex;
    uint LightIndex;
	mat4 proj;
	mat4 view;
	vec3 CameraPos;
} sceneData;

layout(binding = 0) buffer TransformBuffer { mat4 transform; } transformBuffer[];
layout(binding = 1) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 2) buffer MaterialPropertiesBuffer { MaterialProperties materialProperties; } materialBuffer[];
layout(binding = 3) uniform sampler2D TextureMap[];
layout(binding = 4) buffer DirectionalLightBuffer { DirectionalLight directionalLight; } DLight[];

void main() {

    FragPos = vec3(DLight[sceneData.LightIndex].directionalLight.LightSpaceMatrix * 
                   transformBuffer[sceneData.MeshIndex].transform * 
                   vec4(inPosition.xyz, 1.0));    
    UV = aUV;
    gl_Position = DLight[sceneData.LightIndex].directionalLight.LightSpaceMatrix * 
				  transformBuffer[sceneData.MeshIndex].transform * 
                  vec4(inPosition, 1.0);

}