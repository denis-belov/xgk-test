#define DEBUG



#include <cstdint>
#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

#if defined(_WIN64)

	#include <windows.h>
#endif

#include "GLFW/glfw3.h"

#include "xgk-math/src/data/data.h"
#include "xgk-math/src/object/object.h"
#include "xgk-math/src/util/util.h"

#include "xgk-aux/src/transition-stack/transition-stack.h"
#include "xgk-aux/src/transition/transition.h"



namespace XGK {

	namespace DATA {

		extern const uint8_t FLOAT_SIZE_4;
	};
};



using std::cout;
using std::endl;



namespace TIME {

	void getFramerate () {

		static const std::chrono::time_point<std::chrono::system_clock> start_time = std::chrono::system_clock::now();
		static std::chrono::time_point<std::chrono::system_clock> program_time = std::chrono::system_clock::now();
		static uint64_t now_seconds = 0;
		static uint64_t last_seconds = 0;
		static uint64_t frames = 0;

		frames++;
		program_time = std::chrono::system_clock::now();
		now_seconds = std::chrono::duration_cast<std::chrono::seconds>(program_time - start_time).count();

		if (now_seconds - last_seconds) { // > 0

			last_seconds = now_seconds;

			#if defined(__linux__)

				printf("\x1B[32mFPS: %lu                             \e[?25l\x1B[0m\r", frames);
			#elif defined(_WIN64)

				printf("\x1B[32mFPS: %llu                            \x1B[0m\r", frames);
			#endif

			fflush(stdout);

			frames = 0;
		}
	};

	void getAverageFrametime () {

		static std::chrono::time_point<std::chrono::system_clock> start_time = std::chrono::system_clock::now();
		static std::chrono::time_point<std::chrono::system_clock> program_time = std::chrono::system_clock::now();
		static uint64_t now_seconds = 0;
		static uint64_t frames = 0;

		frames++;
		program_time = std::chrono::system_clock::now();
		now_seconds = std::chrono::duration_cast<std::chrono::seconds>(program_time - start_time).count();

		if (now_seconds) { // > 0

			#if defined(__linux__)

				printf("\x1B[32mAverage frametime: %fs                            \e[?25l\x1B[0m\r", (double) now_seconds / ((double) frames));
			#elif defined(_WIN64)

				printf("\x1B[32mAverage frametime: %fs                            \x1B[0m\r", (double) now_seconds / ((double) frames));
			#endif

			fflush(stdout);

			start_time = std::chrono::system_clock::now();
			program_time = std::chrono::system_clock::now();
			now_seconds = 0;
			frames = 0;
		}
	};

	// void getTime (Time* time) {

	// 	time->program_time = std::chrono::system_clock::now();
	// 	time->_time = std::chrono::duration_cast<std::chrono::microseconds>(time->program_time - time->last_program_time).count();
	// 	time->last_program_time = time->program_time;

	// 	// time->now_seconds = floor(((float) std::chrono::duration_cast<std::chrono::microseconds>(time->program_time - time->start_time).count()) * 0.000001f);
	// 	time->now_seconds = ((std::chrono::duration_cast<std::chrono::seconds>(time->program_time - time->start_time).count()));

	// 	printf("%f\n", time->_time);
	// };

	void calculateFrametime (uint64_t* frame_time) {

		static std::chrono::time_point<std::chrono::system_clock> program_time;
		static std::chrono::time_point<std::chrono::system_clock> last_program_time;

		program_time = std::chrono::system_clock::now();
		*frame_time = std::chrono::duration_cast<std::chrono::nanoseconds>(program_time - last_program_time).count();
		last_program_time = program_time;
	};
};



namespace ORBIT {

	struct alignas(16) Orbit {

		alignas(16) float proj_mat[16];

		alignas(16) float view_mat[16];

		alignas(8) XGK::Object object;

		alignas(4) float rotation_speed_x;

		alignas(4) float rotation_speed_y;

		alignas(4) float translation_speed_x;

		alignas(4) float translation_speed_y;



		Orbit (void);
		void rotate (void);
		void translate (void);
		void update (void);
	};

	Orbit::Orbit (void) {

		rotation_speed_x = 0.0f;
		rotation_speed_y = 0.0f;

		translation_speed_x = 0.0f;
		translation_speed_y = 0.0f;

		XGK::DATA::MAT4::ident(&view_mat);
	};

