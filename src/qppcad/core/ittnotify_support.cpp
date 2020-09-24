#ifdef WITH_VTUNE_INSTRUMENTATION

#include <qppcad/core/ittnotify_support.hpp>

void instrumentation::init() {

  instrumentation::d_qppcad = __itt_domain_create("main_qppcad");
  instrumentation::d_qppcad->flags = 1;

  instrumentation::h_qppcad_frame = __itt_string_handle_create("frame_render");
  instrumentation::h_ws_mouse_click = __itt_string_handle_create("ws_mouse_click");

}

#endif
