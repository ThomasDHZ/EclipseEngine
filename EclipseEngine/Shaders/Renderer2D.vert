#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable

#include "MeshProperties.glsl"

layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec4 aNormal;
layout (location = 2) in vec4 aTexCoords;
layout (location = 3) in vec4 aTangent;
layout (location = 4) in vec4 aBitangent;

layout(location = 0) out vec4 FragPos;
layout(location = 1) out vec4 TexCoords;
layout(location = 2) out vec4 Normal;
layout(location = 3) out vec4 Tangent;
layout(location = 4) out vec4 BiTangent;

layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];

layout(push_constant) uniform SceneData
{
    uint MeshIndex;
    mat4 proj;
    mat4 view;
    vec3 CameraPos;
    float Timer;
} sceneData;

void main() {

//    if(gl_VertexIndex == 0)
//	{
//		debugPrintfEXT(": %i \n",  meshBuffer[sceneData.MeshIndex].meshProperties.materialProperties.DiffuseMapID);
//	}
    gl_Position = sceneData.proj * sceneData.view * meshBuffer[sceneData.MeshIndex].meshProperties.MeshTransform * vec4(inPosition.xy, 0.0f, 1.0f);
   // Color = inColor;
    TexCoords = aTexCoords;
}