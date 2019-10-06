#ifndef QPPCAD_OBJECT_H
#define QPPCAD_OBJECT_H

#include <qppcad/core/qppcad.hpp>
#include <qppcad/core/type_info.hpp>

namespace qpp {

  namespace cad {

//BEGIN QPP_OBJECT_MACRO
  #define QPP_OBJECT(type_name, base_type_name) \
    public: \
      using cls_name = type_name; \
      using base_cls_name = base_type_name; \
      virtual size_t get_type() const override { \
        return get_type_info_static()->get_type(); \
      } \
      virtual const std::string& get_type_name() const override {  \
        return get_type_info_static()->get_type_name(); \
      } \
      virtual const qtype_info* get_type_info() const override { \
        return get_type_info_static(); \
      } \
      static size_t get_type_static() { \
        return get_type_info_static()->get_type(); \
      } \
      static const std::string& get_type_name_static() { \
        return get_type_info_static()->get_type_name(); \
      } \
      static const qtype_info* get_type_info_static() { \
      static const qtype_info type_info_static(#type_name, base_cls_name::get_type_info_static()); \
        return &type_info_static; \
      } \
//END QPP_OBJECT_MACRO

    //inspired by Urho3d https://github.com/urho3d
    class qpp_object_t {

      public:

        virtual size_t get_type() const = 0;
        virtual const std::string& get_type_name() const = 0;
        virtual const qtype_info* get_type_info() const = 0;
        static const qtype_info* get_type_info_static() { return nullptr; }

        bool is_instance_of(size_t type) const;
        bool is_instance_of(const qtype_info* _type_info) const;

        template<typename T>
        bool is_instance_of() const {
          return is_instance_of(T::get_type_info_static());
        }

        template<typename T>
        T* cast_as() {
          return is_instance_of<T>() ? static_cast<T*>(this) : nullptr;
        }

        template<typename T>
        const T* cast_as() const {
          return is_instance_of<T>() ? static_cast<const T*>(this) : nullptr;
        }

    };

    template <char... chars>
    using char_as_int_seq = std::integer_sequence<char, chars...>;

    template <typename T, T... chars>
    constexpr char_as_int_seq<chars...> operator""_t_name() { return { }; }


    template<class base, char... chars>
    class derived_qpp_object_t : public qpp_object_t {

      public:

        static const char* get_chars() {
          static constexpr char str[sizeof...(chars) + 1] = { chars..., '\0' };
          return str;
        }

        virtual size_t get_type() const override {
          return get_type_info_static()->get_type();
        }

        virtual const std::string& get_type_name() const override {
          return get_type_info_static()->get_type_name();
        }

        virtual const qtype_info* get_type_info() const override {
          return get_type_info_static();
        }

        static size_t get_type_static() {
          return get_type_info_static()->get_type();
        }
        static const std::string& get_type_name_static() {
          return get_type_info_static()->get_type_name();
        }

        static const qtype_info* get_type_info_static() {
          static const qtype_info type_info_static(get_chars(), base::get_type_info_static());
          return &type_info_static;
        }

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
