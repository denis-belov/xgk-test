// #define DEBUG



#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"



using std::cout;
using std::endl;



const GLchar* vertex_shader_code_opengl =
R"(
	#version 460

	// #extension GL_ARB_separate_shader_objects : enable

	precision highp float;

	layout (location = 0) in vec3 vertex_data;

	// out gl_PerVertex
	// {
	//   vec4 gl_Position;
	// };

	layout (binding = 0) uniform camera
	{
		mat4 proj_mat;
		mat4 view_mat;
	};

	layout (location = 0) out vec3 color;

	void main (void)
	{
		color = vertex_data;

		gl_Position = proj_mat * view_mat * vec4(vertex_data * 2.0, 1.0);
	}
)";

const GLchar* fragment_shader_code_opengl =
R"(
	#version 460

	// #extension GL_ARB_separate_shader_objects : enable

	precision highp float;

	layout (location = 0) in vec3 color;

	layout (location = 0) out vec4 output_color;

	void main (void)
	{
		output_color = vec4(color, 1.0);
	}
)";



extern const float vertices [];
extern const uint32_t vertices_size;
extern void (* loop_function) (void);
extern void (* destroy_api_function) (void);
extern GLFWwindow* window;

struct Orbit;

extern Orbit orbit;

void idle_function (void);
void glfw_key_callback (GLFWwindow*, int, int, int, int);



GLuint vertex_buffer {};
GLuint framebuffer {};
GLuint framebuffer_renderbuffer_color {};
GLuint framebuffer_renderbuffer_depth {};
GLuint pixel_pack_buffer {};
GLuint program {};



void loop_function_GL (void)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);
}



void* pixel_data {};

size_t pbo_index {};
size_t next_pbo_index { 1 };



void loop_function_GL2 (void)
{
	glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, ((void*) &orbit) + 64);

	// glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
	// glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, vertices_size / 12);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// glFinish();

	// glBindBuffer(GL_PIXEL_PACK_BUFFER, pixel_pack_buffer);
	glBufferData(GL_PIXEL_PACK_BUFFER, 800 * 800 * 4, nullptr, GL_DYNAMIC_READ);
	// glReadBuffer(GL_COLOR_ATTACHMENT0);
	glReadPixels(0, 0, 800, 800, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	pixel_data = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);

	// if (pixel_data)
	// {
	// 	glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
	// }

	// glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	// glDrawBuffer(GL_BACK);
	// glReadBuffer(GL_FRONT);
}

void destroyGL (void)
{
	loop_function = idle_function;

	glFinish();

	glfwDestroyWindow(window);

	glfwTerminate();
}

void destroyGL2 (void)
{
	loop_function = idle_function;

	glFinish();

	glfwDestroyWindow(window);

	glfwTerminate();
}

void initGL (void)
{
	if (destroy_api_function != destroyGL)
	{
		destroy_api_function();

		glfwInit();

		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

		window = glfwCreateWindow(800, 600, "", nullptr, nullptr);

		glfwSetKeyCallback(window, glfw_key_callback);
		glfwMakeContextCurrent(window);
		glfwSwapInterval(0);
		// glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);



		gladLoadGL();



		glViewport(0, 0, 800, 600);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);



		GLuint uniform_buffer;
		glCreateBuffers(1, &uniform_buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer);
		glBufferData(GL_UNIFORM_BUFFER, 128, &orbit, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform_buffer);



		// GLuint vertex_buffer;
		glCreateBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_DYNAMIC_DRAW);

		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_shader_code_opengl, nullptr);
		glCompileShader(vertex_shader);

		GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_shader_code_opengl, nullptr);
		glCompileShader(fragment_shader);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		glUseProgram(program);



		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);



		loop_function = loop_function_GL;

		destroy_api_function = destroyGL;
	}
}

void initGL2 (const size_t& window_x, const size_t& window_y)
{
	if (destroy_api_function != destroyGL)
	{
		destroy_api_function();

		glfwInit();

		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

		window = glfwCreateWindow(1, 1, "", nullptr, nullptr);

		glfwHideWindow(window);

		glfwMakeContextCurrent(window);
		// glfwSwapInterval(1);



		gladLoadGL();



		glViewport(0, 0, window_x, window_y);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glClearColor(0.125f, 0.125f, 0.125f, 1.0f);



		GLuint uniform_buffer;
		glCreateBuffers(1, &uniform_buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer);
		glBufferData(GL_UNIFORM_BUFFER, 128, &orbit, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform_buffer);



		glCreateBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);



		// GLsizei l;
		// GLchar log;

		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_shader_code_opengl, nullptr);
		glCompileShader(vertex_shader);
		// glGetShaderInfoLog(vertex_shader, 0xffffffff, &l, &log);
		// cout << 1 << log << endl;

		GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_shader_code_opengl, nullptr);
		glCompileShader(fragment_shader);
		// glGetShaderInfoLog(fragment_shader, 0xffffffff, &l, &log);
		// cout << log << endl;

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);



		glEnableVertexAttribArray(0);



		glCreateRenderbuffers(1, &framebuffer_renderbuffer_color);
		glBindRenderbuffer(GL_RENDERBUFFER, framebuffer_renderbuffer_color);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, window_x, window_y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glCreateRenderbuffers(1, &framebuffer_renderbuffer_depth);
		glBindRenderbuffer(GL_RENDERBUFFER, framebuffer_renderbuffer_depth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, window_x, window_y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glCreateFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, framebuffer_renderbuffer_color);
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, framebuffer_renderbuffer_depth);
		// glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);



		glCreateBuffers(1, &pixel_pack_buffer);

		glBindBuffer(GL_PIXEL_PACK_BUFFER, pixel_pack_buffer);
		glBufferData(GL_PIXEL_PACK_BUFFER, window_x * window_y * 4, nullptr, GL_DYNAMIC_READ);

		// Redundant call. GL_COLOR_ATTACHMENT0 is a default framebuffer read buffer.
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glReadPixels(0, 0, window_x, window_y, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		pixel_data = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);

		// if (pixel_data)
		// {
		// 	glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
		// }



		loop_function = loop_function_GL2;

		destroy_api_function = destroyGL2;
	}
}
