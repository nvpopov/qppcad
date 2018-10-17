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
      simple_query = 0,
      python = 1
    };

    class app_state_t;

    class console_widget_t {
      private:
#ifdef PYTHON_CONSOLE
        py::scoped_interpreter py_interp;
        py::object scope;
#endif
      public:
        std::array<std::string, 2> m_output;
        std::string m_command;
        std::vector<std::string> m_sq_history;

        float m_console_alpha{0.95f};
        float m_console_height{10.0f};
        uint32_t m_id;
        uint8_t m_line_height{28};

        int m_total_com_lines{1};
        int m_total_output_lines{5};
        int m_history_pos{-1};

        bool m_last_frame_inactive{true};
        bool m_show_output{true};
        bool m_active{false};

        console_type_t m_console_type{console_type_t::simple_query};

        console_widget_t(app_state_t *astate);
        void toggle_visible();
        void process_command(std::string &command);
        void render();
        static int InputTextCallback(ImGuiInputTextCallbackData* data);
        static int simple_query_input_callback(ImGuiInputTextCallbackData* data);
    };

  }

}

#endif
