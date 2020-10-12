# build and run command
# cls && cd /d C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build && vcvarsall.bat amd64 && cd /d C:/reps/denis-belov/xgk-test/makefiles/msvs-x64 && nmake && cd /d C:/reps/denis-belov/xgk-test/build/msvs-x64/bin && xgk.exe
# or run in ConEmu64 - amd64 (vc developer command prompt x64)
# cls && cd /d C:/reps/denis-belov/xgk-test/makefiles/msvs-x64 && nmake && cd /d C:/reps/denis-belov/xgk-test/build/msvs-x64/bin && xgk.exe
#



BUILD_TYPE=msvs-x64

ROOT=C:

DIR=$(ROOT)/reps/denis-belov/xgk-test

SRC=$(DIR)/src

BUILD=$(DIR)/build/$(BUILD_TYPE)

XGK_MATH_BUILD=$(ROOT)/reps/denis-belov/xgk-math/build/$(BUILD_TYPE)

XGK_MATH_SRC=$(ROOT)/reps/denis-belov/xgk-math/src

XGK_AUX_SRC=$(ROOT)/reps/denis-belov/xgk-aux/src

_MAKE=nmake /F

#file extensions
bin=.exe
a=lib
o=obj
s=asm

makeBuild=(IF NOT EXIST $(DIR)/build cd /d $(DIR) && mkdir build) && (IF NOT EXIST $(BUILD) cd /d $(DIR)/build && mkdir $(BUILD_TYPE))
makeBin=(IF NOT EXIST $(BUILD)/bin cd /d $(BUILD) && mkdir bin)
makeO=(IF NOT EXIST $(BUILD)/obj cd /d $(BUILD) && mkdir obj)
makeS=(IF NOT EXIST $(BUILD)/asm cd /d $(BUILD) && mkdir asm)
makeSpv=(IF NOT EXIST $(BUILD)/spv cd /d $(BUILD) && mkdir spv)
makeH=(IF NOT EXIST $(BUILD)/h cd /d $(BUILD) && mkdir h)

COMPILER=cl

COMPILER_ARG=\
	/c /EHsc /MP999 /std:c++17 \
	/O2 \
	/I$(SRC) \
	/Fo

LINKER=link

LINKER_LIB=ucrt.lib gdi32.lib user32.lib shell32.lib glfw3.lib \

LINKER_ARG=\
	/LIBPATH:$(ROOT)/libs/glfw-3.3.2.bin.WIN64/glfw-3.3.2.bin.WIN64/lib-vc2019 \
	/SUBSYSTEM:CONSOLE \
	/NODEFAULTLIB:LIBUCRT \
	/NODEFAULTLIB:MSVCRT \
	/OUT:



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
	&& dumpbin /disasm $(BUILD)/bin/xgk$(bin) /out:$(BUILD)/$(s)/build.$(s)



$(BUILD)/$(o)/glad.$(o) : $(ROOT)/libs/glad/src/glad.c
	$(makeBuild) && $(makeO) && $(makeS) && \
	$(COMPILER) $(ROOT)/libs/glad/src/glad.c $(COMPILER_ARG)$(BUILD)/$(o)/glad.$(o) \
	/I$(ROOT)/libs/glad/include \
	/FA /Fa$(BUILD)/$(s)/glad.$(s)



#GUI
$(BUILD)/$(o)/imgui.$(o) : $(ROOT)/reps/ocornut/imgui/imgui.cpp
	$(makeBuild) && $(makeO) && $(makeS) && \
	$(COMPILER) $(ROOT)/reps/ocornut/imgui/imgui.cpp $(COMPILER_ARG)$(BUILD)/$(o)/imgui.$(o) \
	/I$(ROOT)/libs \
	/I$(ROOT)/reps/ocornut/imgui \
	/FA /Fa$(BUILD)/$(s)/imgui.$(s)



$(BUILD)/$(o)/imgui_widgets.$(o) : $(ROOT)/reps/ocornut/imgui/imgui_widgets.cpp
	$(makeBuild) && $(makeO) && $(makeS) && \
	$(COMPILER) $(ROOT)/reps/ocornut/imgui/imgui_widgets.cpp $(COMPILER_ARG)$(BUILD)/$(o)/imgui_widgets.$(o) \
	/I$(ROOT)/libs \
	/I$(ROOT)/reps/ocornut/imgui \
	/FA /Fa$(BUILD)/$(s)/imgui_widgets.$(s)



