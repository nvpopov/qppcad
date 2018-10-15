#ifndef QPP_CAD_SIMPLE_QUERY
#define QPP_CAD_SIMPLE_QUERY

#include <io/strfun.hpp>
#include <qppcad/qppcad.hpp>

namespace qpp {

  namespace cad {

    class simple_query_command_t {
      public:
        virtual bool execute (std::vector<std::string_view> &commands_list,
                              std::string &output) = 0;
        virtual ~simple_query_command_t ();
    };


    class sq_command_select_item : public simple_query_command_t {
      public:
        bool execute (std::vector<std::string_view> &commands_list, std::string &output) override ;
    };

    class sq_command_echo : public simple_query_command_t {
      public:
        bool execute (std::vector<std::string_view> &commands_list, std::string &output) override ;
    };

    class sq_command_select_content : public simple_query_command_t {
      public:
        bool execute (std::vector<std::string_view> &commands_list, std::string &output) override ;
    };

    class sq_command_change_edit_type : public simple_query_command_t {
      private:
        ws_edit_type cur_edit_type{ws_edit_type::EDIT_WS_ITEM};
      public:
         bool execute (std::vector<std::string_view> &commands_list, std::string &output) override ;
         sq_command_change_edit_type(const ws_edit_type edit_type);
    };

    class simple_query_manager_t {

      private:

        std::map<std::string, std::shared_ptr<simple_query_command_t> > m_commands;

      public:

        void _add_command (std::string c_name,
                          std::shared_ptr<simple_query_command_t> c_inst) {
          m_commands.emplace(c_name, c_inst);
        }

        template<typename T, typename... CArgs>
        void add_command(std::string c_name, CArgs ... cargs){

          auto c1 = std::make_shared<T>(cargs...);
          _add_command(c_name, c1);

        }

        void execute (const std::string &command, std::string &output) {

          if (command == "") return;

          std::vector<std::string_view> sv = split_sv(command, " ");
          auto it = m_commands.find(std::string(sv[0]));

          if (it != m_commands.end()) {
              it->second->execute(sv, output);
            } else {
              output = fmt::format("Command \"{}\" not found!\n", sv[0]);
            }

        }

        simple_query_manager_t () {
          add_command<sq_command_echo>("echo");
          add_command<sq_command_select_item>("seli");
          add_command<sq_command_change_edit_type>("itm", ws_edit_type::EDIT_WS_ITEM);
          add_command<sq_command_change_edit_type>("cnt", ws_edit_type::EDIT_WS_ITEM_CONTENT);
          add_command<sq_command_select_content>("sel");
        }

    };
  }
}

#endif
