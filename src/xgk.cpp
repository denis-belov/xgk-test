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



namespace XGK {

	namespace DATA {

		extern const uint8_t FLOAT_SIZE_4;
	};
};



using std::cout;
using std::endl;



namespace TIME {

	void idle_function (const float) {};



	struct Transition {

		// revise types
		uint8_t active;
		uint64_t time_gone;
		float interpolation;
		uint64_t duration;
		uint64_t stack_index;
		void* additional;

		void (* process_callback) (const float);
		void (* end_callback) (const float);
	};

	struct Time {

		std::vector<Transition*> stack_VECTOR;
		Transition** stack_RANGE;

		uint64_t frame_time;
		uint64_t stack_length;
		uint64_t stack_counter;

		Transition** stack;
	};



	void init (Time* time, const uint64_t stack_max_size) {

		time->frame_time = 0;

		time->stack_length = 0;
		time->stack_counter = 0;

		time->stack_VECTOR.resize(stack_max_size);
		time->stack_RANGE = time->stack_VECTOR.data();
	};



	// only cancels within stack execution, not for using manually
	void cancelTransition (Time* time, Transition* transition) {

		transition->active = 0;
		// transition->duration = 0;

		time->stack_RANGE[time->stack_counter--] = time->stack_RANGE[--time->stack_length];
		time->stack_RANGE[time->stack_length] = nullptr;
	};



	void cancelTransition2 (Time* time, Transition* transition) {

		if (transition->active) {

			transition->active = 0;

			time->stack_RANGE[transition->stack_index] = time->stack_RANGE[--time->stack_length];
			time->stack_RANGE[time->stack_length] = nullptr;
		}
	};



	void startTransition (

		Time* time,
		Transition* transition,
		uint64_t duration,
		void (* process_callback) (const float)
	) {

		cancelTransition2(time, transition);

		transition->interpolation = 0.0f;
		transition->duration = duration;
		transition->process_callback = process_callback;
		transition->end_callback = idle_function;
		transition->time_gone = 0;
		transition->active = 1;
		transition->stack_index = time->stack_length;

		time->stack_RANGE[time->stack_length] = transition;

		++time->stack_length;
	};



	void startTransition2 (
		Time* time,
		Transition* transition,
		uint64_t duration,
		void (* process_callback) (const float),
		void (* end_callback) (const float)
	) {

		cancelTransition2(time, transition);

		transition->interpolation = 0.0f;
		transition->duration = duration;
		transition->process_callback = process_callback;
		transition->end_callback = end_callback;
		transition->time_gone = 0;
		transition->active = 1;
		transition->stack_index = time->stack_length;

		time->stack_RANGE[time->stack_length++] = transition;
	};



	void updateTransition (Time* time, Transition* transition) {

		transition->interpolation = ((float) transition->time_gone) / ((float) transition->duration);

		// cout << ((int) (transition->time_gone >= transition->duration)) << " " << ((int) (transition->interpolation >= 1.0f)) << endl;

		if (transition->time_gone >= transition->duration) {
		// if (transition->interpolation >= 1.0f) {

			cancelTransition(time, transition);

			transition->end_callback(transition->interpolation);
		}
		else {

			transition->time_gone += time->frame_time;

			transition->process_callback(transition->interpolation);
		}
	};



	void updateTransitions (Time* time) {

		for (time->stack_counter = 0; time->stack_RANGE[time->stack_counter]; time->stack_counter++) {

			updateTransition(time, time->stack_RANGE[time->stack_counter]);
		}

		// cout << time->stack_counter << endl;
	};



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



	void calculateFrametime (Time* time) {

		static std::chrono::time_point<std::chrono::system_clock> program_time;
		static std::chrono::time_point<std::chrono::system_clock> last_program_time;

		program_time = std::chrono::system_clock::now();
		time->frame_time = std::chrono::duration_cast<std::chrono::nanoseconds>(program_time - last_program_time).count();
		last_program_time = program_time;
	};
};



namespace ORBIT {

	struct alignas(16) Orbit {

		alignas(16) float proj_mat[16];

		alignas(16) float view_mat[16];

		alignas(8) XGK::Object object;

		alignas(4) float speed_x;

		alignas(4) float speed_y;



		Orbit (void);

		void rotate (void);

		void update (void);
	};



	Orbit::Orbit (void) {

		speed_x = 0.0f;
		speed_y = 0.0f;

		XGK::DATA::MAT4::ident(&view_mat);
	};



	void Orbit::rotate (void) {

		object.postRotX(speed_x);
		object.preRotY(speed_y);
	};



	void Orbit::update (void) {

		object.update2();

		XGK::DATA::MAT4::copy(&view_mat, &object.mat);
		XGK::DATA::MAT4::invns(&view_mat);
	};
};



uint8_t gui_g = 0;



TIME::Time _time;
ORBIT::Orbit orbit;
TIME::Transition orbit_transition;
float bez[1000];



std::mutex orbit_mutex;
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

  float temp = 3.14f * bez[uint64_t(interpolation * 1000.0f)];

	// Is CPU branch prediction faster than setting "end_callback"?
	orbit.speed_x = orbit.speed_y = (temp < prev) ? temp : (temp - prev);

	orbit.rotate();

	prev = temp;
};



void transition_thread_function (void) {

	while (render_flag) {

		TIME::calculateFrametime(&_time);

		TIME::updateTransitions(&_time);
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

			cout << render_flag << endl;

			render_flag = 0;
		}
		else if (key == GLFW_KEY_X) {

			TIME::startTransition(&_time, &orbit_transition, 1000000000, test);
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

	XGK::DATA::MAT4::print(&orbit.proj_mat);
	XGK::DATA::MAT4::print(&orbit.view_mat);
	XGK::DATA::MAT4::print(&orbit.object.mat);



	// wrap into function
	// vkGetPhysicalDeviceFormatProperties(vulkan_context.physical_devices[0], VK_FORMAT_R32G32B32_SFLOAT, &vulkan_context.format_props);
	// std::cout << (vulkan_context.format_props.bufferFeatures & VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT) << std::endl;
	initGL();



	XGK::MATH::UTIL::makeBezierCurve3Sequence2(

		bez,
	  0,.98,0,.97,
	  1000
	);



	std::thread transition_thread(transition_thread_function);



	TIME::init(&_time, 8);

	while (render_flag) {

		TIME::getFramerate();
		// TIME::getAverageFrametime();

		glfwPollEvents();

		orbit.update();

		loop_function();
	}



	transition_thread.join();



	destroy_api_function();



	cout << "END" << endl;



	return 0;
};
