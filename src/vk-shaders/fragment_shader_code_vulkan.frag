// #version 300 es
#version 460
#extension GL_ARB_separate_shader_objects : enable
precision highp float;

layout (location = 0) in vec3 color;

layout (location = 0) out vec4 output_color;

void main (void)
{
	// output_color = vec4(color, 1.0);
	output_color = vec4(1.0, 0.0, 0.0, 1.0);
}
