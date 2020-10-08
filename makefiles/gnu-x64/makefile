# build and run command
# clear && make && ~/reps/denis-belov/c-test/xgk/build/gnu-x64/bin/xgk
#


# -ldl

DIR=~/reps/denis-belov/c-test/xgk

SRC=$(DIR)/src

BUILD=$(DIR)/build/gnu-x64

XGK_MATH_BUILD=~/reps/denis-belov/xgk-math/build/gnu-x64

XGK_MATH_SRC=~/reps/denis-belov/xgk-math/src

XGK_AUX_SRC=~/reps/denis-belov/xgk-aux/src

makeBuildFolder=mkdir -p $(DIR)/build && mkdir -p $(BUILD)

# # colors
# RED='\033[0;31m'

# DIR=~/reps/denis-belov/c-test/$(FOLDER)

# SRC=$(DIR)/src

# BUILD=$(DIR)/build-win-x64

# XGK_BUILD=E:/reps/denis-belov/xgk/build-win-x64

# XGK_SRC=E:/reps/denis-belov/xgk/src

# NAME=test



# -L/usr/lib/x86_64-linux-gnu \
# --entry main \
# -melf_x86_64 \
# -flto \
# -ldl \
# -lc \



# linking
$(BUILD)/bin/xgk : \
$(BUILD)/o/glad.o \
$(BUILD)/o/imgui.o \
$(BUILD)/o/imgui_widgets.o \
$(BUILD)/o/imgui_draw.o \
$(BUILD)/o/imgui_impl_glfw.o \
$(BUILD)/o/imgui_impl_opengl3.o \
$(BUILD)/o/imgui_impl_vulkan.o \
$(XGK_MATH_BUILD)/a/xgk-math.a \
$(BUILD)/o/gl.o \
$(BUILD)/o/vk.o \
$(BUILD)/o/xgk.o
	echo $(makeBuildFolder) && mkdir -p $(BUILD)/bin && \
	g++ -o $(BUILD)/bin/xgk \
	$(BUILD)/o/glad.o \
	$(BUILD)/o/imgui.o \
	$(BUILD)/o/imgui_widgets.o \
	$(BUILD)/o/imgui_draw.o \
	$(BUILD)/o/imgui_impl_glfw.o \
	$(BUILD)/o/imgui_impl_opengl3.o \
	$(BUILD)/o/imgui_impl_vulkan.o \
	$(XGK_MATH_BUILD)/a/xgk-math.a \
	$(BUILD)/o/gl.o \
	$(BUILD)/o/vk.o \
	$(BUILD)/o/xgk.o \
	-L ~/libs/glfw-3.3.2/src -l glfw3 \
	-l dl \
	-l pthread \
	-l Xrandr -l Xxf86vm -l Xi -l Xinerama -l X11 -l Xcursor \
	-flto \
	-fdiagnostics-color=always \
	&& $(makeBuildFolder) && mkdir -p $(BUILD)/s && \
	objdump -d -M intel -S $(BUILD)/bin/xgk > $(BUILD)/s/build.s \
	&& echo



$(BUILD)/o/xgk.o : $(SRC)/xgk.cpp
	$(makeBuildFolder) && mkdir -p $(BUILD)/o && \
	g++ -o $(BUILD)/o/xgk.o \
	$(SRC)/xgk.cpp \
	-std=c++11 \
	-c \
	-m64 -msse3 -Ofast -funroll-loops \
	-fdiagnostics-color=always \
	-I ~/libs \
	-I ~/reps/denis-belov \
	-I ~/reps/denis-belov/xgk-math/src \
	&& $(makeBuildFolder) && mkdir -p $(BUILD)/s && \
	objdump -d -M intel -S $(BUILD)/o/xgk.o > $(BUILD)/s/xgk.s \
	&& echo



