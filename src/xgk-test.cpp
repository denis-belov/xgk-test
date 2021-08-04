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

// #include "xgk-math/src/data/data.h"
#include "xgk-math/src/data/mat4/mat4.h"
#include "xgk-math/src/object/object.h"
#include "xgk-math/src/util/util.h"

#include "xgk-aux/src/transition-stack/transition-stack.h"
#include "xgk-aux/src/transition/transition.h"



namespace XGK
{
	namespace DATA
	{
		extern const uint8_t FLOAT_SIZE_4;
	};
};



using std::cout;
using std::endl;



namespace TIME
{
	void getFramerate (void)
	{
		using std::chrono;

		static const time_point<system_clock> start_time = system_clock::now();
		static time_point<system_clock> program_time = system_clock::now();
		static uint64_t now_seconds = 0;
		static uint64_t last_seconds = 0;
		static uint64_t frames = 0;

		++frames;
		program_time = system_clock::now();
		now_seconds = duration_cast<seconds>(program_time - start_time).count();

		if ((now_seconds - last_seconds) != 0)
		{
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

	void getAverageFrametime (void)
	{
		using std::chrono;

		static time_point<system_clock> start_time = system_clock::now();
		static time_point<system_clock> program_time = system_clock::now();
		static uint64_t now_seconds = 0;
		static uint64_t frames = 0;

		++frames;
		program_time = system_clock::now();
		now_seconds = duration_cast<seconds>(program_time - start_time).count();

		if (now_seconds != 0)
		{
			#if defined(__linux__)
				printf("\x1B[32mAverage frametime: %fs                            \e[?25l\x1B[0m\r", (double) now_seconds / ((double) frames));
			#elif defined(_WIN64)
				printf("\x1B[32mAverage frametime: %fs                            \x1B[0m\r", (double) now_seconds / ((double) frames));
			#endif

			fflush(stdout);

			start_time = system_clock::now();
			program_time = system_clock::now();
			now_seconds = 0;
			frames = 0;
		}
	};

	void getTime (void)
	{
		using std::chrono;

		static time_point<system_clock> program_time;
		static time_point<system_clock> last_program_time;

		program_time = system_clock::now();

		const uint64_t nanoseconds = duration_cast<nanoseconds>(program_time - last_program_time).count();

		last_program_time = program_time;
	};

	void getTime2 (void)
	{
		using std::chrono;

		static time_point<system_clock> program_time;
		static time_point<system_clock> last_program_time;

		program_time = system_clock::now();

		const uint64_t nanoseconds = duration_cast<nanoseconds>(program_time - last_program_time).count();

		last_program_time = program_time;

		#if defined(__linux__)
			printf("Time: %lu\n", nanoseconds);
		#elif defined(_WIN64)
			printf("Time: %llu\n", nanoseconds);
		#endif
	};

	void calculateFrametime (uint64_t* frame_time)
	{
		using std::chrono;

		static time_point<system_clock> program_time;
		static time_point<system_clock> last_program_time;

		program_time = system_clock::now();

		*frame_time = duration_cast<nanoseconds>(program_time - last_program_time).count();

		last_program_time = program_time;
	};
};



namespace ORBIT
{
	alignas(16) struct Orbit
	{
		XGK::DATA::Mat4 proj_mat;
		XGK::DATA::Mat4 view_mat;

		alignas(8) XGK::Object object;

		alignas(4) float rotation_speed_x;
		alignas(4) float rotation_speed_y;

		alignas(4) float translation_speed_x;
		alignas(4) float translation_speed_y;
		alignas(4) float translation_speed_z;



		Orbit (void);
		void rotate (void);
		void transX (void);
		void transZ (void);
		void update (void);
	};

	Orbit::Orbit (void)
	{
		rotation_speed_x = 0.0f;
		rotation_speed_y = 0.0f;

		translation_speed_x = 0.0f;
		translation_speed_y = 0.0f;
		translation_speed_z = 0.0f;
	}

	void Orbit::rotate (void)
	{
		object.postRotX(rotation_speed_x);
		object.preRotY(rotation_speed_y);
	}

	void Orbit::transX (void)
	{
		object.transX(translation_speed_x);
	}

	void Orbit::transZ (void)
	{
		object.transZ(translation_speed_z);
	}

	void Orbit::update (void)
	{
		object.update2();

		view_mat = object.mat;

		view_mat.invns();
	}
};



uint8_t gui_g = 0;



XGK::Transition orbit_transition;
XGK::Transition orbit_transition2;
float curve_values[1000000];

ORBIT::Orbit orbit;



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

const float* _vertices = vertices;

extern const uint32_t vertices_size = sizeof(vertices);



float BezierCubicCurve (const float time, const float x1, const float y1, const float x2, const float y2)
{
	// B(t) = (1-t)^3*p0 + 3*t*(1-t)^2*p1 + 3*t^2*(1-t)*p2 + t^3*p3
	// x(t) = (1-t)^3*x0 + 3*t*(1-t)^2*x1 + 3*t^2*(1-t)*x2 + t^3*x3
	// y(t) = (1-t)^3*y0 + 3*t*(1-t)^2*y1 + 3*t^2*(1-t)*y2 + t^3*y3
	// p0 = vec2(0,0), p3 = vec2(1,1)
	// x(t) = 3*t*(1-t)^2*x1 + 3*t^2*(1-t)*x2 + t^3
	const float a = 3 * x1 - 3 * x2 + 1;
	const float b = 3 * x2 - 6 * x1;
	const float c = 3 * x1;
	const float d = - time;
	// x(t) = a*t^3 + b*t^2 + c*t
	// x(t) = time => a*t^3 + b*t^2 + c*t - d = 0
	const float A = b / a;
	const float B = c / a;
	const float C = d / a;
	const float Q = ( 3 * B - A*A ) / 9;
	const float R = ( 9 * A * B - 27 * C - 2 * A*A*A ) / 54;
	const float D = Q*Q*Q + R*R; // discriminant
	float t;

	if ( D >= 0 )
	{
		// float asd = 0;
		// if (R + sqrt( D ) > 0) {

		// 	asd = 1;
		// }
		// else if (R + sqrt( D ) < 0) {

		// 	asd = -1;
		// }

		// float qwe = 0;
		// if (R - sqrt( D ) > 0) {

		// 	qwe = 1;
		// }
		// else if (R - sqrt( D ) < 0) {

		// 	qwe = -1;
		// }

		// const float S = asd * pow( abs( R + sqrt( D ) ), 1 / 3 );
		// const float T = qwe * pow( abs( R - sqrt( D ) ), 1 / 3 );
		const float sqrtD = sqrt( D );
		const float S = cbrt(R + sqrtD);
		const float T = cbrt(R - sqrtD);
		t = - A / 3 + ( S + T );
	}
	else
	{
		const float th = acos( R / sqrt( - pow( Q, 3 ) ) );
		const float t1 = 2 * sqrt( - Q ) * cos( th / 3 ) - A / 3;
		const float t2 = 2 * sqrt( - Q ) * cos( th / 3 + 2 * M_PI / 3 ) - A / 3;
		const float t3 = 2 * sqrt( - Q ) * cos( th / 3 - 2 * M_PI / 3 ) - A / 3;
		if ( t1 >= 0 && t1 <= 1 ) t = t1;
		else if ( t2 >= 0 && t2 <= 1 ) t = t2;
		else if ( t3 >= 0 && t3 <= 1 ) t = t3;
	}
	float y;
	if ( t == 0 ) y = 0;
	else if ( t == 1 ) y = 1;
	else y = 3 * t * pow( 1 - t, 2 ) * y1 + 3 * t*t * ( 1 - t ) * y2 + t*t*t;
	return y;
};

void test (const float interpolation)
{
	static float prev = 0.0f;
	static float prev_i = 0.0f;

  // float temp = M_PI * curve_values[uint64_t(interpolation * 1000.0f)];
	float temp = M_PI * BezierCubicCurve(interpolation, 0, 1, 0, 1);

	// Is CPU branch prediction faster than setting "end_callback"?
	orbit.rotation_speed_x = orbit.rotation_speed_y = (interpolation < prev_i) ? temp : (temp - prev);

	orbit.rotate();

	prev = temp;
	prev_i = interpolation;
};

void test2 (const float interpolation)
{
	static float prev = 0.0f;

	// float temp = M_PI * curve_values[uint64_t(interpolation * 1000.0f)];
	float temp = M_PI * BezierCubicCurve(interpolation, 0, 1, 0, 1);

	// orbit.translation_speed_x = orbit.translation_speed_y = (temp < prev) ? temp : (temp - prev);
	orbit.translation_speed_x = temp - prev * ceil(1.0f - interpolation);

	orbit.transX();

	prev = temp;
};

void test3 (const float interpolation)
{
	static float prev = 0.0f;

	float temp = -3.14f * curve_values[uint64_t(interpolation * 1000.0f)];

	// orbit.translation_speed_x = orbit.translation_speed_y = (temp < prev) ? temp : (temp - prev);
	orbit.translation_speed_x = temp - prev * ceil(1.0f - interpolation);

	orbit.transX();

	prev = temp;
};



void thread_function (XGK::TransitionStack* _stack)
{
	XGK::TransitionStack& stack = *_stack;

	while (render_flag)
	{
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
		else if (key == GLFW_KEY_A)
		{
			orbit_transition2.start2(1000000000, test2);
		}
		else if (key == GLFW_KEY_D)
		{
			orbit_transition2.start2(1000000000, test3);
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
			gui_g = 1 - gui_g;
		}
	}
};

void glfw_error_callback (int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
};



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



	// XGK::DATA::VEC4::simd32();
	// XGK::DATA::QUAT::simd32();
	// XGK::DATA::MAT4::simd32();
	// XGK::DATA::VEC4::simd128();
	// XGK::DATA::QUAT::simd128();
	// XGK::DATA::MAT4::simd128();



	orbit.object.setTransZ(10.0f);
	orbit.update();

	orbit.proj_mat.makeProjPersp(45.0f, 800.0f / 600.0f, 1.0f, 2000.0f, 1.0f);



	// wrap into function
	// vkGetPhysicalDeviceFormatProperties(vulkan_context.physical_devices[0], VK_FORMAT_R32G32B32_SFLOAT, &vulkan_context.format_props);
	// std::cout << (vulkan_context.format_props.bufferFeatures & VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT) << std::endl;
	initGL();



	XGK::MATH::UTIL::makeBezierCurve3Sequence2
	(
		curve_values,
	  // 0,1,0,1,
		.2,.97,.82,-0.97,
	  1000000
	);



	// const uint64_t thread_count = std::thread::hardware_concurrency() - 1;
	const uint64_t thread_count = 5;

	std::vector<XGK::TransitionStack*> stacks(thread_count);
	std::vector<std::thread*> threads(thread_count);

	for (uint64_t i = 0; i < thread_count; ++i)
	{
		stacks[i] = new XGK::TransitionStack(64);
		threads[i] = new std::thread(thread_function, stacks[i]);
	}



	while (render_flag)
	{
		// TIME::getFramerate();
		// TIME::getAverageFrametime();

		glfwPollEvents();

		orbit.update();

		loop_function();
	}



	float a = 0.0f;
	float b = 0.0f;

	std::chrono::time_point<std::chrono::system_clock> t1 = std::chrono::system_clock::now();

	for (size_t i = 0; i < 9999999; ++i)
	{
		b += BezierCubicCurve(((float) i) / 9999999.0f, .2,.97,.82,-0.97);
	}

	std::chrono::time_point<std::chrono::system_clock> t2 = std::chrono::system_clock::now();

	for (size_t i = 0; i < 9999999; ++i)
	{
		a += curve_values[uint64_t(((float) i) / 9999999.0f * 1000000.0f)];
	}

	std::chrono::time_point<std::chrono::system_clock> t3 = std::chrono::system_clock::now();

	cout << "a: " << a << " " << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() << endl;
	cout << "b: " << b << " " << std::chrono::duration_cast<std::chrono::nanoseconds>(t3 - t2).count() << endl;



	for (uint64_t i = 0; i < thread_count; ++i)
	{
		threads[i]->join();
		delete threads[i];
		delete stacks[i];
	}



	destroy_api_function();



	cout << "END" << endl;



	return 0;
};
