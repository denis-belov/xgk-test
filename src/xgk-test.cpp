#define DEBUG



#include <cstdint>
#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <thread>
#include <mutex>
#include <unistd.h>

#if defined(_WIN64)
	#include <windows.h>
#endif

#include "GLFW/glfw3.h"

#include "xgk-math/src/mat4/mat4.h"
#include "xgk-math/src/object/object.h"
#include "xgk-math/src/orbit/orbit.h"
#include "xgk-math/src/util/util.h"

#include "xgk-aux/src/transition-stack/transition-stack.h"
#include "xgk-aux/src/transition/transition.h"
#include "xgk-aux/src/meas/meas.h"



using std::cout;
using std::endl;



uint8_t gui_g {};



XGK::Transition orbit_transition;
XGK::Transition orbit_transition2;
float curve_values [1000000];

XGK::MATH::Orbit orbit;



// std::mutex orbit_mutex;
// replace by atomic
volatile uint8_t render_flag { 1 };



extern const float vertices []
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
	1.0f,-1.0f, 1.0f,
};

extern const size_t vertices_size { sizeof(vertices) };



void test (const size_t& time_gone)
{
	static size_t prev_time {};

	float temp { curve_values[(time_gone / 1000) - 1] };

	if (time_gone < prev_time)
	{
		prev_time = 0;
	}

	orbit.rotation_speed_x = orbit.rotation_speed_y = temp * float(time_gone - prev_time);

	prev_time = time_gone;

	orbit.rotate();
}

void test2 (const size_t& time_gone)
{
	static size_t prev_time {};

	float temp { curve_values[(time_gone / 1000) - 1] };

	if (time_gone < prev_time)
	{
		prev_time = 0;
	}

	orbit.translation_speed_z = temp * float(time_gone - prev_time) * 10.0f;

	prev_time = time_gone;

	orbit.transZ();
}

void test3 (const size_t& time_gone)
{
	static size_t prev_time {};

	float temp { curve_values[(time_gone / 1000) - 1] };

	if (time_gone < prev_time)
	{
		prev_time = 0;
	}

	orbit.translation_speed_z = temp * float(time_gone - prev_time);

	prev_time = time_gone;

	orbit.transZ();
}



void thread_function (XGK::TransitionStack* _stack)
{
	XGK::TransitionStack& stack { *_stack };

	while (render_flag)
	{
		stack.calculateFrametime();

		if (stack.frame_time < 100000)
		{
			continue;
		}

		stack.update();
	}
}



void idle_function (void) {}

// rename
using idle_function_t = void (*) (void);

idle_function_t loop_function { idle_function };
idle_function_t destroy_api_function { idle_function };



void loop_function_VK (void);
void destroyVK (void);
void initVK (void);

void loop_function_GL (void);
void destroyGL (void);
void initGL (void);



void initOrbit ()
{
	orbit.object.setTransZ(10.0f);
	orbit.update();

	orbit.proj_mat.makeProjPersp(45.0f, 800.0f / 600.0f, 1.0f, 2000.0f, 1.0f);
}

void tran (const float& value_x, const float& value_y)
{
	// orbit.rotation_speed_x = orbit.rotation_speed_y = 0.5f;
	orbit.rotation_speed_y = value_x;
	orbit.rotation_speed_x = value_y;

	orbit.rotate();
	orbit.update();
	// orbit_transition.start2(1000000000, test);
}



GLFWwindow* window {};

void glfw_key_callback (GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_RELEASE)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			render_flag = 0;
		}
		else if (key == GLFW_KEY_X)
		{
			orbit_transition.start2(1000000000, test);
		}
		else if (key == GLFW_KEY_Z)
		{
			orbit_transition2.start2(1000000000, test2);
		}
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



int _main (void)
{
	// hiding cursor in Windows console
	#if defined(_WIN64)
		{
			HANDLE console { GetStdHandle(STD_OUTPUT_HANDLE) };
			CONSOLE_CURSOR_INFO console_cursor_info;

			console_cursor_info.bVisible { FALSE };
			console_cursor_info.dwSize { 1 };

			SetConsoleCursorInfo(console, &console_cursor_info);
		}
	#endif
	//



	initOrbit();



	// wrap into function
	// vkGetPhysicalDeviceFormatProperties(vulkan_context.physical_devices[0], VK_FORMAT_R32G32B32_SFLOAT, &vulkan_context.format_props);
	// std::cout << (vulkan_context.format_props.bufferFeatures & VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT) << std::endl;
	initGL();



	XGK::MATH::UTIL::makeBezierCurve3Sequence2
	(
		curve_values,
		// .49,.23,.51,.98,
		0,.5,.5,1,
		// 0, 1, 1, 1,
		1000000
	);

	XGK::MATH::UTIL::makeBezierCurve3Sequence
	(
		curve_values,

		// 0.0000001f, 0.0000001f, 0.0000001f, 0.0f, 0.00000005f, 0.0f, 0.0f, 0.0f,

		1.0f * 3.14f * 0.000000001f,
		1.0f * 3.14f * 0.000000001f,
		1.0f * 3.14f * 0.000000001f,
		0.0f,
		0.5f * 3.14f * 0.000000001f,
		0.0f,
		0.0f,
		0.0f,

		// 1.0f * 3.14f / 1000000000.0f,
		// 1.0f * 3.14f / 1000000000.0f,
		// 1.0f * 3.14f / 1000000000.0f,
		// 1.0f * 3.14f / 1000000000.0f,
		// 1.0f * 3.14f / 1000000000.0f,
		// 1.0f * 3.14f / 1000000000.0f,
		// 1.0f * 3.14f / 1000000000.0f,
		// 1.0f * 3.14f / 1000000000.0f,


		1000000
	);



	// for (size_t i {}; i < 1000; ++i)
	// {
	// 	cout << curve_values[i] << endl;
	// }



	// // const size_t thread_count = std::thread::hardware_concurrency() - 1;
	const size_t thread_count { 5 };

	std::vector<XGK::TransitionStack*> stacks(thread_count);
	std::vector<std::thread*> threads(thread_count);

	for (size_t i {}; i < thread_count; ++i)
	{
		stacks[i] = new XGK::TransitionStack(64);
		threads[i] = new std::thread(thread_function, stacks[i]);
	}



	// size_t frame_time {};

	// while (render_flag)
	// {
	// 	// XGK::AUX::MEAS::printFramerate();
	// 	// XGK::AUX::MEAS::printAverageFrametime();
	// 	// XGK::AUX::MEAS::calculateFrametime(&frame_time);

	// 	glfwPollEvents();

	// 	orbit.update();

	// 	loop_function();
	// }



	for (size_t i {}; i < thread_count; ++i)
	{
		threads[i]->join();

		delete threads[i];
		delete stacks[i];
	}



	destroy_api_function();



	cout << "END" << endl;



	return 0;
}
