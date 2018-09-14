#ifndef QPPCAD_KEYBOARD_EVENT
#define QPPCAD_KEYBOARD_EVENT

#include <qppcad/qppcad.hpp>

namespace qpp {

  class keyboard_command_t;
  class keyboard_manager_t;
  class keyboard_command_chain_t;

  class keyboard_command_t {
    public:
      keyboard_command_t();
  };

  class keyboard_command_chain_t {
     public:
      keyboard_command_chain_t();
  };

  class keyboard_manager_t {
    public:
      keyboard_manager_t();

  };

}

#endif
