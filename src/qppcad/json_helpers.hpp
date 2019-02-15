#ifndef QPPCAD_JSON_HELPERS
#define QPPCAD_JSON_HELPERS

#include <qppcad/json_adapter.hpp>

namespace qpp {

  namespace cad {

    class json_helper {

      public:

        static bool exists(const std::string &secname, json &data) {
          return data.find(secname) != data.end();
        }

        template<typename T>
        static void save_var(const std::string &varname, T &var, json &data) {
          data[varname] = var;
        }

        template<typename T>
        static void save_vec3(const std::string &varname, vector3<T> &var, json &data) {
          json var_vec = json::array({var[0], var[1], var[2]});
          data[varname] = var_vec;
        }

        template<typename T>
        static void load_var(const std::string &varname, T &var, json &data) {
          if (data.find(varname) != data.end()) var = data[varname].get<T>();
        }

        template<typename T>
        static void load_vec3(const std::string &varname, vector3<T> &var, json &data) {
          if (data.find(varname) != data.end())
            for(auto i = 0; i < 3; i++) var[i] = data[varname][i].get<T>();
        }

    };

  }

}

#endif
