#ifndef QPP_CAD_DELEGATES
#define QPP_CAD_DELEGATES
#include <qppcad/qppcad.hpp>

namespace qpp {

  ///
  /// \brief The delegate_slot_interface class
  ///
  template<typename... Args>
  class delegate_slot_interface {
    public:
      virtual void call (Args...) {}
      virtual ~delegate_slot_interface () {}
  };

  ///
  /// \brief The delegate_slot_function class
  ///
  template<typename M, typename... Args>
  class delegate_slot_function : public delegate_slot_interface<Args...>{
    public:
      delegate_slot_function (M _method){ method = _method; }
      void call (Args... args){ (method)(args...);}
    private:
      M method;
  };

  ///
  /// \brief The delegate_slot_class_member class
  ///
  template<typename T, typename M, typename... Args>
  class delegate_slot_class_member : public delegate_slot_interface<Args...>{
    public:
      delegate_slot_class_member (T* _obj_inst, M _obj_method){
        obj_inst = _obj_inst; obj_method = _obj_method;
      }
      void call (Args... args){ (obj_inst->*obj_method)(args...);}
    private:
      T* obj_inst;
      M obj_method;
  };

  ///
  /// \brief The delegate class
  ///
  template<typename... Args>
  class delegate{
    private:
      std::vector<delegate_slot_interface<Args...>* > slots;
    public:
      delegate(){}

      ~delegate(){
        clear_all();
      }

      ///
      /// \brief clear_all
      ///
      void clear_all(){
        for (delegate_slot_interface<Args...> *item : slots)
          if (item) delete item;
      }

      ///
      /// \brief connect
      /// \param _obj_inst
      /// \param _obj_method
      ///
      template<typename T, typename M>
      void connect(T *_obj_inst, M _obj_method){
        delegate_slot_class_member<T, M, Args...> *_slot_cm =
            new delegate_slot_class_member<T, M, Args...>(_obj_inst, _obj_method);
        slots.push_back(_slot_cm);
      }

      ///
      /// \brief connect
      /// \param _func_inst
      ///
      template<typename M>
      void connect(M _func_inst){
        delegate_slot_function<M, Args...> *_slot_f =
            new delegate_slot_function<M, Args...>(_func_inst);
        slots.push_back(_slot_f);
      }

      ///
      /// \brief disconnect
      /// \param _obj_inst
      /// \param _obj_method
      ///
      template<typename T, typename M>
      void disconnect(T *_obj_inst, M _obj_method){
        uint8_t idx = 0;
        delegate_slot_class_member<T, M, Args...> *_slot_cm = nullptr;
        for (uint8_t i = 0; i < slots.size(); i++){
            delegate_slot_class_member<T, M, Args...> *temp =
                dynamic_cast<delegate_slot_class_member<T, M, Args...>*>(slots[i]);
            if (temp->obj_inst == _obj_inst && temp->obj_method == _obj_method){
                _slot_cm = temp;
                idx = i;
                break;
              }
          }

        if (_slot_cm) {
            delete _slot_cm;
            slots.erase(idx);
          }
      }

      ///
      /// \brief disconnect
      /// \param _func_inst
      ///
      template<typename M>
      void disconnect(M _func_inst){
        uint8_t idx = 0;
        delegate_slot_function<M, Args...> *_slot_cm = nullptr;
        for (uint8_t i = 0; i < slots.size(); i++){
            delegate_slot_function<M, Args...> *temp =
                dynamic_cast<delegate_slot_function<M, Args...>*>(slots[i]);
            if (temp->method == _func_inst){
                _slot_cm = temp;
                idx = i;
                break;
              }
          }

        if (_slot_cm) {
            delete _slot_cm;
            slots.erase(idx);
          }
      }

      ///
      /// \brief operator ()
      /// \param args
      ///
      void operator()(Args... args){
        call(args...);
      }

      ///
      /// \brief call
      /// \param args
      ///
      void call(Args... args){
        for (uint8_t i = 0; i < slots.size(); i++)
          slots[i]->call(args...);
      }

  };
}

#endif
