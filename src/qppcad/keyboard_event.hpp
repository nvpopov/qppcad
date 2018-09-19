#ifndef QPPCAD_KEYBOARD_EVENT
#define QPPCAD_KEYBOARD_EVENT
#include <qppcad/qppcad.hpp>
#include <qppcad/delegates.hpp>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

namespace qpp {

  class keyboard_command_t {
    public:
      vector<uint16_t> keys;
      vector<uint16_t> key_state_current;
      vector<uint16_t> key_state_last;
      delegate<> kc_dgt;

      void update(GLFWwindow *window){

//        bool all_keys{true};
        uint8_t total_press_to_release{0};
        uint8_t total_press{0};
        uint8_t total_release{0};
        uint8_t total_repeat{0};

        for (uint16_t i = 0; i < keys.size(); i++){
            key_state_last[i] = key_state_current[i];
            key_state_current[i] = glfwGetKey(window, keys[i]);
            if (key_state_current[i] == GLFW_RELEASE) total_release++;
            if (key_state_current[i] == GLFW_PRESS) total_press++;
            if (key_state_current[i] == GLFW_REPEAT) total_repeat++;
            bool bloc = (key_state_current[i] == GLFW_RELEASE) && (key_state_last[i] == GLFW_PRESS);
            if (bloc) total_press_to_release++;
//            all_keys = all_keys && bloc;
          }

//        if (total_press_to_release > 0 || total_press > 0 || total_release > 0 || total_repeat > 0)
//          std::cout << fmt::format("tpr = {}, tp = {}, trl = {}, trp = {}\n",
//                                   total_press_to_release, total_press,
//                                   total_release, total_repeat) << std::flush;
        const auto tot_keys = keys.size();
        if (tot_keys - 1 == total_press && total_press_to_release == 1) {
            kc_dgt.call();
            //std::cout << "command called\n";
          }
      }

      keyboard_command_t(initializer_list<uint16_t> keys_list){
        for (auto &key : keys_list) keys.push_back(key);
        key_state_current.resize(keys.size());
        key_state_last.resize(keys.size());
      }

  };

  class keyboard_manager_t {
    public:

      map<string, keyboard_command_t> commands;

      void update(GLFWwindow *window){
        for (auto &command : commands)
          command.second.update(window);
      }

      template<typename T, typename M>
      void connect(const string com_name, T *_obj_inst, M _obj_method){
        auto it = commands.find(com_name);
        if (it != commands.end()){
            it->second.kc_dgt.connect(_obj_inst, _obj_method);
          }
      }

      template<typename FUNC>
      void connect(const string com_name, FUNC _func_inst){
        auto it = commands.find(com_name);
        if (it != commands.end()){
            it->second.kc_dgt.connect(_func_inst);
          }
      }

      keyboard_manager_t(){
        commands.emplace("python_console_toggle", keyboard_command_t({GLFW_KEY_GRAVE_ACCENT}));
        commands.emplace("edit_mode_toggle", keyboard_command_t({GLFW_KEY_TAB}));
        commands.emplace("switch_to_ws_0", keyboard_command_t({GLFW_KEY_1, GLFW_KEY_LEFT_CONTROL}));
        commands.emplace("switch_to_ws_1", keyboard_command_t({GLFW_KEY_2, GLFW_KEY_LEFT_CONTROL}));
        commands.emplace("switch_to_ws_2", keyboard_command_t({GLFW_KEY_3, GLFW_KEY_LEFT_CONTROL}));
        commands.emplace("switch_to_ws_3", keyboard_command_t({GLFW_KEY_4, GLFW_KEY_LEFT_CONTROL}));
        commands.emplace("switch_to_ws_4", keyboard_command_t({GLFW_KEY_5, GLFW_KEY_LEFT_CONTROL}));
        commands.emplace("switch_to_ws_5", keyboard_command_t({GLFW_KEY_6, GLFW_KEY_LEFT_CONTROL}));
        commands.emplace("switch_to_ws_6", keyboard_command_t({GLFW_KEY_7, GLFW_KEY_LEFT_CONTROL}));
        commands.emplace("switch_to_ws_7", keyboard_command_t({GLFW_KEY_8, GLFW_KEY_LEFT_CONTROL}));
        commands.emplace("switch_to_ws_8", keyboard_command_t({GLFW_KEY_9, GLFW_KEY_LEFT_CONTROL}));
//        commands.emplace("console_nl", keyboard_command_t({GLFW_KEY_ENTER, GLFW_KEY_RIGHT_SHIFT}));
//        commands.emplace("console_apply", keyboard_command_t({GLFW_KEY_ENTER}));

      }
  };

}

#endif
