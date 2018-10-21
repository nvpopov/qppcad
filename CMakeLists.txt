cmake_minimum_required (VERSION 3.2)
project(qppcad CXX C)
add_definitions(-O3 -fPIC -DFMT_HEADER_ONLY)
set(CMAKE_CXX_FLAGS "-O3 -std=c++17 -fno-omit-frame-pointer -fvisibility=hidden")
set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE "Release")

#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
#set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

macro(set_option option value)
  set(${option} ${value} CACHE INTERNAL "" FORCE)
endmacro()

set_option(PYTHON_BINDINGS OFF)
set_option(WITH_HarfBuzz OFF)

add_subdirectory(deps/qpp)
add_subdirectory(deps/freetype)
add_subdirectory(deps/glfw)

include_directories(deps/glfw/include)
include_directories(deps/glfw/deps)
include_directories(src/)
include_directories(data/)
include_directories(deps/imgui)
include_directories(deps/imgui/misc/freetype)
include_directories(deps/imgui/misc/stl)
include_directories(deps/imgui/examples)
include_directories(deps/freetype/include)
include_directories(deps/qpp/modules)
include_directories(deps/qpp/deps/pybind11/include)
include_directories(deps/qpp/deps/fmtlib)
include_directories(deps/qpp/deps/eigen3)
include_directories(deps/fifo_map)
include_directories(deps/args)
include_directories(deps)
include_directories(deps/nativefiledialog/src/)
include_directories(deps/nativefiledialog/src/include)
include_directories(${PYTHON_INCLUDE_DIRS})
if (UNIX)
    find_package(PkgConfig REQUIRED)
    PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
    add_library(nfd deps/nativefiledialog/src/nfd_common.c deps/nativefiledialog/src/nfd_gtk.c)
    include_directories(${GTK3_INCLUDE_DIRS})
endif(UNIX)

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
    src/qppcad/ws_atoms_list.cpp
    src/qppcad/ws_atoms_list_lattice_planes_subsys.cpp
    src/qppcad/ws_atoms_list_measurement_subsys.cpp
    src/qppcad/camera.cpp
    src/qppcad/action.cpp
    src/qppcad/gl_math.cpp
    src/qppcad/ui_manager.cpp
    src/qppcad/gizmo.cpp
    src/qppcad/file_dialog.cpp
    src/qppcad/imgui_addons.cpp
    src/qppcad/console.cpp
    src/qppcad/super_cell_widget.cpp
    src/qppcad/simple_query.cpp
    src/qppcad/uniq_id.cpp
    deps/imgui/misc/freetype/imgui_freetype.cpp
    deps/imgui/misc/stl/imgui_stl.cpp
    deps/imgui/examples/imgui_impl_glfw.cpp
    deps/imgui/examples/imgui_impl_opengl3.cpp
    deps/glfw/deps/gl3w.c
    deps/imgui/imgui.cpp
    deps/imgui/imgui_widgets.cpp
    deps/imgui/imgui_draw.cpp)

add_executable(bin2compc deps/imgui/misc/fonts/binary_to_compressed_c.cpp)

if(PYTHON_BINDINGS)
add_definitions(-DPYTHON_CONSOLE)
target_link_libraries(qppcad qpp ${glfw_LIBRARIES} ${GL_LIBRARY}
    freetype glfw nfd ${GTK3_LIBRARIES} pybind11::embed m )
else()
    target_link_libraries(qppcad qpp ${glfw_LIBRARIES} ${GL_LIBRARY}
    freetype glfw nfd ${GTK3_LIBRARIES} m )
endif(PYTHON_BINDINGS)
