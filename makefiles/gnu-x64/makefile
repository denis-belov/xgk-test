# build and run command
# clear && make && ~/reps/denis-belov/xgk-test/build/gnu-x64/bin/xgk
#





BUILD_TYPE=gnu-x64

ROOT=~

DIR=$(ROOT)/reps/denis-belov/xgk-test

SRC=$(DIR)/src

BUILD=$(DIR)/build/$(BUILD_TYPE)

XGK_MATH_BUILD=$(ROOT)/reps/denis-belov/xgk-math/build/$(BUILD_TYPE)

XGK_MATH_SRC=$(ROOT)/reps/denis-belov/xgk-math/src

XGK_AUX_SRC=$(ROOT)/reps/denis-belov/xgk-aux/src

_MAKE=make

#file extensions
bin=
a=a
o=o
s=s

makeBuild=echo mkdir -p $(DIR)/build && mkdir -p $(BUILD)
makeBin=mkdir -p $(BUILD)/bin
makeO=mkdir -p $(BUILD)/$(o)
makeS=mkdir -p $(BUILD)/$(s)
makeSpv=mkdir -p $(BUILD)/spv
makeH=mkdir -p $(BUILD)/h

COMPILER=g++

COMPILER_ARG=\
	-std=c++11 \
	-c \
	-m64 -msse3 -Ofast -funroll-loops \
	-fdiagnostics-color=always \
	-o # trailing space

LINKER=g++

LINKER_LIB=\
	-L $(ROOT)/libs/glfw-3.3.2/src -l glfw3 \
	-l dl \
	-l pthread \
	-l Xrandr -l Xxf86vm -l Xi -l Xinerama -l X11 -l Xcursor

LINKER_ARG=-flto -o # trailing space



# linking
$(BUILD)/bin/xgk$(bin) :\
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
	$(LINKER) \
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
		$(LINKER_LIB) \
	$(LINKER_ARG)$(BUILD)/bin/xgk$(bin) \
	&& objdump -d -M intel -S $(BUILD)/bin/xgk$(bin) > $(BUILD)/$(s)/build.$(s) && echo



$(BUILD)/$(o)/glad.$(o) : $(ROOT)/libs/glad/src/glad.c
	$(makeBuild) && $(makeO) && $(makeS) && \
	$(COMPILER) $(ROOT)/libs/glad/src/glad.c $(COMPILER_ARG)$(BUILD)/$(o)/glad.$(o) \
	-I $(ROOT)/libs/glad/include \
	&& objdump -d -M intel -S $(BUILD)/$(o)/glad.$(o) > $(BUILD)/$(s)/glad.$(s) && echo



#GUI
$(BUILD)/$(o)/imgui.$(o) : $(ROOT)/reps/ocornut/imgui/imgui.cpp
	$(makeBuild) && $(makeO) && $(makeS) && \
	$(COMPILER) $(ROOT)/reps/ocornut/imgui/imgui.cpp $(COMPILER_ARG)$(BUILD)/$(o)/imgui.$(o) \
	-I $(ROOT)/libs \
	-I $(ROOT)/reps/ocornut/imgui \
	&& objdump -d -M intel -S $(BUILD)/$(o)/imgui.$(o) > $(BUILD)/$(s)/imgui.$(s) && echo



$(BUILD)/$(o)/imgui_widgets.$(o) : $(ROOT)/reps/ocornut/imgui/imgui_widgets.cpp
	$(makeBuild) && $(makeO) && $(makeS) && \
	$(COMPILER) $(ROOT)/reps/ocornut/imgui/imgui_widgets.cpp $(COMPILER_ARG)$(BUILD)/$(o)/imgui_widgets.$(o) \
	-I $(ROOT)/libs \
	-I $(ROOT)/reps/ocornut/imgui \
	&& objdump -d -M intel -S $(BUILD)/$(o)/imgui_widgets.$(o) > $(BUILD)/$(s)/imgui_widgets.$(s) && echo



$(BUILD)/$(o)/imgui_draw.$(o) : $(ROOT)/reps/ocornut/imgui/imgui_draw.cpp
	$(makeBuild) && $(makeO) && $(makeS) && \
	$(COMPILER) $(ROOT)/reps/ocornut/imgui/imgui_draw.cpp $(COMPILER_ARG)$(BUILD)/$(o)/imgui_draw.$(o) \
	-I $(ROOT)/libs \
	-I $(ROOT)/reps/ocornut/imgui \
	&& objdump -d -M intel -S $(BUILD)/$(o)/imgui_draw.$(o) > $(BUILD)/$(s)/imgui_draw.$(s) && echo



$(BUILD)/$(o)/imgui_impl_glfw.$(o) : $(ROOT)/reps/ocornut/imgui/examples/imgui_impl_glfw.cpp
	$(makeBuild) && $(makeO) && $(makeS) && \
	$(COMPILER) $(ROOT)/reps/ocornut/imgui/examples/imgui_impl_glfw.cpp $(COMPILER_ARG)$(BUILD)/$(o)/imgui_impl_glfw.$(o) \
	-I $(ROOT)/libs/glfw-3.3.2/include \
	-I $(ROOT)/reps/ocornut/imgui \
	&& objdump -d -M intel -S $(BUILD)/$(o)/imgui_impl_glfw.$(o) > $(BUILD)/$(s)/imgui_impl_glfw.$(s) && echo



