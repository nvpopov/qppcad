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

    enum console_type_t {
      simple_query,
      python
    };

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
        int m_total_com_lines;
        int m_total_output_lines{5};
        std::vector<std::string> m_sq_history;
        int m_history_pos;
        float m_console_alpha{0.95f};

        console_type_t m_console_type{console_type_t::simple_query};

        bool     m_active;
        std::vector<std::string> m_output;
        std::string m_command;

        console_widget_t(app_state_t *init_app_state);
        void toggle_visible();
        void process_command(std::string &command);
        void render();
        static int InputTextCallback(ImGuiInputTextCallbackData* data);
        static int simple_query_input_callback(ImGuiInputTextCallbackData* data);
    };

  }

}

#endif
