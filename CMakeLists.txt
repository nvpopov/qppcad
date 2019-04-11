cmake_minimum_required (VERSION 3.2)
project(qppcad CXX C)

#set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_BUILD_TYPE "Release")

add_definitions(-DFMT_HEADER_ONLY)
if(NOT MSVC)
  add_definitions(-fPIC)
  set(CMAKE_CXX_FLAGS " -std=c++17 -fno-omit-frame-pointer -fvisibility=hidden -pthread")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Og -fno-omit-frame-pointer -g3 -ggdb")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -ffast-math")
elseif(MSVC)
   add_definitions(-D_ENABLE_EXTENDED_ALIGNED_STORAGE -DHAVE_SNPRINTF)
   add_compile_options("/std:c++latest")
endif()

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

find_package(Doxygen)

add_subdirectory(deps/qpp)

include_directories(src/)
include_directories(data/)
include_directories(deps/qpp/modules)
include_directories(deps/qpp/deps/pybind11/include)
include_directories(deps/qpp/deps/fmtlib/include)
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

find_package(Git)
if(GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/src/qppcad/"
    OUTPUT_VARIABLE _build_version
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  message( STATUS "GIT hash: ${_build_version}")
else()
  message(STATUS "GIT not found")
endif()

string(TIMESTAMP _time_stamp)

configure_file(${CMAKE_SOURCE_DIR}/src/qppcad/gitversion_ex.h.in
               ${CMAKE_SOURCE_DIR}/src/qppcad/gitversion_ex.h @ONLY)

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5OpenGL CONFIG REQUIRED)
find_package(Qt5Svg)
find_package(Qt5Charts)

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Charts_INCLUDE_DIRS})
include_directories(deps/qpp/tests)

qt5_add_resources(QRCS data/resources.qrc)

