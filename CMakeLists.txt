cmake_minimum_required (VERSION 3.2)
project(qppcad CXX C)
add_definitions(-fPIC -DFMT_HEADER_ONLY)
set(CMAKE_CXX_FLAGS " -std=c++17 -fno-omit-frame-pointer -fvisibility=hidden")
#set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Og -fno-omit-frame-pointer -g3 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -ffast-math")

#set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
set(OpenGL_GL_PREFERENCE "GLVND")
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
find_package(Qt5Charts)

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Charts_INCLUDE_DIRS})

qt5_add_resources(QRCS data/resources.qrc)

add_executable(qppcad

    src/qppcad/main.cpp
    src/qppcad/main_window.cpp

    src/qppcad/ws_viewer_widget.cpp
    src/qppcad/object_inspector_widget.cpp
    src/qppcad/app_settings_widget.cpp
    src/qppcad/qextended_action.cpp
    src/qppcad/qspoiler_widget.cpp
    src/qppcad/qt_helpers.cpp

    src/qppcad/app_state.cpp
    src/qppcad/app_state_event_disp.cpp

    src/qppcad/shader_program.cpp
    src/qppcad/shader_generators.cpp
    src/qppcad/mesh.cpp
    src/qppcad/mesh_generators.cpp
    src/qppcad/glapi.cpp
    src/qppcad/draw_pipeline.cpp

    src/qppcad/workspace.cpp
    src/qppcad/string_hash_register.cpp
    src/qppcad/type_info.cpp
    src/qppcad/qpp_object.cpp
    src/qppcad/register_all_things.cpp

    src/qppcad/ws_item.cpp
    src/qppcad/ws_item_obj_insp_widget.cpp
    src/qppcad/ws_item_behaviour_manager.cpp
    src/qppcad/camera.cpp
    src/qppcad/gizmo.cpp
    src/qppcad/embedded_cluster_tools.cpp

    src/qppcad/ws_cube_primitive/ws_cube_primitive.cpp
    src/qppcad/ws_cube_primitive/ws_cube_primitive_obj_insp_widget.cpp

    src/qppcad/ws_arrow_primitive/ws_arrow_primitive.cpp
    src/qppcad/ws_arrow_primitive/ws_arrow_primitive_obj_insp_widget.cpp

    src/qppcad/ws_comp_chem_data/ws_comp_chem_data.cpp
    src/qppcad/ws_comp_chem_data/ws_comp_chem_data_obj_insp_widget.cpp

    src/qppcad/ws_volume_data/ws_volume_data.cpp
    src/qppcad/ws_volume_data/ws_volume_data_obj_insp_widget.cpp
    src/qppcad/volume_tools.cpp

    src/qppcad/geom_view/geom_view.cpp
    src/qppcad/geom_view/geom_view_render_bs.cpp
    src/qppcad/geom_view/geom_view_render_billboards.cpp
    src/qppcad/geom_view/geom_view_render_buffered_billboards.cpp
    src/qppcad/geom_view/geom_view_render_dlines.cpp
    src/qppcad/geom_view/geom_view_render_xlines.cpp
    src/qppcad/geom_view/geom_view_anim_subsys.cpp
    src/qppcad/geom_view/geom_view_measurement_subsys.cpp
    src/qppcad/geom_view/geom_view_lattice_planes_subsys.cpp
    src/qppcad/geom_view/geom_view_labels_subsys.cpp
    src/qppcad/geom_view/geom_view_colorizer.cpp
    src/qppcad/geom_view/geom_view_obj_insp_widget.cpp
    src/qppcad/geom_view/geom_view_io.cpp
    src/qppcad/geom_view/qtype_specific_rendering_model.cpp
    src/qppcad/geom_view/qbonding_table_model.cpp
    src/qppcad/geom_view/qmeasurements_table_model.cpp

    src/qppcad/ws_point_sym_group/ws_point_sym_group.cpp
    src/qppcad/ws_point_sym_group/ws_point_sym_group_obj_insp_widget.cpp

    src/qppcad/ws_traj_highlight/ws_traj_highlight.cpp
    src/qppcad/ws_traj_highlight/ws_traj_highlight_obj_insp_widget.cpp

    src/qppcad/tools/supercell/supercell.cpp
    src/qppcad/tools/axial_scale/axial_scale.cpp
    src/qppcad/tools/clamp_atoms_to_cell/clamp_atoms_to_cell.cpp

    src/qppcad/qbinded_inputs.cpp
    src/qppcad/qrealspinbox_delegate.cpp
    src/qppcad/python_console_output_redirectors.cpp
    src/qppcad/python_console_widget.cpp
    src/qppcad/python_manager.cpp
    src/qppcad/python_embedded_core.cpp
    src/qppcad/python_embedded_simple_query.cpp
    src/qppcad/python_embedded_pyqpp.cpp
    src/qppcad/python_embedded_embc.cpp
    src/qppcad/python_simple_query.cpp
    src/qppcad/python_embedded_workspace.cpp
    deps/qpp/modules/pyqpp/pyqpp_linalg.cpp
    deps/qpp/modules/pyqpp/pyqpp_geom.cpp
    deps/qpp/modules/pyqpp/pyqpp_xgeom.cpp
    deps/qpp/modules/pyqpp/pyqpp_cell.cpp
    deps/qpp/modules/pyqpp/pyqpp_autosymm.cpp
    deps/qpp/modules/pyqpp/pyqpp_builders.cpp
    deps/qpp/modules/data/errors.cpp
    ${QRCS})

qt5_use_modules(qppcad Core OpenGL Gui Widgets)

target_link_libraries(qppcad
    qpp
    Qt5::Widgets
    Qt5::OpenGL
    Qt5::Charts
    ${OPENGL_LIBRARIES}
    pybind11::embed)
target_compile_definitions(qppcad PUBLIC -DQPPCAD_PY_EXPORT)

