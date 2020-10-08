DIR=E:/reps/denis-belov/c-test/$(FOLDER)
SRC=$(DIR)/src
BUILD=$(DIR)/build-win-x64
XGK_BUILD=E:/reps/denis-belov/xgk/build-win-x64
XGK_SRC=E:/reps/denis-belov/xgk/src
NAME=test



$(BUILD)/exe/$(NAME).exe : \
$(BUILD)/obj/glad.obj \
$(BUILD)/obj/$(NAME).obj \
$(XGK_BUILD)/lib/xgk.lib
  link \
  $(BUILD)/obj/glad.obj \
  $(BUILD)/obj/$(NAME).obj \
  xgk.lib \
  gdi32.lib \
  user32.lib \
  shell32.lib \
  glfw3.lib \
  vulkan-1.lib \
  /OUT:$(BUILD)/exe/$(NAME).exe \
  /SUBSYSTEM:CONSOLE \
  /NODEFAULTLIB:LIBCMT \
  /LIBPATH:E:/clibs/glfw-3.3.bin.WIN64/glfw-3.3.bin.WIN64/lib-vc2017 \
  /LIBPATH:E:/clibs/VulkanSDK/1.1.121.0/Lib \
  /LIBPATH:$(XGK_BUILD)/lib \
  && dumpbin /disasm E:/reps/denis-belov/c-test/xgk/build-win-x64/exe/test.exe /out:E:/reps/denis-belov/c-test/xgk/build-win-x64/masm/disasm.asm



$(XGK_BUILD)/lib/xgk.lib : \
$(XGK_SRC)/util/win-x64.asm \
$(XGK_SRC)/util/util.h \
$(XGK_SRC)/util/util.cpp \
$(XGK_SRC)/data/data.h \
$(XGK_SRC)/data/vec4/vec4.cpp \
$(XGK_SRC)/data/vec4/vec4-32.cpp \
$(XGK_SRC)/data/vec4/vec4-128.cpp \
$(XGK_SRC)/data/quat/quat.cpp \
$(XGK_SRC)/data/quat/quat-32.cpp \
$(XGK_SRC)/data/quat/quat-128.cpp \
$(XGK_SRC)/data/mat4/mat4.cpp \
$(XGK_SRC)/data/mat4/mat4-32.cpp \
$(XGK_SRC)/data/mat4/mat4-128.cpp \
$(XGK_SRC)/object/object.h \
$(XGK_SRC)/object/object.cpp \
$(XGK_SRC)/time/time.h \
$(XGK_SRC)/time/time.cpp \
$(XGK_SRC)/orbit/orbit.h \
$(XGK_SRC)/orbit/orbit.cpp \
$(XGK_SRC)/api/vulkan.h
  node E:/reps/denis-belov/c-build/src/make E:/reps/denis-belov/xgk/makefile-win-x64



$(BUILD)/obj/glad.obj : E:/clibs/glad/src/glad.c
  cl \
  E:/clibs/glad/src/glad.c \
  /c \
  /EHsc \
  /MD \
  /MP999 \
  /IE:/clibs/glad/include \
  /Fo$(BUILD)/obj/glad.obj \
  /FA \
  /Fa$(BUILD)/masm/glad.asm



$(BUILD)/obj/$(NAME).obj : \
$(SRC)/$(NAME).cpp \
$(XGK_SRC)/api/vulkan.h \
$(XGK_SRC)/util/util.h \
$(XGK_SRC)/data/data.h \
$(XGK_SRC)/object/object.h \
$(XGK_SRC)/time/time.h \
$(XGK_SRC)/orbit/orbit.h \
$(XGK_SRC)/api/vulkan.h \
$(BUILD)/temp/shaders.h
  cl \
  $(SRC)/$(NAME).cpp \
  /c \
  /EHsc \
  /MD \
  /MP999 \
  /O2 \
  /IE:/clibs/glfw-3.3.bin.WIN64/glfw-3.3.bin.WIN64/include/GLFW \
  /IE:/clibs/glad/include \
  /IE:/clibs/VulkanSDK/1.1.121.0/Include \
  /I$(XGK_SRC) \
  /I$(BUILD) \
  /Fo$(BUILD)/obj/$(NAME).obj \
  /FA \
  /Fa$(BUILD)/masm/$(NAME).asm



$(BUILD)/temp/shaders.h : $(SRC)/shaders.h
  node \
  E:/reps/denis-belov/c-build/src/glsl \
  $(SRC)/shaders.h \
  $(BUILD)/temp/shaders.h