$(BUILD)/o/gl.o : $(SRC)/gl.cpp
	$(makeBuildFolder) && mkdir -p $(BUILD)/o && \
	g++ -o $(BUILD)/o/gl.o \
	$(SRC)/gl.cpp \
	-std=c++11 \
	-c \
	-m64 -msse3 -Ofast -funroll-loops \
	-fdiagnostics-color=always \
	-I ~/libs \
	-I ~/libs/imgui \
	&& $(makeBuildFolder) && mkdir -p $(BUILD)/s && \
	objdump -d -M intel -S $(BUILD)/o/gl.o > $(BUILD)/s/gl.s \
	&& echo



$(BUILD)/o/vk.o : \
$(SRC)/vk.cpp \
$(XGK_AUX_SRC)/api/vulkan.h \
$(BUILD)/h/vertex_shader_code_vulkan.h \
$(BUILD)/h/fragment_shader_code_vulkan.h
	$(makeBuildFolder) && mkdir -p $(BUILD)/o && \
	g++ -o $(BUILD)/o/vk.o \
	$(SRC)/vk.cpp \
	-std=c++11 \
	-c \
	-m64 -msse3 -Ofast -funroll-loops \
	-fdiagnostics-color=always \
	-I ~/libs \
	-I ~/libs/imgui \
	-I ~/reps/denis-belov \
	&& $(makeBuildFolder) && mkdir -p $(BUILD)/s && \
	objdump -d -M intel -S $(BUILD)/o/vk.o > $(BUILD)/s/vk.s \
	&& echo



$(BUILD)/h/vertex_shader_code_vulkan.h : $(SRC)/vk-shaders/vertex_shader_code_vulkan.vert
	$(makeBuildFolder) && mkdir -p $(BUILD)/spv && \
	glslangValidator -o $(BUILD)/spv/vertex_shader_code_vulkan.spv $(SRC)/vk-shaders/vertex_shader_code_vulkan.vert -V -t && \
	mkdir -p $(BUILD)/h && \
	node $(SRC)/spv2h $(BUILD)/spv/vertex_shader_code_vulkan.spv $(BUILD)/h/vertex_shader_code_vulkan.h vertex_shader_code_vulkan \
	&& echo



$(BUILD)/h/fragment_shader_code_vulkan.h : $(SRC)/vk-shaders/fragment_shader_code_vulkan.frag
	$(makeBuildFolder) && mkdir -p $(BUILD)/spv && \
	glslangValidator -o $(BUILD)/spv/fragment_shader_code_vulkan.spv $(SRC)/vk-shaders/fragment_shader_code_vulkan.frag -V -t && \
	mkdir -p $(BUILD)/h && \
	node $(SRC)/spv2h $(BUILD)/spv/fragment_shader_code_vulkan.spv $(BUILD)/h/fragment_shader_code_vulkan.h fragment_shader_code_vulkan \
	&& echo



$(BUILD)/o/glad.o : ~/libs/glad/src/glad.c
	$(makeBuildFolder) && mkdir -p $(BUILD)/o && \
	gcc -o $(BUILD)/o/glad.o \
	~/libs/glad/src/glad.c \
	-c \
	-I ~/libs/glad/include \
	&& $(makeBuildFolder) && mkdir -p $(BUILD)/s && \
	objdump -d -M intel -S $(BUILD)/o/glad.o > $(BUILD)/s/glad.s \
	&& echo



#GUI
$(BUILD)/o/imgui.o : ~/libs/imgui/imgui.cpp
	$(makeBuildFolder) && mkdir -p $(BUILD)/o && \
	g++ -o $(BUILD)/o/imgui.o \
	~/libs/imgui/imgui.cpp \
	-c \
	-I ~/libs \
	-I ~/libs/imgui \
	&& $(makeBuildFolder) && mkdir -p $(BUILD)/s && \
	objdump -d -M intel -S $(BUILD)/o/imgui.o > $(BUILD)/s/imgui.s \
	&& echo



$(BUILD)/o/imgui_widgets.o : ~/libs/imgui/imgui_widgets.cpp
	$(makeBuildFolder) && mkdir -p $(BUILD)/o && \
	g++ -o $(BUILD)/o/imgui_widgets.o \
	~/libs/imgui/imgui_widgets.cpp \
	-c \
	-I ~/libs \
	-I ~/libs/imgui \
	&& $(makeBuildFolder) && mkdir -p $(BUILD)/s && \
	objdump -d -M intel -S $(BUILD)/o/imgui_widgets.o > $(BUILD)/s/imgui_widgets.s \
	&& echo



