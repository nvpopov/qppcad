cmake_minimum_required (VERSION 3.2)
project(qppcad CXX C)
add_definitions(-fPIC -DFMT_HEADER_ONLY)
set(CMAKE_CXX_FLAGS " -std=c++17 -fno-omit-frame-pointer -fvisibility=hidden")
#set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -g3 -rdynamic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

#set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

macro(set_option option value)
  set(${option} ${value} CACHE INTERNAL "" FORCE)
endmacro()

#to prevent building qpp python bindings
set_option(PYTHON_BINDINGS OFF)
set_option(BUILD_TESTS ON)

add_subdirectory(deps/qpp)

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
find_package(Qt5OpenGL CONFIG REQUIRED)

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
qt5_add_resources(QRCS data/resources.qrc)
add_executable(qppcad
    src/qppcad/main.cpp
    src/qppcad/main_window.cpp
    src/qppcad/ws_viewer_widget.cpp
    src/qppcad/super_cell_widget.cpp
    src/qppcad/axial_scale_widget.cpp
    src/qppcad/object_inspector_widget.cpp
    src/qppcad/app_state.cpp
    src/qppcad/app_state_event_disp.cpp
    src/qppcad/shader_program.cpp
    src/qppcad/shader_generators.cpp
    src/qppcad/mesh.cpp
    src/qppcad/mesh_generators.cpp
    src/qppcad/glapi.cpp
    src/qppcad/draw_pipeline.cpp
    src/qppcad/workspace.cpp
    src/qppcad/ws_item.cpp
    src/qppcad/camera.cpp
    src/qppcad/gizmo.cpp
    src/qppcad/ws_atoms_list.cpp
    src/qppcad/ws_comp_chem_data.cpp
    src/qppcad/ws_atoms_list_render_bs.cpp
    src/qppcad/ws_atoms_list_render_billboards.cpp
    src/qppcad/ws_atoms_list_render_buffered_billboards.cpp
    src/qppcad/ws_atoms_list_render_dlines.cpp
    src/qppcad/ws_atoms_list_render_xlines.cpp
    src/qppcad/ws_atoms_list_anim_subsys.cpp
    src/qppcad/ws_atoms_list_measurement_subsys.cpp
    src/qppcad/ws_atoms_list_lattice_planes_subsys.cpp
    src/qppcad/ws_atoms_list_labels_subsys.cpp
    src/qppcad/ws_item_obj_insp_widget.cpp
    src/qppcad/ws_atoms_list_obj_insp_widget.cpp
    src/qppcad/ws_comp_chem_data_obj_insp_widget.cpp
    src/qppcad/qbinded_inputs.cpp
    src/qppcad/qbonding_table_model.cpp
    src/qppcad/qrealspinbox_delegate.cpp
    src/qppcad/python_console_output_redirectors.cpp
    src/qppcad/python_console_widget.cpp
    src/qppcad/python_manager.cpp
    src/qppcad/python_embedded_core.cpp
    ${QRCS})
qt5_use_modules(qppcad Core OpenGL Gui Widgets)
target_link_libraries(qppcad qpp Qt5::Widgets Qt5::OpenGL ${OPENGL_LIBRARIES} pybind11::embed)

