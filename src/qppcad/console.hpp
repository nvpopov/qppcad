#ifndef QPP_PYTHON_CONSOLE
#define QPP_PYTHON_CONSOLE

#include <qppcad/qppcad.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stl.h>

#ifdef PYTHON_CONSOLE
#include <pybind11/embed.h>
namespace py = pybind11;

#endif

namespace qpp {

  namespace cad {

    class app_state_t;

    class console_widget_t {
      private:
#ifdef PYTHON_CONSOLE
        py::scoped_interpreter py_interp;
        py::object scope;
#endif
      public:
        uint32_t m_id;
        uint8_t m_line_height;
        uint8_t m_total_com_lines;
        bool     m_active;
        string m_output;
        string m_command;
        console_widget_t(app_state_t *init_app_state);
        void toggle_visible();
        void process_command(string &command);
        void render();
        static int InputTextCallback(ImGuiInputTextCallbackData* data);
    };

  }

}

#endif
