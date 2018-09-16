#include <qppcad/app.hpp>

void
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam ){
  if (type == GL_DEBUG_TYPE_ERROR){
      fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
               ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
               type, severity, message );
      exit(0);
    }
}

void qpp::c_app::error_callback(int error, const char* description){
  std::string s1(description);
  qpp::c_app::log(s1);
}

void qpp::c_app::key_callback(GLFWwindow* window,
                              int key, int scancode, int action, int mods){

  app_state_t* astate = &(c_app::get_state());

  qpp::c_app::log(fmt::format("Key pressed  {}, sc = {}, act = {}", key, scancode, action));

  if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS){
      astate->ui_manager->console_widget->m_active = !(astate->ui_manager->console_widget->m_active);
      qpp::c_app::log("wft?");
    }

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);


}

void qpp::c_app::run(){

  glfwSetErrorCallback(qpp::c_app::error_callback);

  if (!glfwInit()){
      qpp::c_app::log("Failed to initialize GLFW3");
      exit(EXIT_FAILURE);
    }

  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 8);


  qpp::c_app::curWindow = glfwCreateWindow(800,
                                           600,
                                           "qpp::cad", nullptr, nullptr);

  if (!qpp::c_app::curWindow){
      glfwTerminate();
      qpp::c_app::log("Failed to initialize window");
      exit(EXIT_FAILURE);
    }

  glfwMakeContextCurrent(qpp::c_app::curWindow);
  glfwSwapInterval(1);
  if (gl3wInit()) {
      qpp::c_app::log("Failed  to initialize OpenGL");
    }

  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  //io.Fonts->AddFontDefault();
  /*ImWchar ranges[] = { 0xf000, 0xf3ff, 0 };
  ImFontConfig config;
  config.MergeMode = true;*/

  io.Fonts->AddFontFromFileTTF("../data/fonts/Hack-Regular.ttf",
                                17.0f);
  unsigned int flags = ImGuiFreeType::ForceAutoHint;
  ImGuiFreeType::BuildFontAtlas(io.Fonts, flags);


  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui::StyleColorsDark();

  glEnable              ( GL_DEBUG_OUTPUT );
  glDebugMessageCallback( MessageCallback, 0 );
  ImGui_ImplGlfw_InitForOpenGL(qpp::c_app::curWindow, true);
  ImGui_ImplOpenGL3_Init();

  glfwSetScrollCallback(qpp::c_app::curWindow, qpp::c_app::mouse_scroll_callback);
  glfwSetKeyCallback(qpp::c_app::curWindow, qpp::c_app::key_callback);
  glfwSetWindowSizeCallback(qpp::c_app::curWindow,
                            qpp::c_app::resize_window_callback);
  glfwSetMouseButtonCallback(qpp::c_app::curWindow,
                             qpp::c_app::mouse_button_callback);
  glfwSetCursorPosCallback(qpp::c_app::curWindow,
                           qpp::c_app::mouse_callback);

  qpp::c_app::log("qpp::cad initialized succesfully!");
  app_state_t* astate = &(c_app::get_state());

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  /// main app cycle

  float previous_time = glfwGetTime();
  int frame_count = 0;

  double _mouse_x, _mouse_y;

  while (!glfwWindowShouldClose(qpp::c_app::curWindow)){

      if(!glfwGetWindowAttrib(qpp::c_app::curWindow, GLFW_ICONIFIED)){
          glfwPollEvents();
          float current_time = glfwGetTime();
          float delta_time = current_time - previous_time;
          frame_count++;
          if ( delta_time >= 1.0f ){
              astate->FPS = frame_count;
              frame_count = 0;
              previous_time = current_time;
            }

          glfwGetCursorPos(c_app::curWindow, &_mouse_x, &_mouse_y);
          astate->update_mouse_coord(_mouse_x, _mouse_y);
          astate->update(delta_time);

          qpp::c_app::begin_render();
          qpp::c_app::render();
          qpp::c_app::end_render();
        }
      else glfwWaitEvents();
    }

  glfwDestroyWindow(qpp::c_app::curWindow);
  glfwTerminate();
  //exit(EXIT_SUCCESS);
}

