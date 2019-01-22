#!/usr/bin/python3
import sys
import os

hpp_tmp = """
namespace qpp {

  namespace cad {

    class #CLASS_NAME_t : public ws_item_t {

       QPP_OBJECT(#CLASS_NAME_t, ws_item_t)

      public:
        #CLASS_NAME_t();

        void vote_for_view_vectors(vector3<float> &out_look_pos,
                                   vector3<float> &out_look_at) override ;
        void render() override;
        bool mouse_click(ray_t<float> *click_ray) override;

        std::string compose_item_name() override;
        void update(float delta_time) override;
        float get_bb_prescaller() override;

        uint32_t get_amount_of_selected_content() override;
        size_t get_content_count() override;

        void save_to_json(json &data) override;
        void load_from_json(json &data) override;

    };

  }

}
"""

cpp_tmp = """
#include <qppcad/#CLASS_NAME/#CLASS_NAME.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

#CLASS_NAME_t::#CLASS_NAME_t() {
  set_default_flags(ws_item_flags_default);
}

void #CLASS_NAME_t::vote_for_view_vectors(vector3<float> &out_look_pos,
                                          vector3<float> &out_look_at) {
  //do nothing
}

void #CLASS_NAME_t::render() {
 //do nothing
}

bool #CLASS_NAME_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}

std::string #CLASS_NAME_t::compose_item_name() {
  return "#CLASS_NAME";
}

void #CLASS_NAME_t::update(float delta_time) {
  //do nothing
}

float #CLASS_NAME_t::get_bb_prescaller() {
  return 1.0f;
}

uint32_t #CLASS_NAME_t::get_amount_of_selected_content() {
  return 0;
}

size_t #CLASS_NAME_t::get_content_count() {
  return 0;
}

void #CLASS_NAME_t::save_to_json(json &data) {
  ws_item_t::save_to_json(data);
}

void #CLASS_NAME_t::load_from_json(json &data) {
  ws_item_t::load_from_json(data);
}

"""

argc = len(sys.argv)

if (argc == 1):
    print("argc == 1, exiting")
    sys.exit(0)

if (argc == 2):
    class_name = sys.argv[1]
    new_path = "../src/qppcad/{}".format(class_name)
    os.mkdir(new_path)

    file_hpp = open("{}/{}.hpp".format(new_path, class_name), "w")
    file_cpp = open("{}/{}.cpp".format(new_path, class_name), "w")

    hpp_guard = ("QPP_CAD_{}".format(class_name)).upper()
    file_hpp.write("#ifndef {}\n".format(hpp_guard))
    file_hpp.write("#define {}\n".format(hpp_guard))

    hpp_emp = hpp_tmp.replace("#CLASS_NAME", class_name)
    cpp_emp = cpp_tmp.replace("#CLASS_NAME", class_name)

    file_hpp.write(hpp_emp)
    file_cpp.write(cpp_emp)

    file_hpp.write("#endif")