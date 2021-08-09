// #version 300 es
#version 460
#extension GL_ARB_separate_shader_objects : enable
precision highp float;

layout (location = 0) in vec3 vertex_data;

// out gl_PerVertex {
//   vec4 gl_Position;
// };

layout (set = 0, binding = 0) uniform camera
{
	mat4 proj_mat;
	mat4 view_mat;
}
cam[2];

layout (set = 0, binding = 1) uniform camera2
{
	mat4 proj_mat;
	mat4 view_mat;
}
cam2[2];

layout (location = 0) out vec3 color;

void main (void)
{
	color = vertex_data;

	gl_Position = cam[1].proj_mat * cam2[0].view_mat * vec4(vertex_data * 2.0, 1.0);

	gl_Position = vec4(vertex_data * 2.0, 1.0);

	// vulkan has opposite Y direction with opengl
	gl_Position.y = - gl_Position.y;
	// gl_Position = vec4(vertex_data.xy * 0.5, 0.0, 1.0);
}