void qpp::c_app::begin_render(){

  glfwMakeContextCurrent(qpp::c_app::curWindow);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  qpp::c_app::get_state().ui_manager->render_ui();

  float ratio;
  int width, height;
  glfwGetFramebufferSize(qpp::c_app::curWindow, &width, &height);

  ratio = width / static_cast<float>(height);
  glViewport(0, 0, width, height);

  if (c_app::get_state().cur_task == app_task_type::TASK_WORKSPACE_EDITOR){
      glClearColor(0.8f, 0.8f, 0.8f, 1);
    }

  if (c_app::get_state().cur_task == app_task_type::TASK_NODE_EDITOR){
      glClearColor(1, 1, 1, 1);
    }

  if (c_app::get_state().cur_task == app_task_type::TASK_MENDELEY_TABLE){
      glClearColor(0.9f, 0.9f, 0.9f, 1);
    }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void qpp::c_app::end_render(){
  glfwSwapBuffers(qpp::c_app::curWindow);
}

void qpp::c_app::render(){
  app_state_t* astate = &(c_app::get_state());
  float _viewport_w = 0.0f;
  if (!astate->show_object_inspector) _viewport_w = astate->ui_manager->iObjInspWidth;

  glViewport(astate->vViewportXY(0), astate->vViewportXY(1),
             astate->vViewportWidthHeight(0)+_viewport_w, astate->vViewportWidthHeight(1));

  if (astate->cur_task == app_task_type::TASK_WORKSPACE_EDITOR)
    astate->ws_manager->render_current_workspace();

  glViewport(0, 0, astate->wWidth, astate->wHeight);
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void qpp::c_app::resize_window_callback(GLFWwindow *window, int _width, int _height){
  app_state_t* astate = &(c_app::get_state());
  if (_width < 0)  _width = 10;
  if (_height < 0) _height = 10;
  astate->wWidth = _width;
  astate->wHeight = _height;
  astate->vViewportXY(0) = 0.0;
  astate->vViewportXY(1) = astate->ui_manager->iWorkPanelHeight +
      astate->ui_manager->iWorkPanelYOffset - 64;
  astate->vViewportWidthHeight(0) = _width - astate->ui_manager->iObjInspWidth;
  astate->vViewportWidthHeight(1) = _height -
      (astate->ui_manager->iWorkPanelHeight +
       astate->ui_manager->iWorkPanelYOffset) ;
}

void qpp::c_app::mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset){
  app_state_t* astate =  &(c_app::get_state());
  if ( astate->cur_task == app_task_type::TASK_WORKSPACE_EDITOR &&
       astate->camera != nullptr &&
       !astate->disable_mouse_camera_control &&
       !astate->config_vote_pool.is_voting_active(DISABLE_MOUSE_CONTROL_IN_WORKSPACE) &&
       astate->mouse_in_3d_area)
    astate->camera->update_camera_zoom(-yoffset);
}

void qpp::c_app::mouse_callback(GLFWwindow *window, double x, double y){
  //std::cout<<x<<" "<<y<<std::endl;
  //qpp::c_app::get_state().update_mouse_coord(x, y);
}

void qpp::c_app::mouse_button_callback(GLFWwindow *window, int button, int action, int mods){
  app_state_t* astate =  &(c_app::get_state());

  if (astate->config_vote_pool.is_voting_active(DISABLE_MOUSE_CONTROL_IN_WORKSPACE))
    return;

  if (astate->cur_task == app_task_type::TASK_WORKSPACE_EDITOR &&
      astate->camera != nullptr &&
      !astate->disable_mouse_camera_control){

      if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
          astate->mouse_lb_pressed = true;
          astate->ws_manager->mouse_click();
        }

      if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
          astate->mouse_lb_pressed = false;


      astate->camera->update_camera_rotation(
            button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS);

      astate->camera->update_camera_translation(
            button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS);
    }
}

void qpp::c_app::log(const std::string logText){

  std::time_t t = std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now());
  std::string ts( ctime( &t) );
  std::cout << fmt::format("[{}] {}",
                           ts.substr( 0, ts.length() -1  ), logText) << std::endl;
}

GLFWwindow* qpp::c_app::curWindow;
qpp::app_state_t* qpp::c_app::app_state;
