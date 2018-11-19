cmake_minimum_required (VERSION 3.2)
project(qppcad CXX C)
add_definitions(-fPIC -DFMT_HEADER_ONLY)
set(CMAKE_CXX_FLAGS " -std=c++17 -fno-omit-frame-pointer -fvisibility=hidden")
set(CMAKE_BUILD_TYPE "Release")
#set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -g3 -rdynamic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

#set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

macro(set_option option value)
  set(${option} ${value} CACHE INTERNAL "" FORCE)
endmacro()

set_option(PYTHON_BINDINGS OFF)
set_option(BUILD_TESTS ON)
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
include_directories(${PYTHON_INCLUDE_DIRS})

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

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

if(WIN32)
  set(CMAKE_WIN32_EXECUTABLE ON)
endif()

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
qt5_add_resources(QRCS data/resources.qrc)
add_executable(qppcad
    src/qppcad/main.cpp
    src/qppcad/main_window.cpp
    src/qppcad/ws_viewer_widget.cpp
    ${QRCS})
qt5_use_modules(qppcad Core Gui Widgets)
target_link_libraries(qppcad Qt5::Widgets)