$(BUILD)/o/imgui_draw.o : ~/libs/imgui/imgui_draw.cpp
	$(makeBuildFolder) && mkdir -p $(BUILD)/o && \
	g++ -o $(BUILD)/o/imgui_draw.o \
	~/libs/imgui/imgui_draw.cpp \
	-c \
	-I ~/libs \
	-I ~/libs/imgui \
	&& $(makeBuildFolder) && mkdir -p $(BUILD)/s && \
	objdump -d -M intel -S $(BUILD)/o/imgui_draw.o > $(BUILD)/s/imgui_draw.s \
	&& echo



$(BUILD)/o/imgui_impl_glfw.o : ~/libs/imgui/examples/imgui_impl_glfw.cpp
	$(makeBuildFolder) && mkdir -p $(BUILD)/o && \
	g++ -o $(BUILD)/o/imgui_impl_glfw.o \
	~/libs/imgui/examples/imgui_impl_glfw.cpp \
	-c \
	-I ~/libs \
	-I ~/libs/imgui \
	&& $(makeBuildFolder) && mkdir -p $(BUILD)/s && \
	objdump -d -M intel -S $(BUILD)/o/imgui_impl_glfw.o > $(BUILD)/s/imgui_impl_glfw.s \
	&& echo



$(BUILD)/o/imgui_impl_opengl3.o : ~/libs/imgui/examples/imgui_impl_opengl3.cpp
	$(makeBuildFolder) && mkdir -p $(BUILD)/o && \
	g++ -o $(BUILD)/o/imgui_impl_opengl3.o \
	~/libs/imgui/examples/imgui_impl_opengl3.cpp \
	-c \
	-I ~/libs/imgui \
	-I ~/libs/glad/include \
	&& $(makeBuildFolder) && mkdir -p $(BUILD)/s && \
	objdump -d -M intel -S $(BUILD)/o/imgui_impl_opengl3.o > $(BUILD)/s/imgui_impl_opengl3.s \
	&& echo



$(BUILD)/o/imgui_impl_vulkan.o : ~/libs/imgui/examples/imgui_impl_vulkan.cpp
	$(makeBuildFolder) && mkdir -p $(BUILD)/o && \
	g++ -o $(BUILD)/o/imgui_impl_vulkan.o \
	~/libs/imgui/examples/imgui_impl_vulkan.cpp \
	-c \
	-I ~/libs/imgui \
	&& $(makeBuildFolder) && mkdir -p $(BUILD)/s && \
	objdump -d -M intel -S $(BUILD)/o/imgui_impl_vulkan.o > $(BUILD)/s/imgui_impl_vulkan.s \
	&& echo
#



$(XGK_MATH_BUILD)/a/xgk-math.a : \
$(XGK_MATH_SRC)/util/util-asm-x64.s \
$(XGK_MATH_SRC)/util/util.h \
$(XGK_MATH_SRC)/util/util.cpp \
$(XGK_MATH_SRC)/data/data.h \
$(XGK_MATH_SRC)/data/const.h \
$(XGK_MATH_SRC)/data/vec4/vec4.h \
$(XGK_MATH_SRC)/data/vec4/vec4-32.h \
$(XGK_MATH_SRC)/data/vec4/vec4-128.h \
$(XGK_MATH_SRC)/data/vec4/vec4.cpp \
$(XGK_MATH_SRC)/data/quat/quat.h \
$(XGK_MATH_SRC)/data/quat/quat-32.h \
$(XGK_MATH_SRC)/data/quat/quat-128.h \
$(XGK_MATH_SRC)/data/quat/quat.cpp \
$(XGK_MATH_SRC)/data/mat4/mat4.h \
$(XGK_MATH_SRC)/data/mat4/mat4-32.h \
$(XGK_MATH_SRC)/data/mat4/mat4-128.h \
$(XGK_MATH_SRC)/data/mat4/mat4.cpp \
$(XGK_MATH_SRC)/object/object.h \
$(XGK_MATH_SRC)/object/object.cpp
	make -f ~/reps/denis-belov/xgk-math/makefiles/gnu-x64/makefile



