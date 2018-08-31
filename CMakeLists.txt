cmake_minimum_required (VERSION 3.2)
project(qppcad CXX C)
add_definitions(-O3 -std=c++11 -fPIC -DFMT_HEADER_ONLY)
set(CMAKE_CXX_FLAGS "-O3 -funroll-loops -ftree-vectorize")
#set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

macro(set_option option value)
  set(${option} ${value} CACHE INTERNAL "" FORCE)
endmacro()

set_option(PYTHON_BINDINGS OFF)
add_subdirectory(deps/qpp)
add_subdirectory(deps/freetype)
add_subdirectory(deps/glfw)
include_directories(deps/glfw/include)
include_directories(deps/glfw/deps)
include_directories(src/)
include_directories(data/)
include_directories(deps/imgui)
include_directories(deps/imgui/misc/freetype)
include_directories(deps/imgui/examples)
include_directories(deps/freetype/include)
include_directories(deps/qpp/modules)
include_directories(deps/qpp/deps/pybind11/include)
include_directories(deps/qpp/deps/fmtlib)
include_directories(deps/qpp/deps/eigen3)

file(GLOB_RECURSE ide_cpp_qppcad "src/qppcad/*.cpp")
file(GLOB_RECURSE ide_hpp_qppcad "src/qppcad/*.hpp")
#file(GLOB_RECURSE ide_t1 "tests/*/*.*pp")
#file(GLOB_RECURSE ide_t2 "tests/*.*pp")
add_custom_target(cad_files SOURCES ${ide_cpp_qppcad} ${ide_hpp_qppcad})
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)
set(GL_LIBRARY GL  X11)
add_executable(qppcad
    src/qppcad/main.cpp
    src/qppcad/mesh.cpp
    src/qppcad/app.cpp
    src/qppcad/shader_program.cpp
    src/qppcad/draw_pipeline.cpp
    src/qppcad/workspace.cpp
    src/qppcad/ws_item.cpp
    src/qppcad/ws_atom_list.cpp
    src/qppcad/camera.cpp
    src/qppcad/action.cpp
    src/qppcad/gl_math.cpp
    src/qppcad/ui_manager.cpp
    src/qppcad/gizmo.cpp
    deps/imgui/misc/freetype/imgui_freetype.cpp
    deps/imgui/examples/imgui_impl_glfw.cpp
    deps/imgui/examples/imgui_impl_opengl3.cpp
    deps/glfw/deps/gl3w.c
    deps/imgui/imgui.cpp
    deps/imgui/imgui_draw.cpp
    )
target_link_libraries(qppcad qpp ${glfw_LIBRARIES} ${GL_LIBRARY} freetype glfw m)
