#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_debug_printf : enable

layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aColor;
layout (location = 3) in mat4 IntanceMatrix;
layout (location = 7) in vec2 aUVOffset;
layout (location = 8) in int aMaterialID;
layout (location = 9) in vec2 aSpriteFlip;

layout(location = 0) out vec3 FragPos;
layout(location = 1) out vec2 UV;
layout(location = 2) out vec3 Color;
layout(location = 3) out vec2 UVOffset;
layout(location = 4) out int MaterialID;
layout(location = 5) out vec2 SpriteFlip;

#include "VertexLayout.glsl"
#include "MeshProperties.glsl"
#include "MaterialProperties.glsl"
#include "Lights.glsl"
#include "SceneData.glsl"

layout(binding = 0) buffer MeshPropertiesBuffer { MeshProperties meshProperties; } meshBuffer[];
layout(binding = 1) buffer TransformBuffer { mat4 transform; } transformBuffer[];
layout(binding = 2) buffer MaterialPropertiesBuffer { MaterialProperties materialProperties; } materialBuffer[];
layout(binding = 3) uniform sampler2D TextureMap[];

void main() {

// if(gl_VertexIndex == 0)
//	{
//		debugPrintfEXT(": %i \n", sceneData.MeshIndex);
//	}
    FragPos = vec3(IntanceMatrix * vec4(inPosition.xy, 0.0f, 1.0));    
    Color = aColor;
    UV = aUV;
    UVOffset = aUVOffset;
    MaterialID = aMaterialID;
    SpriteFlip = aSpriteFlip;
    gl_Position = sceneData.proj * 
                  sceneData.view *                
                  IntanceMatrix * 
                  vec4(inPosition, 0.0f, 1.0f);

}