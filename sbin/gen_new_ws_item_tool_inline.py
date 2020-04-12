#!/usr/bin/python3
import sys
import os

hpp_tmp = """#ifndef QPPCAD_TOOL_#UCLASS_NAME
#define QPPCAD_TOOL_#UCLASS_NAME
#include <qppcad/core/qppcad.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>
#include <qppcad/ui/qspoiler_widget.hpp>
#include <qppcad/ui/ws_item_inline_tool_widget.hpp>
#include <qppcad/core/iupdatable.hpp>

#include <QWidget>
#include <QApplication>

namespace qpp {

  namespace cad {

    class #CLASS_NAME_widget_t : public ws_item_inline_tool_widget_t,
                                 public iupdatable_externally_t {
        Q_OBJECT

      private:

      public:

	#CLASS_NAME_widget_t(QWidget *parent = nullptr);
        bool restore_cam_on_cancel() override;
        void on_apply() override;
        void on_cancel() override;
        void bind_item(ws_item_t *item) override;
        void updated_externally(uint32_t update_reason) override;

    };

    class #CLASS_NAME_tool_t : public ws_item_tool_t {
    
      public:
      
        void exec(ws_item_t *item, uint32_t _error_ctx) override;
        ws_item_inline_tool_widget_t *construct_inline_tool() override;

    };
    
  } // namespace qpp::cad
 
} // namespace qpp

#endif
"""

cpp_tmp = """#include <qppcad/tools/#CLASS_NAME/#CLASS_NAME.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

#CLASS_NAME_widget_t::#CLASS_NAME_widget_t (QWidget *parent)
  : ws_item_inline_tool_widget_t(parent) {

}

bool #CLASS_NAME_widget_t::restore_cam_on_cancel() {
  return true;
}

void #CLASS_NAME_widget_t::on_apply() {

}

void #CLASS_NAME_widget_t::on_cancel() {

}

void #CLASS_NAME_widget_t::bind_item(ws_item_t *item) {

  ws_item_inline_tool_widget_t::bind_item(item);

}

void #CLASS_NAME_widget_t::updated_externally(uint32_t update_reason) {

}

ws_item_inline_tool_widget_t *#CLASS_NAME_tool_t::construct_inline_tool() {

  //TODO: test widget
  return new #CLASS_NAME_widget_t();

}

void #CLASS_NAME_tool_t::exec(ws_item_t *item, uint32_t _error_ctx) {

}
"""

argc = len(sys.argv)

if (argc == 1):
    print("argc == 1, exiting")
    sys.exit(0)

if (argc == 2):
    class_name = sys.argv[1]
    new_path = "../src/qppcad/tools/{}".format(class_name)
    os.mkdir(new_path)

    new_hpp_path = "{}/{}.hpp".format(new_path, class_name)
    new_cpp_path = "{}/{}.cpp".format(new_path, class_name)

    hpp_file = open(new_hpp_path, "w")
    hpp_emp = hpp_tmp.replace("#UCLASS_NAME", class_name.upper())
    hpp_emp = hpp_emp.replace("#CLASS_NAME", class_name)
    hpp_file.write(hpp_emp)

    cpp_file = open(new_cpp_path, "w")
    cpp_emp = cpp_tmp.replace("#CLASS_NAME", class_name)
    cpp_file.write(cpp_emp)
