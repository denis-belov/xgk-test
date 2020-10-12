#define DEBUG



#include <cstdint>
#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

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

	void idle_function (const float, void*) {};



	struct Transition {

		uint8_t active;
		float time_gone;
		float interpolation;
		float duration;
		uint64_t stack_index;
		void* additional;

		void (* process_callback) (const float, void*);
		void (* end_callback) (const float, void*);
	};

	struct Time {

		std::vector<Transition*> stack_VECTOR;
		Transition** stack_RANGE;

		std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
		std::chrono::time_point<std::chrono::high_resolution_clock> program_time;
		std::chrono::time_point<std::chrono::high_resolution_clock> last_program_time;
		uint64_t now_seconds;
		uint64_t last_seconds;
		uint64_t frames = 0;
		uint64_t frame_time;
		uint64_t stack_length;
		uint64_t stack_counter;

		double _time;

		Transition** stack;
	};



	void init (Time* time, const uint64_t stack_max_size) {

		time->start_time = std::chrono::high_resolution_clock::now();
		time->last_program_time = std::chrono::high_resolution_clock::now();
		time->frame_time = 0.0f;
		time->now_seconds = 0.0f;
		time->last_seconds = 0.0f;

		time->stack_length = 0;
		time->stack_counter = 0;

		time->stack_VECTOR.resize(stack_max_size);
		time->stack_RANGE = time->stack_VECTOR.data();
	};



	// only cancels within stack execution, not for using manually
	void cancelTransition (Time* time, Transition* transition) {

		// if (transition->active) {

			transition->active = 0;

			time->stack_RANGE[time->stack_counter--] = time->stack_RANGE[--time->stack_length];
			time->stack_RANGE[time->stack_length] = nullptr;
		// }

		// printf("\33[2J");
		// std::cout << "update transition count: " << qwe << std::endl;
		// qwe = 0;
	};



	void cancelTransition2 (Time* time, Transition* transition) {

		if (transition->active) {

			transition->active = 0;

			time->stack_RANGE[transition->stack_index] = time->stack_RANGE[--time->stack_length];
			time->stack_RANGE[time->stack_length] = nullptr;
		}
	};



	void setTransition (
		Time* time,
		Transition* transition,
		const float duration,
		void (* process_callback) (const float, void*),
		void (* end_callback) (const float, void*)
	) {

		cancelTransition2(time, transition);

		transition->interpolation = 0.0f;
		transition->duration = duration;
		transition->process_callback = process_callback;
		transition->end_callback = end_callback;
		transition->time_gone = 0.0f;
		transition->active = 1;
		transition->stack_index = time->stack_length;

		time->stack_RANGE[time->stack_length++] = transition;
	};



	void setTransition2 (

		Time* time,
		Transition* transition,
		const float duration,
		void (* process_callback) (const float, void*)
	) {

		cancelTransition2(time, transition);

		transition->interpolation = 0.0f;
		transition->duration = duration;
		transition->process_callback = process_callback;
		transition->end_callback = idle_function;
		transition->time_gone = 0.0f;
		transition->active = 1;
		transition->stack_index = time->stack_length;

		time->stack_RANGE[time->stack_length++] = transition;
	};



	void updateTransition (Time* time, Transition* transition) {

		transition->interpolation = transition->time_gone / transition->duration;

		// qwe++;

		if (transition->interpolation >= 1.0) {

			cancelTransition(time, transition);

			transition->end_callback(transition->interpolation, transition->additional);
		}
		else {

			transition->time_gone += time->frame_time;

			transition->process_callback(transition->interpolation, transition->additional);
		}
	};



	void updateTransitions (Time* time) {

		for (time->stack_counter = 0; time->stack_RANGE[time->stack_counter]; time->stack_counter++) {

			updateTransition(time, time->stack_RANGE[time->stack_counter]);
		}
	};



	void getFrameTime (Time* time) {

		time->frames++;

		time->program_time = std::chrono::high_resolution_clock::now();
		time->frame_time = std::chrono::duration_cast<std::chrono::microseconds>(time->program_time - time->last_program_time).count();
		time->last_program_time = time->program_time;

		// time->now_seconds = floor(((float) std::chrono::duration_cast<std::chrono::microseconds>(time->program_time - time->start_time).count()) * 0.000001f);
		time->now_seconds = ((std::chrono::duration_cast<std::chrono::seconds>(time->program_time - time->start_time).count()));

		if (time->now_seconds - time->last_seconds) { // > 0

			time->last_seconds = time->now_seconds;

			// printf("\x1B[32mFPS: %f                           \e[?25l\x1B[0m\r", 1.0f / (((float) time->frame_time) * 0.000001f));
			printf("\x1B[32mFPS: %lu                           \e[?25l\x1B[0m\r", time->frames);
			fflush(stdout);

			time->frames = 0;
		}
	};



	void getTime (Time* time) {

		time->frames++;

		time->program_time = std::chrono::high_resolution_clock::now();
		time->_time = std::chrono::duration_cast<std::chrono::microseconds>(time->program_time - time->last_program_time).count();
		time->last_program_time = time->program_time;

		// time->now_seconds = floor(((float) std::chrono::duration_cast<std::chrono::microseconds>(time->program_time - time->start_time).count()) * 0.000001f);
		time->now_seconds = ((std::chrono::duration_cast<std::chrono::seconds>(time->program_time - time->start_time).count()));

		printf("%f\n", time->_time);
	};
};



