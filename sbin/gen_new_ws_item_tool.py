#!/usr/bin/python3
import sys
import os

hpp_tmp = """#ifndef QPPCAD_TOOL_#UCLASS_NAME
#define QPPCAD_TOOL_#UCLASS_NAME
#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>

namespace qpp {

  namespace cad {

    class #CLASS_NAME_tool_t : public ws_item_tool_t {
    
      public:
      
        void exec(ws_item_t *item, uint32_t _error_ctx) override;
        
    };
    
  } // namespace cad
 
} // namespace qpp

#endif
"""

cpp_tmp = """#include <qppcad/tools/#CLASS_NAME/#CLASS_NAME.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

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