# $(BUILD)/exe/$(NAME).exe : \
# $(BUILD)/o/glad.o \
# $(BUILD)/o/$(NAME).o \
# $(XGK_MATH_BUILD)/a/xgk-math.lib \
# $(XGK_AUX_BUILD)/a/xgk-aux.lib
#   link \
#   $(BUILD)/o/glad.obj \
#   $(BUILD)/obj/$(NAME).obj \
#   xgk.lib \
#   gdi32.lib \
#   user32.lib \
#   shell32.lib \
#   glfw3.lib \
#   vulkan-1.lib \
#   /OUT:$(BUILD)/exe/$(NAME).exe \
#   /SUBSYSTEM:CONSOLE \
#   /NODEFAULTLIB:LIBCMT \
#   /LIBPATH:E:/clibs/glfw-3.3.bin.WIN64/glfw-3.3.bin.WIN64/lib-vc2017 \
#   /LIBPATH:E:/clibs/VulkanSDK/1.1.121.0/Lib \
#   /LIBPATH:$(XGK_BUILD)/lib



# $(XGK_BUILD)/a/xgk.a : \
# $(XGK_SRC)/util/win-x64.asm \
# $(XGK_SRC)/util/util.h \
# $(XGK_SRC)/util/util.cpp \
# $(XGK_SRC)/data/data.h \
# $(XGK_SRC)/data/vec4/vec4.cpp \
# $(XGK_SRC)/data/vec4/vec4-32.cpp \
# $(XGK_SRC)/data/vec4/vec4-128.cpp \
# $(XGK_SRC)/data/quat/quat.cpp \
# $(XGK_SRC)/data/quat/quat-32.cpp \
# $(XGK_SRC)/data/quat/quat-128.cpp \
# $(XGK_SRC)/data/mat4/mat4.cpp \
# $(XGK_SRC)/data/mat4/mat4-32.cpp \
# $(XGK_SRC)/data/mat4/mat4-128.cpp \
# $(XGK_SRC)/object/object.h \
# $(XGK_SRC)/object/object.cpp
#   node E:/reps/denis-belov/c-build/src/make E:/reps/denis-belov/xgk/makefile-win-x64



# $(BUILD)/obj/glad.obj : E:/clibs/glad/src/glad.c
#   cl \
#   E:/clibs/glad/src/glad.c \
#   /c \
#   /EHsc \
#   /MD \
#   /MP999 \
#   /IE:/clibs/glad/include \
#   /Fo$(BUILD)/obj/glad.obj \
#   /FA \
#   /Fa$(BUILD)/masm/glad.asm



# $(BUILD)/obj/$(NAME).obj : \
# $(SRC)/$(NAME).cpp \
# $(XGK_SRC)/api/vulkan.h \
# $(XGK_SRC)/util/util.h \
# $(XGK_SRC)/data/data.h \
# $(XGK_SRC)/object/object.h \
# $(XGK_SRC)/time/time.h \
# $(XGK_SRC)/orbit/orbit.h \
# $(XGK_SRC)/api/vulkan.h \
# $(BUILD)/temp/shaders.h
#   cl \
#   $(SRC)/$(NAME).cpp \
#   /c \
#   /EHsc \
#   /MD \
#   /MP999 \
#   /O2 \
#   /IE:/clibs/glfw-3.3.bin.WIN64/glfw-3.3.bin.WIN64/include/GLFW \
#   /IE:/clibs/glad/include \
#   /IE:/clibs/VulkanSDK/1.1.121.0/Include \
#   /I$(XGK_SRC) \
#   /I$(BUILD) \
#   /Fo$(BUILD)/obj/$(NAME).obj \
#   /FA \
#   /Fa$(BUILD)/masm/$(NAME).asm



# $(BUILD)/temp/shaders.h : $(SRC)/shaders.h
#   node \
#   E:/reps/denis-belov/c-build/src/glsl \
#   $(SRC)/shaders.h \
#   $(BUILD)/temp/shaders.h
