# build and run command
# clear && make && $(ROOT)/reps/denis-belov/c-test/xgk/build/gnu-x64/bin/xgk
#





BUILD_TYPE=gnu-x64

ROOT=~

DIR=$(ROOT)/reps/denis-belov/$(BUILD_TYPE)

SRC=$(DIR)/src

BUILD=$(DIR)/build/$(BUILD_TYPE)

XGK_MATH_BUILD=$(ROOT)/reps/denis-belov/xgk-math/build/$(BUILD_TYPE)

XGK_MATH_SRC=$(ROOT)/reps/denis-belov/xgk-math/src

XGK_AUX_SRC=$(ROOT)/reps/denis-belov/xgk-aux/src

_MAKE=make

#file extensions
a=a
o=o
s=s

makeBuild=echo mkdir -p $(DIR)/build && mkdir -p $(BUILD)
makeBin=mkdir -p $(BUILD)/$(bin)
makeO=mkdir -p $(BUILD)/$(o)
makeS=mkdir -p $(BUILD)/$(s)
makeSpv=mkdir -p $(BUILD)/spv
makeH=mkdir -p $(BUILD)/h

COMPILER_ARG=\
	-std=c++11 \
	-c \
	-m64 -msse3 -Ofast -funroll-loops \
	-fdiagnostics-color=always \



# linking
$(BUILD)/bin/xgk :\
$(BUILD)/$(o)/glad.$(o) \
$(BUILD)/$(o)/imgui.$(o) \
$(BUILD)/$(o)/imgui_widgets.$(o) \
$(BUILD)/$(o)/imgui_draw.$(o) \
$(BUILD)/$(o)/imgui_impl_glfw.$(o) \
$(BUILD)/$(o)/imgui_impl_opengl3.$(o) \
$(BUILD)/$(o)/imgui_impl_vulkan.$(o) \
$(XGK_MATH_BUILD)/$(a)/xgk-math.$(a) \
$(BUILD)/$(o)/gl.$(o) \
$(BUILD)/$(o)/vk.$(o) \
$(BUILD)/$(o)/xgk.$(o)
	$(makeBuild) && $(makeBin) && $(makeS) && \
	g++ -o $(BUILD)/bin/xgk \
	$(BUILD)/$(o)/glad.$(o) \
	$(BUILD)/$(o)/imgui.$(o) \
	$(BUILD)/$(o)/imgui_widgets.$(o) \
	$(BUILD)/$(o)/imgui_draw.$(o) \
	$(BUILD)/$(o)/imgui_impl_glfw.$(o) \
	$(BUILD)/$(o)/imgui_impl_opengl3.$(o) \
	$(BUILD)/$(o)/imgui_impl_vulkan.$(o) \
	$(XGK_MATH_BUILD)/$(a)/xgk-math.$(a) \
	$(BUILD)/$(o)/gl.$(o) \
	$(BUILD)/$(o)/vk.$(o) \
	$(BUILD)/$(o)/xgk.$(o) \
	-L $(ROOT)/libs/glfw-3.3.2/src -l glfw3 \
	-l dl \
	-l pthread \
	-l Xrandr -l Xxf86vm -l Xi -l Xinerama -l X11 -l Xcursor \
	-flto \
	-fdiagnostics-color=always \
	objdump -d -M intel -S $(BUILD)/bin/xgk > $(BUILD)/$(s)/build.$(s)



$(BUILD)/$(o)/glad.$(o) : $(ROOT)/libs/glad/src/glad.c
	$(makeBuild) && $(makeO) && $(makeS) && \
	gcc -o $(BUILD)/$(o)/glad.$(o) \
	$(ROOT)/libs/glad/src/glad.c \
	$(COMPILER_ARG) \
	-I $(ROOT)/libs/glad/include \
	objdump -d -M intel -S $(BUILD)/$(o)/glad.$(o) > $(BUILD)/$(s)/glad.$(s)



