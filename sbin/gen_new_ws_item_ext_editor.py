#!/usr/bin/python3
import sys
import os

hpp_tmp = """#include <qppcad/ws_item/ws_item_extended_editor.hpp>
#include <qppcad/ws_item/BASE_CLASS_NAME/BASE_CLASS_NAME.hpp>

namespace qpp {

  namespace cad {

    class CLASS_NAME_t : public ws_item_extended_editor_t {

        Q_OBJECT

      public:

        BASE_CLASS_NAME_t *m_binded_nx{nullptr};
        QVBoxLayout *main_lt;

        CLASS_NAME_t();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;

        bool can_be_binded_to(ws_item_t *item) override;

		QString header_name_hint() override;
		
    };

  }

}
"""

cpp_tmp = """#include <qppcad/ws_item/BASE_CLASS_NAME/CLASS_NAME.hpp>

using namespace qpp;
using namespace qpp::cad;

CLASS_NAME_t::CLASS_NAME_t() {

  main_lt = new QVBoxLayout;
  main_lt->setContentsMargins(0,0,10,10);
  setLayout(main_lt);

}

void CLASS_NAME_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_extended_editor_t::bind_to_item(_binding_item);

  if (!_binding_item) {
      m_binded_nx = nullptr;
      return;
    }

  auto as_nx = _binding_item->cast_as<BASE_CLASS_NAME_t>();
  if (!as_nx) {
      m_binded_nx = nullptr;
      return;
    }

  m_binded_nx = as_nx;

}

void CLASS_NAME_t::update_from_ws_item() {

  ws_item_extended_editor_t::update_from_ws_item();

}

void CLASS_NAME_t::unbind_item() {

  ws_item_extended_editor_t::unbind_item();
  m_binded_nx = nullptr;

}

bool CLASS_NAME_t::can_be_binded_to(ws_item_t *item) {
  return true;
}

QString CLASS_NAME_t::header_name_hint() {
  return "CLASS_NAME EDITOR";
}
"""

argc = len(sys.argv)

if (argc != 3):
    print("argc == 1, exiting")
    sys.exit(0)

if (argc == 3):
	base_class_name = sys.argv[1]
	class_name = sys.argv[2]
	new_path = "../src/qppcad/ws_item/{}/".format(base_class_name)
	if not os.path.exists(new_path):
		os.makedirs(new_path)
    
	file_hpp = open("{}/{}.hpp".format(new_path, class_name), "w")
	file_cpp = open("{}/{}.cpp".format(new_path, class_name), "w")
	
    #write extended edtior
	hpp_guard = ("QPP_CAD_{}".format(class_name)).upper()
	file_hpp.write("#ifndef {}\n".format(hpp_guard))
	file_hpp.write("#define {}\n".format(hpp_guard))
	
	hpp_emp = hpp_tmp.replace("BASE_CLASS_NAME", base_class_name).replace("CLASS_NAME", class_name)
	cpp_emp = cpp_tmp.replace("BASE_CLASS_NAME", base_class_name).replace("CLASS_NAME", class_name)

	file_hpp.write(hpp_emp)
	file_cpp.write(cpp_emp)

	file_hpp.write("#endif")
