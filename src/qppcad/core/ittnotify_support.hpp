#ifndef ITTNOTIFY_SUPPORT_GUARD
#define ITTNOTIFY_SUPPORT_GUARD
#ifdef WITH_VTUNE_INSTRUMENTATION

#include "ittnotify.h"

struct instrumentation {
  inline static __itt_domain* d_qppcad;
  inline static __itt_string_handle* h_qppcad_frame;
  inline static __itt_string_handle* h_ws_mouse_click;
  static void init();
};

#endif
#endif
