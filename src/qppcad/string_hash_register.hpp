#ifndef QPP_CAD_STRING_HASH_REGISTER_H
#define QPP_CAD_STRING_HASH_REGISTER_H

#include <string>
#include <functional>
#include <optional>
#include <map>

namespace qpp {

  namespace cad {

    //inspired by Urho3d https://github.com/urho3d
    class string_hash_register_t {
      private:
        std::map<size_t, std::string> p_data;

      public:
        string_hash_register_t();

        bool register_string(const std::string &_str, const size_t _hash);
        bool register_string(const std::string &_str);
        std::string get_pretty_string(const size_t _hash);
        std::optional<std::string> get_string(const size_t _hash);
        std::optional<size_t> get_hash(const std::string &_str);
        size_t get_or_create_hash(const std::string &_str);
        bool contains(size_t _hash);
        bool contains(const std::string &_str);

        template<typename T>
        size_t calc_hash(const T &value) {
          return std::hash<T>{}(value);
        }
    };

  }

}


#endif
