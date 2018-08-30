#ifndef QPP_CAD_H
#define QPP_CAD_H

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <io/strfun.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/gl_math.hpp>
#include <chrono>
#include <ctime>

namespace qpp{
  class c_app{

  private:
    static app_state_t* app_state;
    static GLFWwindow* curWindow;

    static void error_callback(int error, const char* description);

    static void key_callback(GLFWwindow* window,
                             int key,
                             int scancode,
                             int action,
                             int mods);

    static void resize_window_callback(GLFWwindow* window,
                                       int _width,
                                       int _height);

    static void mouse_callback( GLFWwindow *window,
                                 double x,
                                 double y );

    static void mouse_scroll_callback(GLFWwindow* window,
                                      double xoffset,
                                      double yoffset);

    static void mouse_button_callback( GLFWwindow * window,
                                     int button,
                                     int action,
                                     int mods );
    static void begin_render();
    static void render();
    static void end_render();


  public:
    static void log(const std::string logText);
    static void run();
    static app_state_t & get_state(){
      if (c_app::app_state == nullptr)
        c_app::app_state = new qpp::app_state_t();
      return *app_state;
    }
  };
}
#endif