#GUI
$(BUILD)/$(o)/imgui.$(o) : $(ROOT)/libs/imgui/imgui.cpp
	$(makeBuild) && $(makeO) && $(makeS) && \
	g++ -o $(BUILD)/$(o)/imgui.$(o) \
	$(ROOT)/libs/imgui/imgui.cpp \
	$(COMPILER_ARG) \
	-I $(ROOT)/libs \
	-I $(ROOT)/libs/imgui \
	objdump -d -M intel -S $(BUILD)/$(o)/imgui.$(o) > $(BUILD)/$(s)/imgui.$(s)



$(BUILD)/$(o)/imgui_widgets.$(o) : $(ROOT)/libs/imgui/imgui_widgets.cpp
	$(makeBuild) && $(makeO) && $(makeS) && \
	g++ -o $(BUILD)/$(o)/imgui_widgets.$(o) \
	$(ROOT)/libs/imgui/imgui_widgets.cpp \
	$(COMPILER_ARG) \
	-I $(ROOT)/libs \
	-I $(ROOT)/libs/imgui \
	objdump -d -M intel -S $(BUILD)/$(o)/imgui_widgets.$(o) > $(BUILD)/$(s)/imgui_widgets.$(s)



$(BUILD)/$(o)/imgui_draw.$(o) : $(ROOT)/libs/imgui/imgui_draw.cpp
	$(makeBuild) && $(makeO) && $(makeS) && \
	g++ -o $(BUILD)/$(o)/imgui_draw.$(o) \
	$(ROOT)/libs/imgui/imgui_draw.cpp \
	$(COMPILER_ARG) \
	-I $(ROOT)/libs \
	-I $(ROOT)/libs/imgui \
	objdump -d -M intel -S $(BUILD)/$(o)/imgui_draw.$(o) > $(BUILD)/$(s)/imgui_draw.$(s)



$(BUILD)/$(o)/imgui_impl_glfw.$(o) : $(ROOT)/libs/imgui/examples/imgui_impl_glfw.cpp
	$(makeBuild) && $(makeO) && $(makeS) && \
	g++ -o $(BUILD)/$(o)/imgui_impl_glfw.$(o) \
	$(ROOT)/libs/imgui/examples/imgui_impl_glfw.cpp \
	$(COMPILER_ARG) \
	-I $(ROOT)/libs \
	-I $(ROOT)/libs/imgui \
	objdump -d -M intel -S $(BUILD)/$(o)/imgui_impl_glfw.$(o) > $(BUILD)/$(s)/imgui_impl_glfw.$(s)



$(BUILD)/$(o)/imgui_impl_opengl3.$(o) : $(ROOT)/libs/imgui/examples/imgui_impl_opengl3.cpp
	$(makeBuild) && $(makeO) && $(makeS) && \
	g++ -o $(BUILD)/$(o)/imgui_impl_opengl3.$(o) \
	$(ROOT)/libs/imgui/examples/imgui_impl_opengl3.cpp \
	$(COMPILER_ARG) \
	-I $(ROOT)/libs/imgui \
	-I $(ROOT)/libs/glad/include \
	objdump -d -M intel -S $(BUILD)/$(o)/imgui_impl_opengl3.$(o) > $(BUILD)/$(s)/imgui_impl_opengl3.$(s)



$(BUILD)/$(o)/imgui_impl_vulkan.$(o) : $(ROOT)/libs/imgui/examples/imgui_impl_vulkan.cpp
	$(makeBuild) && $(makeO) && $(makeS) && \
	g++ -o $(BUILD)/$(o)/imgui_impl_vulkan.$(o) \
	$(ROOT)/libs/imgui/examples/imgui_impl_vulkan.cpp \
	$(COMPILER_ARG) \
	-I $(ROOT)/libs/imgui \
	objdump -d -M intel -S $(BUILD)/$(o)/imgui_impl_vulkan.$(o) > $(BUILD)/$(s)/imgui_impl_vulkan.$(s)
#



$(BUILD)/$(o)/xgk.$(o) : $(SRC)/xgk.cpp
	$(makeBuild) && $(makeO) && $(makeS) && \
	g++ -o $(BUILD)/$(o)/xgk.$(o) \
	$(SRC)/xgk.cpp \
	$(COMPILER_ARG) \
	-I $(ROOT)/libs \
	-I $(ROOT)/reps/denis-belov \
	-I $(ROOT)/reps/denis-belov/xgk-math/src \
	objdump -d -M intel -S $(BUILD)/$(o)/xgk.$(o) > $(BUILD)/$(s)/xgk.$(s)



