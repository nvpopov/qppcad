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


    class sq_command_echo : public simple_query_command_t {

      public:

        bool execute (std::vector<std::string_view> &commands_list,
                      std::string &output) override {

          bool status = true;
          if (commands_list.size() != 2) {
              output += "commands_list.size() != 2\n";
              return false;
            } else {
              output += fmt::format("echo says : {}\n", commands_list[1]);
            }

          return status;

        }


    };

    class simple_query_manager_t {

      private:

        std::map<std::string, std::shared_ptr<simple_query_command_t> > m_commands;

      public:

        void add_command (std::string c_name,
                          std::shared_ptr<simple_query_command_t> c_inst) {
          m_commands.emplace(c_name, c_inst);
        }

        template<typename T>
        void add_command_by_type(std::string c_name){

          auto c1 = std::make_shared<T>();
          add_command(c_name, c1);

        }

        void execute (const std::string &command, std::string &output) {

          auto sv = split_sv(command, " ");
          auto it = m_commands.find(sv[0].data());
          if (it != m_commands.end()) {
              it->second->execute(sv, output);
            } else {
              output = fmt::format("Command \"{}\" not found!\n", sv[0].data());
            }

        }

        simple_query_manager_t () {
          add_command_by_type<sq_command_echo>("echo");
        }

    };
  }
}

#endif