$(BUILD)/$(o)/imgui_impl_opengl3.$(o) : $(ROOT)/reps/ocornut/imgui/examples/imgui_impl_opengl3.cpp
	$(makeBuild) && $(makeO) && $(makeS) && \
	$(COMPILER) $(ROOT)/reps/ocornut/imgui/examples/imgui_impl_opengl3.cpp $(COMPILER_ARG)$(BUILD)/$(o)/imgui_impl_opengl3.$(o) \
	-I $(ROOT)/reps/ocornut/imgui \
	-I $(ROOT)/libs/glad/include \
	&& objdump -d -M intel -S $(BUILD)/$(o)/imgui_impl_opengl3.$(o) > $(BUILD)/$(s)/imgui_impl_opengl3.$(s) && echo



$(BUILD)/$(o)/imgui_impl_vulkan.$(o) : $(ROOT)/reps/ocornut/imgui/examples/imgui_impl_vulkan.cpp
	$(makeBuild) && $(makeO) && $(makeS) && \
	$(COMPILER) $(ROOT)/reps/ocornut/imgui/examples/imgui_impl_vulkan.cpp $(COMPILER_ARG)$(BUILD)/$(o)/imgui_impl_vulkan.$(o) \
	-I $(ROOT)/reps/ocornut/imgui \
	&& objdump -d -M intel -S $(BUILD)/$(o)/imgui_impl_vulkan.$(o) > $(BUILD)/$(s)/imgui_impl_vulkan.$(s) && echo
#



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



$(BUILD)/$(o)/gl.$(o) : $(SRC)/gl.cpp
	$(makeBuild) && $(makeO) && $(makeS) && \
	$(COMPILER) $(SRC)/gl.cpp $(COMPILER_ARG)$(BUILD)/$(o)/gl.$(o) \
	-I $(ROOT)/libs/glad/include \
	-I $(ROOT)/libs/glfw-3.3.2/include \
	-I $(ROOT)/reps/ocornut/imgui \
	&& objdump -d -M intel -S $(BUILD)/$(o)/gl.$(o) > $(BUILD)/$(s)/gl.$(s) && echo



$(BUILD)/$(o)/vk.$(o) :\
$(SRC)/vk.cpp \
$(XGK_AUX_SRC)/api/vulkan.h \
$(BUILD)/h/vertex_shader_code_vulkan.h \
$(BUILD)/h/fragment_shader_code_vulkan.h
	$(makeBuild) && $(makeO) && $(makeS) && \
	$(COMPILER) $(SRC)/vk.cpp $(COMPILER_ARG)$(BUILD)/$(o)/vk.$(o) \
	-I $(ROOT)/libs/glfw-3.3.2/include \
	-I $(ROOT)/reps/ocornut/imgui \
	-I $(ROOT)/reps/denis-belov \
	&& objdump -d -M intel -S $(BUILD)/$(o)/vk.$(o) > $(BUILD)/$(s)/vk.$(s) && echo



$(BUILD)/h/vertex_shader_code_vulkan.h : $(SRC)/vk-shaders/vertex_shader_code_vulkan.vert
	$(makeBuild) && $(makeSpv) && $(makeH) && \
	glslangValidator -o $(BUILD)/spv/vertex_shader_code_vulkan.spv $(SRC)/vk-shaders/vertex_shader_code_vulkan.vert -V -t && \
	node $(SRC)/spv2h $(BUILD)/spv/vertex_shader_code_vulkan.spv $(BUILD)/h/vertex_shader_code_vulkan.h vertex_shader_code_vulkan



$(BUILD)/h/fragment_shader_code_vulkan.h : $(SRC)/vk-shaders/fragment_shader_code_vulkan.frag
	$(makeBuild) && $(makeSpv) && $(makeH) && \
	glslangValidator -o $(BUILD)/spv/fragment_shader_code_vulkan.spv $(SRC)/vk-shaders/fragment_shader_code_vulkan.frag -V -t && \
	node $(SRC)/spv2h $(BUILD)/spv/fragment_shader_code_vulkan.spv $(BUILD)/h/fragment_shader_code_vulkan.h fragment_shader_code_vulkan



$(BUILD)/$(o)/xgk.$(o) : $(SRC)/xgk.cpp
	$(makeBuild) && $(makeO) && $(makeS) && \
	$(COMPILER) $(SRC)/xgk.cpp $(COMPILER_ARG)$(BUILD)/$(o)/xgk.$(o) \
	-I $(ROOT)/libs/glfw-3.3.2/include \
	-I $(ROOT)/reps/denis-belov \
	-I $(ROOT)/reps/denis-belov/xgk-math/src \
	&& objdump -d -M intel -S $(BUILD)/$(o)/xgk.$(o) > $(BUILD)/$(s)/xgk.$(s) && echo