namespace ORBIT {

	struct Orbit {

		alignas(16) float proj_mat[16];

		alignas(16) float view_mat[16];

		alignas(8) XGK::Object* object;

		alignas(8) TIME::Transition* transition;

		alignas(4) float speed_x;

		alignas(4) float speed_y;

		alignas(16) float prev_quat[4];
	};



	void init (Orbit* orbit_addr, XGK::Object* object_addr, TIME::Transition* transition_addr) {

		orbit_addr->object = object_addr;
		orbit_addr->transition = transition_addr;
		orbit_addr->transition->additional = orbit_addr;
		orbit_addr->speed_x = 0.0f;
		orbit_addr->speed_y = 0.0f;

		XGK::DATA::MAT4::ident(orbit_addr->view_mat);

		XGK::OBJECT::init(orbit_addr->object);
	};



	void update (Orbit* orbit_addr) {

		XGK::OBJECT::update2(orbit_addr->object);

		XGK::DATA::MAT4::invns(orbit_addr->object);
		XGK::DATA::MAT4::copy(orbit_addr, orbit_addr->object);
	};



	void rotate (Orbit* orbit_addr) {

		memcpy(orbit_addr->object->quat, orbit_addr->prev_quat, 16);

		XGK::OBJECT::postRotX(orbit_addr->object, orbit_addr->speed_x);
		XGK::OBJECT::preRotY(orbit_addr->object, orbit_addr->speed_y);
	};



	// void test (const float interpolation, void* additional) {

	//   Orbit* orbit_addr = (Orbit*) additional;

	//   rotate(orbit_addr);
	//   // update(orbit_addr);

	//   float temp = 0.000001f * pow(1.0f - interpolation, 2.0);

	//   orbit_addr->speed_x = temp;
	//   orbit_addr->speed_y = temp;
	// };



	void move (Orbit* orbit_addr, TIME::Time* time, const float movement_x, const float movement_y, void (* test) (const float, void*)) {

		memcpy(orbit_addr->prev_quat, orbit_addr->object->quat, 16);

		TIME::setTransition2(time, orbit_addr->transition, 1000.0f, test);
	};
};



uint8_t gui_g = 0;



TIME::Time _time;
ORBIT::Orbit orbit;
float bez[1000];



std::mutex orbit_mutex;
// replace by atomic
volatile uint8_t render_flag = 1;