$(BUILD)/$(o)/gl.$(o) : $(SRC)/gl.cpp
	$(makeBuild) && $(makeO) && $(makeS) && \
	g++ -o $(BUILD)/$(o)/gl.$(o) \
	$(SRC)/gl.cpp \
	$(COMPILER_ARG) \
	-I $(ROOT)/libs \
	-I $(ROOT)/libs/imgui \
	objdump -d -M intel -S $(BUILD)/$(o)/gl.$(o) > $(BUILD)/$(s)/gl.$(s)



$(BUILD)/$(o)/vk.$(o) :\
$(SRC)/vk.cpp \
$(XGK_AUX_SRC)/api/vulkan.h \
$(BUILD)/h/vertex_shader_code_vulkan.h \
$(BUILD)/h/fragment_shader_code_vulkan.h
	$(makeBuild) && $(makeO) && $(makeS) && \
	g++ -o $(BUILD)/$(o)/vk.$(o) \
	$(SRC)/vk.cpp \
	$(COMPILER_ARG) \
	-I $(ROOT)/libs \
	-I $(ROOT)/libs/imgui \
	-I $(ROOT)/reps/denis-belov \
	objdump -d -M intel -S $(BUILD)/$(o)/vk.$(o) > $(BUILD)/$(s)/vk.$(s)



$(BUILD)/h/vertex_shader_code_vulkan.h : $(SRC)/vk-shaders/vertex_shader_code_vulkan.vert
	$(makeBuild) && $(makeSpv) && $(makeH) && \
	glslangValidator -o $(BUILD)/spv/vertex_shader_code_vulkan.$(s)pv $(SRC)/vk-shaders/vertex_shader_code_vulkan.vert -V -t && \
	node $(SRC)/spv2h $(BUILD)/spv/vertex_shader_code_vulkan.$(s)pv $(BUILD)/h/vertex_shader_code_vulkan.h vertex_shader_code_vulkan



$(BUILD)/h/fragment_shader_code_vulkan.h : $(SRC)/vk-shaders/fragment_shader_code_vulkan.frag
	$(makeBuild) && $(makeSpv) && $(makeH) && \
	glslangValidator -o $(BUILD)/spv/fragment_shader_code_vulkan.$(s)pv $(SRC)/vk-shaders/fragment_shader_code_vulkan.frag -V -t && \
	node $(SRC)/spv2h $(BUILD)/spv/fragment_shader_code_vulkan.$(s)pv $(BUILD)/h/fragment_shader_code_vulkan.h fragment_shader_code_vulkan



$(XGK_MATH_BUILD)/$(a)/xgk-math.$(a) :\
$(XGK_MATH_SRC)/data/data.h \
$(XGK_MATH_SRC)/data/const.cpp \
$(XGK_MATH_SRC)/data/vec4/vec4-32.cpp \
$(XGK_MATH_SRC)/data/vec4/vec4-128.cpp \
$(XGK_MATH_SRC)/data/vec4/vec4.cpp \
$(XGK_MATH_SRC)/data/quat/quat-32.cpp \
$(XGK_MATH_SRC)/data/quat/quat-128.cpp \
$(XGK_MATH_SRC)/data/quat/quat.cpp \
$(XGK_MATH_SRC)/data/mat4/mat4-32.cpp \
$(XGK_MATH_SRC)/data/mat4/mat4-128.cpp \
$(XGK_MATH_SRC)/data/mat4/mat4.cpp \
$(XGK_MATH_SRC)/object/object.h \
$(XGK_MATH_SRC)/object/object.cpp \
$(XGK_MATH_SRC)/util/util.h \
$(XGK_MATH_SRC)/util/util.cpp \
$(XGK_MATH_SRC)/util/util-asm-x64.$(s)
	$(_MAKE) -f $(ROOT)/reps/denis-belov/xgk-math/makefiles/$(BUILD_TYPE)/makefile
