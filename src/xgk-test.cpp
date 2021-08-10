#define DEBUG



#include <cstdint>
#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <thread>
#include <mutex>

#if defined(_WIN64)
	#include <windows.h>
#endif

#include "GLFW/glfw3.h"

#include "xgk-math/src/mat4/mat4.h"
#include "xgk-math/src/object/object.h"
#include "xgk-math/src/orbit/orbit.h"

#include "xgk-aux/src/meas/meas.h"
// #include "xgk-math/src/util/util.h"

// #include "xgk-aux/src/transition-stack/transition-stack.h"
// #include "xgk-aux/src/transition/transition.h"



using std::cout;
using std::endl;



uint8_t gui_g = 0;



// XGK::Transition orbit_transition;
// XGK::Transition orbit_transition2;
float curve_values[1000000];

XGK::MATH::Orbit orbit;



// std::mutex orbit_mutex;
// replace by atomic
volatile uint8_t render_flag = 1;



const float vertices[] =
{
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};

// const float vertices[] =
// {
// 	-1.0f, -1.0f, 0.0f,
// 	0.0f, 1.0f, 0.0f,
// 	1.0f, -1.0f, 0.0f
// };

const float* _vertices = vertices;

extern const uint32_t vertices_size = sizeof(vertices);



// void test (const float interpolation)
// {
// 	static float prev = 0.0f;
// 	static float prev_i = 0.0f;

//   // float temp = M_PI * curve_values[uint64_t(interpolation * 1000.0f)];
// 	float temp = M_PI * BezierCubicCurve(interpolation, 0, 1, 0, 1);

// 	// Is CPU branch prediction faster than setting "end_callback"?
// 	orbit.rotation_speed_x = orbit.rotation_speed_y = (interpolation < prev_i) ? temp : (temp - prev);

// 	orbit.rotate();

// 	prev = temp;
// 	prev_i = interpolation;
// };

// void test2 (const float interpolation)
// {
// 	static float prev = 0.0f;

// 	// float temp = M_PI * curve_values[uint64_t(interpolation * 1000.0f)];
// 	float temp = M_PI * BezierCubicCurve(interpolation, 0, 1, 0, 1);

// 	// orbit.translation_speed_x = orbit.translation_speed_y = (temp < prev) ? temp : (temp - prev);
// 	orbit.translation_speed_x = temp - prev * ceil(1.0f - interpolation);

// 	orbit.transX();

// 	prev = temp;
// };

// void test3 (const float interpolation)
// {
// 	static float prev = 0.0f;

// 	float temp = -3.14f * curve_values[uint64_t(interpolation * 1000.0f)];

// 	// orbit.translation_speed_x = orbit.translation_speed_y = (temp < prev) ? temp : (temp - prev);
// 	orbit.translation_speed_x = temp - prev * ceil(1.0f - interpolation);

// 	orbit.transX();

// 	prev = temp;
// };



// void thread_function (XGK::TransitionStack* _stack)
// {
// 	XGK::TransitionStack& stack = *_stack;

// 	while (render_flag)
// 	{
// 		stack.calculateFrametime();
// 		stack.update();
// 	}
// };



void idle_function (void) {};

void (* loop_function) (void) = idle_function;

void (* destroy_api_function) (void) = idle_function;



void loop_function_VK (void);
void destroyVK (void);
void initVK (void);

void loop_function_GL (void);
void destroyGL (void);
void initGL (void);



GLFWwindow* window = nullptr;

void glfw_key_callback (GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_RELEASE)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			render_flag = 0;
		}
		// else if (key == GLFW_KEY_X)
		// {
		// 	orbit_transition.start2(1000000000, test);
		// }
		// else if (key == GLFW_KEY_A)
		// {
		// 	orbit_transition2.start2(1000000000, test2);
		// }
		// else if (key == GLFW_KEY_D)
		// {
		// 	orbit_transition2.start2(1000000000, test3);
		// }
		else if (key == GLFW_KEY_G)
		{
			initGL();
		}
		else if (key == GLFW_KEY_V)
		{
			initVK();
		}
		else if (key == GLFW_KEY_P)
		{
			gui_g = !gui_g;
		}
	}
}

void glfw_error_callback (int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}



int main (void)
{
	// hiding cursor in Windows console
	#if defined(_WIN64)
		{
			HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
			CONSOLE_CURSOR_INFO console_cursor_info;

			console_cursor_info.bVisible = FALSE;
			console_cursor_info.dwSize = 1;

			SetConsoleCursorInfo(console, &console_cursor_info);
		}
	#endif
	//



	orbit.object.setTransZ(10.0f);
	// orbit.object.preRotY(0.1f);
	orbit.update();

	orbit.proj_mat.makeProjPersp(45.0f, 800.0f / 600.0f, 1.0f, 2000.0f, 1.0f);



	// wrap into function
	// vkGetPhysicalDeviceFormatProperties(vulkan_context.physical_devices[0], VK_FORMAT_R32G32B32_SFLOAT, &vulkan_context.format_props);
	// std::cout << (vulkan_context.format_props.bufferFeatures & VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT) << std::endl;
	initGL();



	// XGK::MATH::UTIL::makeBezierCurve3Sequence2
	// (
	// 	curve_values,
	//   // 0,1,0,1,
	// 	.2,.97,.82,-0.97,
	//   1000000
	// );



	// // const uint64_t thread_count = std::thread::hardware_concurrency() - 1;
	// const uint64_t thread_count = 5;

	// std::vector<XGK::TransitionStack*> stacks(thread_count);
	// std::vector<std::thread*> threads(thread_count);

	// for (uint64_t i = 0; i < thread_count; ++i)
	// {
	// 	stacks[i] = new XGK::TransitionStack(64);
	// 	threads[i] = new std::thread(thread_function, stacks[i]);
	// }



	while (render_flag)
	{
		XGK::AUX::MEAS::printFramerate();
		// XGK::AUX::MEAS::printAverageFrametime();

		glfwPollEvents();

		orbit.object.preRotY(0.0001f);
		orbit.update();

		loop_function();
	}



	// for (uint64_t i = 0; i < thread_count; ++i)
	// {
	// 	threads[i]->join();
	// 	delete threads[i];
	// 	delete stacks[i];
	// }



	destroy_api_function();



	cout << "END" << endl;



	return 0;
}