// XGK::Object orbit_object;
// XGK::Transition orbit_transition;
// uint8_t orbit_view_mat_flag = 1;

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



// void test (const float interpolation, void* additional) {

//   XGK::Orbit* orbit_addr = (XGK::Orbit*) additional;

//   float temp = 3.14f * bez[uint64_t(interpolation * 1000.0f)];

//   orbit_addr->speed_x = temp;
//   orbit_addr->speed_y = temp;

//   orbit_mutex.lock();

//     XGK::ORBIT::rotate(orbit_addr);

//   orbit_mutex.unlock();
// };



void transition_thread_function (void) {

	while (render_flag) {

		// XGK::TIME::getFrameTime(&time_);
		// TIME::updateTransitions(&_time);
	}
};

// void transition_thread (void) {

//   while (render_flag) {

//     XGK::TIME::updateTransitions(&time_);
//   }
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

void glfw_key_callback (GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (action != GLFW_RELEASE) {

		if (key == GLFW_KEY_ESCAPE) {

			cout << render_flag << endl;

			render_flag = 0;
		}
		else if (key == GLFW_KEY_X) {

			// orbit_mutex.lock();

			//   XGK::ORBIT::move(&orbit, &time_, 0.001f, 0.001f, test);

			// orbit_mutex.unlock();
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

	XGK::DATA::VEC4::simd32();
	XGK::DATA::QUAT::simd32();
	XGK::DATA::MAT4::simd32();

	XGK::DATA::MAT4::ident(&orbit.view_mat);
	float orbit_trans[3] = { 0.0, 0.0, 10 };
	XGK::DATA::MAT4::makeTrans(&orbit.view_mat, orbit_trans);
	XGK::DATA::MAT4::invns(&orbit.view_mat);
	XGK::DATA::MAT4::makeProjPersp(&orbit.proj_mat, 45.0f, 800.0f / 600.0f, 1.0f, 2000.0f, 1.0f);

	// XGK::DATA::MAT4::print(&orbit.proj_mat);

	// orbit.view_mat.ident();
	// orbit.view_mat.makeTrans(orbit_trans);
	// orbit.view_mat.invns();

	// orbit.proj_mat.makeProjPersp(45.0f, 800.0f / 600.0f, 1.0f, 2000.0f, 1.0f);

	// orbit.proj_mat.print();



	// XGK::ORBIT::init(&orbit, &orbit_object, &orbit_transition);
	// XGK::OBJECT::setTransZ(orbit.object, 10.0f);
	// XGK::ORBIT::update(&orbit);
	// XGK::DATA::MAT4::makeProjPersp(orbit.proj_mat, 45.0f, 800.0f / 600.0f, 1.0f, 2000.0f, 1.0f);

	// orbit.speed_x = 0.1f;
	// orbit.speed_y = 0.1f;
	// XGK::ORBIT::rotate(&orbit);
	// XGK::ORBIT::update(&orbit);



	// wrap into function
	// vkGetPhysicalDeviceFormatProperties(vulkan_context.physical_devices[0], VK_FORMAT_R32G32B32_SFLOAT, &vulkan_context.format_props);
	// std::cout << (vulkan_context.format_props.bufferFeatures & VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT) << std::endl;
	initVK();



	// XGK::MATH::UTIL::makeBezierCurve3Sequence2(

	// 	bez,
	//   0,.98,0,.97,
	//   1000
	// );



	// std::thread transition_thread(transition_thread_function);



	TIME::init(&_time, 8);

	while (render_flag) {

		glfwPollEvents();

		getFrameTime(&_time);

		// orbit_mutex.lock();

		//   ORBIT::update(&orbit);

		// orbit_mutex.unlock();

		loop_function();
	}



	// transition_thread.join();

	// for (uint64_t i = 0; i < spawned_threads.size(); i++) {

	//   spawned_threads[i].join();
	// }



	destroy_api_function();



	cout << "END" << endl;



	return 0;
};