add_executable(qppcad

    src/qppcad/main.cpp
    src/qppcad/gitversion.cpp
    src/qppcad/ui/main_window.cpp

    src/qppcad/ui/ws_viewer_widget.cpp
    src/qppcad/ui/object_inspector_widget.cpp
    src/qppcad/ui/app_settings_widget.cpp
    src/qppcad/ui/qextended_action.cpp
    src/qppcad/ui/qspoiler_widget.cpp
    src/qppcad/ui/qt_helpers.cpp
    src/qppcad/ui/qbinded_inputs.cpp
    src/qppcad/ui/qrealspinbox_delegate.cpp
    src/qppcad/ui/add_new_ws_item_widget.cpp
    src/qppcad/ui/qrich_error_message_box.cpp
    src/qppcad/ui/color_map.cpp
    src/qppcad/ui/size_guide.cpp
    src/qppcad/ui/qembed_window.cpp

    src/qppcad/app_state.cpp
    src/qppcad/app_state_event_disp.cpp

    src/qppcad/render/shader_program.cpp
    src/qppcad/render/shader_generators.cpp
    src/qppcad/render/mesh.cpp
    src/qppcad/render/mesh_generators.cpp
    src/qppcad/render/glapi.cpp
    src/qppcad/render/draw_pipeline.cpp
    src/qppcad/render/camera.cpp
    src/qppcad/render/volume_tools.cpp

    src/qppcad/workspace.cpp
    src/qppcad/string_hash_register.cpp
    src/qppcad/type_info.cpp
    src/qppcad/qpp_object.cpp
    src/qppcad/register_all_things.cpp

    src/qppcad/ws_item/ws_item.cpp
    src/qppcad/ws_item/ws_item_obj_insp_widget.cpp
    src/qppcad/ws_item/ws_item_behaviour_manager.cpp
    src/qppcad/ws_item/ws_item_extended_editor.cpp
    src/qppcad/ws_item/ws_item_extended_editor_compositor.cpp

    src/qppcad/gizmo.cpp
    src/qppcad/embedded_cluster_tools.cpp

    src/qppcad/ws_item/node_book/node_book.cpp
    src/qppcad/ws_item/node_book/node_book_obj_insp_widget.cpp
    src/qppcad/ws_item/node_book/node_book_extended_editor.cpp
    src/qppcad/ws_item/node_book/node_book_graphics_view.cpp
    src/qppcad/ws_item/node_book/node_book_graphics_scene.cpp

    src/qppcad/ws_item/cube_primitive/cube_primitive.cpp
    src/qppcad/ws_item/cube_primitive/cube_primitive_obj_insp_widget.cpp

    src/qppcad/ws_item/arrow_primitive/arrow_primitive.cpp
    src/qppcad/ws_item/arrow_primitive/arrow_primitive_obj_insp_widget.cpp

    src/qppcad/ws_item/ccd_view/ccd_view.cpp
    src/qppcad/ws_item/ccd_view/ccd_view_obj_insp_widget.cpp
    src/qppcad/ws_item/ccd_view/ccd_vib_analyzer_extended_editor.cpp
    src/qppcad/ws_item/ccd_view/ccd_traj_analyzer_extended_editor.cpp
    src/qppcad/ws_item/ccd_view/normal_modes_table_model.cpp

    src/qppcad/ws_item/volume_view/volume_view.cpp
    src/qppcad/ws_item/volume_view/volume_view_obj_insp_widget.cpp

    src/qppcad/ws_item/pgf_producer/pgf_producer.cpp
    src/qppcad/ws_item/pgf_producer/pgf_producer_obj_insp_widget.cpp

    src/qppcad/ws_item/geom_view/geom_view.cpp
    src/qppcad/ws_item/geom_view/geom_view_render_bs.cpp
    src/qppcad/ws_item/geom_view/geom_view_render_billboards.cpp
    src/qppcad/ws_item/geom_view/geom_view_render_buffered_billboards.cpp
    src/qppcad/ws_item/geom_view/geom_view_render_dlines.cpp
    src/qppcad/ws_item/geom_view/geom_view_render_xlines.cpp
    src/qppcad/ws_item/geom_view/geom_view_anim_subsys.cpp
    src/qppcad/ws_item/geom_view/geom_view_measurement_subsys.cpp
    src/qppcad/ws_item/geom_view/geom_view_lattice_planes_subsys.cpp
    src/qppcad/ws_item/geom_view/geom_view_labels_subsys.cpp
    src/qppcad/ws_item/geom_view/geom_view_colorizer.cpp
    src/qppcad/ws_item/geom_view/geom_view_obj_insp_widget.cpp
    src/qppcad/ws_item/geom_view/geom_view_extended_editor.cpp
    src/qppcad/ws_item/geom_view/geom_view_io.cpp
    src/qppcad/ws_item/geom_view/qtype_specific_rendering_model.cpp
    src/qppcad/ws_item/geom_view/qbonding_table_model.cpp
    src/qppcad/ws_item/geom_view/qmeasurements_table_model.cpp
    src/qppcad/ws_item/geom_view/xgeom_table_model.cpp
    src/qppcad/ws_item/geom_view/geom_view_type_summary_popup.cpp

    src/qppcad/ws_item/arrow_array/arrow_array.cpp
    src/qppcad/ws_item/arrow_array/arrow_array_obj_insp_widget.cpp

    src/qppcad/ws_item/psg_view/psg_view.cpp
    src/qppcad/ws_item/psg_view/psg_view_obj_insp_widget.cpp

    src/qppcad/ws_item/traj_hl/traj_hl.cpp
    src/qppcad/ws_item/traj_hl/traj_hl_obj_insp_widget.cpp

    src/qppcad/tools/supercell/supercell.cpp
    src/qppcad/tools/axial_scale/axial_scale.cpp
    src/qppcad/tools/clamp_atoms_to_cell/clamp_atoms_to_cell.cpp
    src/qppcad/tools/colorize_by_xfield/colorize_by_xfield.cpp
    src/qppcad/tools/colorize_by_dist/colorize_by_dist.cpp
    src/qppcad/tools/structure_similarity/structure_similarity.cpp

    src/qppcad/flow/sflow.cpp

    src/qppcad/python/python_console_output_redirectors.cpp
    src/qppcad/python/python_console_widget.cpp
    src/qppcad/python/python_manager.cpp
    src/qppcad/python/python_embedded_core.cpp
    src/qppcad/python/python_embedded_simple_query.cpp
    src/qppcad/python/python_embedded_pyqpp.cpp
    src/qppcad/python/python_embedded_embc.cpp
    src/qppcad/python/python_simple_query.cpp
    src/qppcad/python/python_embedded_workspace.cpp

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
    Qt5::Svg
    ${OPENGL_LIBRARIES}
    pybind11::embed
    ${PYTHON_LIBRARIES}
    pthread
    dl
    util
    m)

target_compile_definitions(qppcad PUBLIC -DQPPCAD_PY_EXPORT)

add_executable(sflow_test src/qppcad/flow/sflow.cpp src/qppcad/flow/sflow_test.cpp)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_SOURCE_DIR}/docs/doxygen.conf"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src
        COMMENT "Building user's documentation into doxyDoc build dir..."
    )
endif()