$(BUILD)/$(o)/imgui_draw.$(o) : $(ROOT)/reps/ocornut/imgui/imgui_draw.cpp
	$(makeBuild) && $(makeO) && $(makeS) && \
	$(COMPILER) $(ROOT)/reps/ocornut/imgui/imgui_draw.cpp $(COMPILER_ARG)$(BUILD)/$(o)/imgui_draw.$(o) \
	/I$(ROOT)/libs \
	/I$(ROOT)/reps/ocornut/imgui \
	/FA /Fa$(BUILD)/$(s)/imgui_draw.$(s)



$(BUILD)/$(o)/imgui_impl_glfw.$(o) : $(ROOT)/reps/ocornut/imgui/examples/imgui_impl_glfw.cpp
	$(makeBuild) && $(makeO) && $(makeS) && \
	$(COMPILER) $(ROOT)/reps/ocornut/imgui/examples/imgui_impl_glfw.cpp $(COMPILER_ARG)$(BUILD)/$(o)/imgui_impl_glfw.$(o) \
	/I$(ROOT)/libs/glfw-3.3.2.bin.WIN64/glfw-3.3.2.bin.WIN64/include \
	/I$(ROOT)/reps/ocornut/imgui \
	/FA /Fa$(BUILD)/$(s)/imgui_impl_glfw.$(s)



$(BUILD)/$(o)/imgui_impl_opengl3.$(o) : $(ROOT)/reps/ocornut/imgui/examples/imgui_impl_opengl3.cpp
	$(makeBuild) && $(makeO) && $(makeS) && \
	$(COMPILER) $(ROOT)/reps/ocornut/imgui/examples/imgui_impl_opengl3.cpp $(COMPILER_ARG)$(BUILD)/$(o)/imgui_impl_opengl3.$(o) \
	/I$(ROOT)/reps/ocornut/imgui \
	/I$(ROOT)/libs/glad/include \
	/FA /Fa$(BUILD)/$(s)/imgui_impl_opengl3.$(s)



$(BUILD)/$(o)/imgui_impl_vulkan.$(o) : $(ROOT)/reps/ocornut/imgui/examples/imgui_impl_vulkan.cpp
	$(makeBuild) && $(makeO) && $(makeS) && \
	$(COMPILER) $(ROOT)/reps/ocornut/imgui/examples/imgui_impl_vulkan.cpp $(COMPILER_ARG)$(BUILD)/$(o)/imgui_impl_vulkan.$(o) \
	/I$(ROOT)/libs/VulkanSDK/1.2.154.0/include \
	/I$(ROOT)/reps/ocornut/imgui \
	/FA /Fa$(BUILD)/$(s)/imgui_impl_vulkan.$(s)
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
	$(_MAKE) $(ROOT)/reps/denis-belov/xgk-math/makefiles/$(BUILD_TYPE)/makefile



$(BUILD)/$(o)/gl.$(o) : $(SRC)/gl.cpp
	$(makeBuild) && $(makeO) && $(makeS) && \
	$(COMPILER) $(SRC)/gl.cpp $(COMPILER_ARG)$(BUILD)/$(o)/gl.$(o) \
	/I$(ROOT)/libs/glad/include \
	/I$(ROOT)/libs/glfw-3.3.2.bin.WIN64/glfw-3.3.2.bin.WIN64/include \
	/I$(ROOT)/reps/ocornut/imgui \
	/FA /Fa$(BUILD)/$(s)/gl.$(s)



$(BUILD)/$(o)/vk.$(o) :\
$(SRC)/vk.cpp \
$(XGK_AUX_SRC)/api/vulkan.h \
$(BUILD)/h/vertex_shader_code_vulkan.h \
$(BUILD)/h/fragment_shader_code_vulkan.h
	$(makeBuild) && $(makeO) && $(makeS) && \
	$(COMPILER) $(SRC)/vk.cpp $(COMPILER_ARG)$(BUILD)/$(o)/vk.$(o) \
	/I$(ROOT)/libs/VulkanSDK/1.2.154.0/include \
	/I$(ROOT)/libs/glfw-3.3.2.bin.WIN64/glfw-3.3.2.bin.WIN64/include \
	/I$(ROOT)/reps/ocornut/imgui \
	/I$(ROOT)/reps/denis-belov \
	/FA /Fa$(BUILD)/$(s)/vk.$(s)



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
	/I$(ROOT)/libs/glfw-3.3.2.bin.WIN64/glfw-3.3.2.bin.WIN64/include \
	/I$(ROOT)/reps/denis-belov \
	/I$(ROOT)/reps/denis-belov/xgk-math/src \
	/FA /Fa$(BUILD)/$(s)/xgk.$(s)
