  #version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_multiview : enable

layout (location = 0) in vec3 aPos;
layout(location = 0) out vec3 WorldPos;

mat4 MVP[6] = {{{0.000000, 0.000000, 1.010101, 1.000000},
                       {0.000000, -1.000000, 0.000000, 0.000000},
                       {-1.000000, 0.000000, 0.000000, 0.000000},
                       {0.000000, 0.000000, -0.101010, 0.000000}},
                      {{0.000000, 0.000000, -1.010101, -1.000000},
                       {0.000000, -1.000000, 0.000000, 0.000000},
                       {1.000000, 0.000000, 0.000000, 0.000000},
                       {0.000000, 0.000000, -0.101010, 0.000000}},
                      {{1.000000, 0.000000, 0.000000, 0.000000},
                       {0.000000, 0.000000, 1.010101, 1.000000},
                       {0.000000, 1.000000, 0.000000, 0.000000},
                       {0.000000, 0.000000, -0.101010, 0.000000}},
                      {{1.000000, 0.000000, 0.000000, 0.000000},
                       {0.000000, 0.000000, -1.010101, -1.000000},
                       {0.000000, -1.000000, 0.000000, 0.000000},
                       {0.000000, 0.000000, -0.101010, 0.000000}},
                      {{1.000000, 0.000000, 0.000000, 0.000000},
                       {0.000000, -1.000000, 0.000000, 0.000000},
                       {0.000000, 0.000000, 1.010101, 1.000000},
                       {0.000000, 0.000000, -0.101010, 0.000000}},
                      {{-1.000000, 0.000000, 0.000000, 0.000000},
                       {0.000000, -1.000000, 0.000000, 0.000000},
                       {0.000000, 0.000000, -1.010101, -1.000000},
                       {0.000000, 0.000000, -0.101010, 0.000000}}};


void main() {
	WorldPos = aPos;
	vec4 pos = MVP[gl_ViewIndex] * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