	void Orbit::rotate (void) {

		object.postRotX(rotation_speed_x);
		object.preRotY(rotation_speed_y);
	};

	void Orbit::translate (void) {

		object.transX(translation_speed_x);
	};

	void Orbit::update (void) {

		object.update2();

		XGK::DATA::MAT4::copy(&view_mat, &object.mat);
		XGK::DATA::MAT4::invns(&view_mat);
	};
};



uint8_t gui_g = 0;



XGK::Transition orbit_transition;
XGK::Transition orbit_transition2;
float curve_values[1000];

ORBIT::Orbit orbit;



// std::mutex orbit_mutex;
// replace by atomic
volatile uint8_t render_flag = 1;



const float vertices[] = {
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

const float* _vertices = vertices;

extern const uint32_t vertices_size = sizeof(vertices);



void test (const float interpolation) {

	static float prev = 0.0f;

  float temp = 3.14f * curve_values[uint64_t(interpolation * 1000.0f)];

	// Is CPU branch prediction faster than setting "end_callback"?
	orbit.rotation_speed_x = orbit.rotation_speed_y = (temp < prev) ? temp : (temp - prev);

	orbit.rotate();

	prev = temp;
};

void test2 (const float interpolation) {

	static float prev = 0.0f;

	float temp = 3.14f * curve_values[uint64_t(interpolation * 1000.0f)];

	orbit.translation_speed_x = orbit.translation_speed_y = (temp < prev) ? temp : (temp - prev);

	orbit.translate();

	prev = temp;
};



void transition_thread_function (XGK::TransitionStack* _stack) {

	XGK::TransitionStack stack = *_stack;

	while (render_flag) {

		stack.calculateFrametime();
		stack.update();
	}
};



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

void glfw_key_callback (GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (action != GLFW_RELEASE) {

		if (key == GLFW_KEY_ESCAPE) {

			render_flag = 0;
		}
		else if (key == GLFW_KEY_X) {

			orbit_transition.start2(1000000000, test);
		}
		else if (key == GLFW_KEY_C) {

			orbit_transition2.start2(1000000000, test2);
		}
		else if (key == GLFW_KEY_G) {

			initGL();
		}
		else if (key == GLFW_KEY_V) {

			initVK();
		}
		else if (key == GLFW_KEY_P) {

			gui_g = 1 - gui_g;
		}
	}
};

void glfw_error_callback (int error, const char* description) {

	fprintf(stderr, "Error: %s\n", description);
};



int main (void) {

	// hiding cursor
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



	XGK::DATA::VEC4::simd32();
	XGK::DATA::QUAT::simd32();
	XGK::DATA::MAT4::simd32();
	// XGK::DATA::VEC4::simd128();
	// XGK::DATA::QUAT::simd128();
	// XGK::DATA::MAT4::simd128();



	orbit.object.setTransZ(10.0f);
	orbit.update();

	XGK::DATA::MAT4::makeProjPersp(orbit.proj_mat, 45.0f, 800.0f / 600.0f, 1.0f, 2000.0f, 1.0f);



	// wrap into function
	// vkGetPhysicalDeviceFormatProperties(vulkan_context.physical_devices[0], VK_FORMAT_R32G32B32_SFLOAT, &vulkan_context.format_props);
	// std::cout << (vulkan_context.format_props.bufferFeatures & VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT) << std::endl;
	initVK();



	XGK::MATH::UTIL::makeBezierCurve3Sequence2(

		curve_values,
	  0,.98,0,.97,
	  1000
	);



	// const uint64_t thread_count = std::thread::hardware_concurrency() - 1;
	const uint64_t thread_count = 5;

	std::vector<XGK::TransitionStack*> stacks(thread_count);
	std::vector<std::thread*> threads(thread_count);

	for (uint64_t i = 0; i < thread_count; i++) {

		stacks[i] = new XGK::TransitionStack(64);
		threads[i] = new std::thread(transition_thread_function, stacks[i]);
	}



	while (render_flag) {

		TIME::getFramerate();
		// TIME::getAverageFrametime();

		glfwPollEvents();

		orbit.update();

		loop_function();
	}



	for (uint64_t i = 0; i < stacks.size(); i++) {

		threads[i]->join();
		delete threads[i];
		delete stacks[i];
	}



	destroy_api_function();



	cout << "END" << endl;



	return 0;
};
