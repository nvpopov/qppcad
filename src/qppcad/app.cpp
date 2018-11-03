#include <qppcad/app.hpp>
#include <clocale>
#include <algorithm>
#include <thread>
#include <args.hxx>
#include <qppcad/font.inc>

void
MessageCallback ( GLenum source,
                  GLenum type,
                  GLuint id,
                  GLenum severity,
                  GLsizei length,
                  const GLchar* message,
                  const void* userParam ) {

  if (type == GL_DEBUG_TYPE_ERROR){
      fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
               ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
               type, severity, message );
      exit(0);
    }
}

void qpp::cad::c_app::error_callback (int error, const char* description) {

  std::string s1(description);
  qpp::cad::c_app::log(s1);

}

void qpp::cad::c_app::key_callback (GLFWwindow* window, int key, int scancode, int action,
                                    int mods) {

  //app_state_t* astate = &(c_app::get_state());

  qpp::cad::c_app::log(fmt::format("Key pressed  {}, sc = {}, act = {}", key, scancode, action));

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

}

void qpp::cad::c_app::run (int argc, char **argv) {

  std::setlocale(LC_ALL, "C");
  qpp::cad::c_app::m_is_state_initialized = false;

  //process args
  args::ArgumentParser parser("qpp::cad program for view and edit data for quantum chemistry"
                              " calculations");
  args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
  args::ValueFlag<std::string> render_mode(parser, "direct, fb_legacy, fb_ms",
                                           "Selected render mode", {'r', "render_mode"});
  args::ValueFlag<uint> glfw_samples(parser, "0..12", "Selected glfw window samples amount",
  {'s', "n_samples"});

  try {
    parser.ParseCLI(argc, argv);
  }

  catch (const args::Completion& e) {
    std::cout << e.what();
    return;
  }

  catch (const args::Help&){
    std::cout << parser;
    return;
  }

  catch (const args::ParseError& e){
    std::cerr << e.what() << std::endl;
    std::cerr << parser;
    return ;
  }

  app_render_mode render_mode_from_args = app_render_mode::buffered_multi_sampling;
  std::string str_render_mode = args::get(render_mode);

  int aa_level = args::get(glfw_samples);
  if (str_render_mode == "direct")
    render_mode_from_args = app_render_mode::direct;
  else if (str_render_mode == "fb_legacy")
    render_mode_from_args = app_render_mode::buffered_legacy;

  //on modern hardware force ms level if it didnt set in options
  if (!glfw_samples && render_mode_from_args == app_render_mode::buffered_multi_sampling)
    aa_level = 12;
  //std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@ " << render_mode_from_args << std::endl;
  //end process args

  glfwSetErrorCallback(qpp::cad::c_app::error_callback);

  if (!glfwInit()){
      qpp::cad::c_app::log("Failed to initialize GLFW3");
      exit(EXIT_FAILURE);
    }

  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  if (aa_level > 0 && render_mode_from_args == app_render_mode::direct) {
      glfwWindowHint(GLFW_SAMPLES, aa_level);
    }

  qpp::cad::c_app::curWindow = glfwCreateWindow(800, 600, "qpp::cad", nullptr, nullptr);

  if (!qpp::cad::c_app::curWindow){
      glfwTerminate();
      qpp::cad::c_app::log("Failed to initialize window");
      exit(EXIT_FAILURE);
    }

  glfwMakeContextCurrent(qpp::cad::c_app::curWindow);
  glfwSwapInterval(1);

  if (gl3wInit()) {
      qpp::cad::c_app::log("Failed  to initialize OpenGL");
    }

  //log renderer stuff
  const GLubyte* vendor = glGetString(GL_VENDOR);
  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version = glGetString(GL_VERSION);
  c_app::log(fmt::format("OpenGL summary: vendor={}, renderer={}, version={}",
                         vendor,
                         renderer,
                         version));
  //end log renderer stuff

  // Start setup imgui
  c_app::m_context_ui = ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  //io.Fonts->AddFontDefault();
  /*ImWchar ranges[] = { 0xf000, 0xf3ff, 0 };
  ImFontConfig config;
  config.MergeMode = true;*/

  //io.Fonts->AddFontFromFileTTF("../data/fonts/Open_Sans/OpenSans-Regular.ttf", 22.0f);
  io.Fonts->AddFontFromMemoryCompressedTTF((void*)fontn_compressed_data, fontn_compressed_size,
                                           21.0f);

  unsigned int flags = ImGuiFreeType::LightHinting;
  ImGuiFreeType::BuildFontAtlas(io.Fonts, flags);
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui::StyleColorsDark();

  c_app::m_context_overlay = ImGui::CreateContext(io.Fonts);
  ImGui::SetCurrentContext(c_app::m_context_ui);

  ImGui_ImplGlfw_InitForOpenGL(qpp::cad::c_app::curWindow, true);
  ImGui_ImplOpenGL3_Init();
  // End setup imgui

  glfwSetScrollCallback(qpp::cad::c_app::curWindow, qpp::cad::c_app::mouse_scroll_callback);
  glfwSetKeyCallback(qpp::cad::c_app::curWindow, qpp::cad::c_app::key_callback);
  glfwSetWindowSizeCallback(qpp::cad::c_app::curWindow, qpp::cad::c_app::resize_window_callback);
  glfwSetMouseButtonCallback(qpp::cad::c_app::curWindow, qpp::cad::c_app::mouse_button_callback);
  glfwSetCursorPosCallback(qpp::cad::c_app::curWindow, qpp::cad::c_app::mouse_callback);

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(MessageCallback, 0);

  c_app::app_state = new qpp::cad::app_state_t();
  app_state_t* astate = &(c_app::get_state());
  qpp::cad::c_app::log("qpp::cad initialized succesfully!");

  qpp::cad::c_app::log(fmt::format("GL_MAX_TEXTURE_BUFFER_SIZE = {}",
                                   astate->c_max_texture_buffer_size));

  qpp::cad::c_app::m_is_state_initialized = true;
  astate->ws_manager->init_default();

  if (aa_level > 0) {
      glfwWindowHint(GLFW_SAMPLES, aa_level);
      glEnable(GL_MULTISAMPLE);
      astate->m_viewport_ms_level = aa_level;
    }

  astate->m_render_mode = render_mode_from_args;

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  /// main app cycle
  ///
  int width, height;
  glfwGetFramebufferSize(qpp::cad::c_app::curWindow, &width, &height);

  float previous_time = glfwGetTime();
  int frame_count = 0;
  double _mouse_x, _mouse_y;

  astate->viewport_xy(0) = 0.0;
  astate->viewport_xy(1) = 0.0;
  astate->viewport_size(0) = width;
  astate->viewport_size(1) = height;
  astate->update_viewport_cache();

  if (astate->m_render_mode != app_render_mode::direct) {
      astate->frame_buffer =
          std::make_unique<frame_buffer_t<frame_buffer_opengl_provider> >(
            astate->viewport_size_c(0),
            astate->viewport_size_c(1),
            astate->m_render_mode == app_render_mode::buffered_multi_sampling,
            astate->m_viewport_ms_level);
    }

  auto start = std::chrono::system_clock::now();

  while (!glfwWindowShouldClose(qpp::cad::c_app::curWindow)) {

      if (!glfwGetWindowAttrib(qpp::cad::c_app::curWindow, GLFW_ICONIFIED)) {

          glfwPollEvents();

          frame_count++;
          std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
          std::chrono::duration<float, std::milli>  diff = now - start;
          //std::chrono::duration<long, std::ration<1, 1000000
          auto end = now + std::chrono::milliseconds(static_cast<int>(
                                                       (1.0f / astate->max_fps)*1000.0f));

          if (diff >= std::chrono::seconds(1)) {
              start = now;
              astate->current_fps = std::clamp(frame_count, 0, astate->max_fps);
              frame_count = 0;
            }

          astate->kb_manager->update(qpp::cad::c_app::curWindow);

          glfwGetCursorPos(c_app::curWindow, &_mouse_x, &_mouse_y);
          astate->update_mouse_coord(_mouse_x, _mouse_y);

          auto secs = std::chrono::duration_cast<std::chrono::duration<float> >(diff);

          astate->update(secs.count());

          qpp::cad::c_app::begin_render();
          qpp::cad::c_app::render();
          qpp::cad::c_app::end_render();

          std::this_thread::sleep_until(end);

        }

      else glfwWaitEvents();

    }

  glfwDestroyWindow(qpp::cad::c_app::curWindow);
  glfwTerminate();
  //exit(EXIT_SUCCESS);

}

void qpp::cad::c_app::copy_to_clipboard (const char * dat) {
  glfwSetClipboardString(c_app::curWindow, dat);
}

void qpp::cad::c_app::begin_render () {

  app_state_t* astate = &(c_app::get_state());

  glfwMakeContextCurrent(qpp::cad::c_app::curWindow);

//  ImGui_ImplOpenGL3_NewFrame();
//  ImGui_ImplGlfw_NewFrame();

  ImGui::SetCurrentContext(c_app::m_context_overlay);
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  astate->ws_manager->render_current_workspace_overlay();

  ImGui::SetCurrentContext(c_app::m_context_ui);
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  qpp::cad::c_app::get_state().ui_manager->render_ui();

  float ratio;
  int width, height;
  glfwGetFramebufferSize(qpp::cad::c_app::curWindow, &width, &height);

  ratio = width / static_cast<float>(height);
  glViewport(0, 0,
             static_cast<int>(astate->viewport_size(0)),
             static_cast<int>(astate->viewport_size(1)));

  if (qpp::cad::c_app::get_state().cur_task == app_task_type::TASK_WORKSPACE_EDITOR){
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

void qpp::cad::c_app::end_render(){
  glfwSwapBuffers(qpp::cad::c_app::curWindow);
}

void qpp::cad::c_app::render () {

  app_state_t* astate = &(c_app::get_state());

  if (astate->cur_task == app_task_type::TASK_WORKSPACE_EDITOR){
      switch (astate->m_render_mode) {
        case app_render_mode::direct:
          qpp::cad::c_app::render_direct();
          break;
        default:
          qpp::cad::c_app::render_fb();
          break;
        }
    }

  // Restore viewport for gui drawing
  glViewport(0, 0,
             static_cast<int>(astate->viewport_size(0)),
             static_cast<int>(astate->viewport_size(1)));

  ImGui::SetCurrentContext(c_app::m_context_overlay);
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  ImGui::SetCurrentContext(c_app::m_context_ui);
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void qpp::cad::c_app::render_direct () {

  app_state_t* astate = &(c_app::get_state());

  glViewport(static_cast<int>(astate->viewport_xy_c(0)),
             static_cast<int>(0),
             static_cast<int>(astate->viewport_size_c(0)),
             static_cast<int>(astate->viewport_size_c(1)));

  astate->ws_manager->render_current_workspace();

}

void qpp::cad::c_app::render_fb () {

  app_state_t* astate = &(c_app::get_state());

  if (astate->m_viewport_dirty || astate->m_realtime || astate->m_workspace_changed){
      //c_app::log("new frame rendered");
      astate->frame_buffer->bind();
      glViewport(0, 0,
                 static_cast<int>(astate->viewport_size_c[0]),
                 static_cast<int>(astate->viewport_size_c[1]));
      astate->ws_manager->render_current_workspace();
      astate->frame_buffer->unbind();
    }

  glViewport(static_cast<int>(astate->viewport_xy_c[0]),
             0,
             static_cast<int>(astate->viewport_size_c[0]),
             static_cast<int>(astate->viewport_size_c[1]));

  astate->dp->render_screen_quad();

  if (!astate->m_viewport_dirty) astate->m_workspace_changed = false;
  astate->m_viewport_dirty = false;

}

void qpp::cad::c_app::resize_window_callback (GLFWwindow *window, int _width, int _height) {

  app_state_t* astate = &(c_app::get_state());

  // to prevent early initialization bug
  if (_width < 0)  _width = 10;
  if (_height < 0) _height = 10;

  astate->viewport_xy(0) = 0.0;
  astate->viewport_xy(1) = 0.0;
  astate->viewport_size(0) = _width;
  astate->viewport_size(1) = _height;
  astate->update_viewport_cache();

  if (astate->m_render_mode != app_render_mode::direct)
    astate->frame_buffer->resize(
          static_cast<uint16_t>(astate->viewport_size_c[0]),
          static_cast<uint16_t>(astate->viewport_size_c[1]));

  astate->make_viewport_dirty();
}

void qpp::cad::c_app::mouse_scroll_callback (GLFWwindow *window, double xoffset, double yoffset) {

  app_state_t* astate =  &(c_app::get_state());

  if ( astate->cur_task == app_task_type::TASK_WORKSPACE_EDITOR &&
       astate->camera != nullptr &&
       !astate->config_vote_pool.is_voting_active(DISABLE_MOUSE_CONTROL_IN_WORKSPACE) &&
       astate->mouse_in_3d_area &&
       !ImGui::GetIO().WantCaptureMouse)
    astate->camera->update_camera_zoom(-yoffset);

  ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);

}

void qpp::cad::c_app::mouse_callback (GLFWwindow *window, double x, double y) {
  //std::cout<<x<<" "<<y<<std::endl;
  //qpp::c_app::get_state().update_mouse_coord(x, y);
}

void qpp::cad::c_app::mouse_button_callback (GLFWwindow *window, int button, int action, int mods){

  app_state_t* astate =  &(c_app::get_state());
  astate->make_viewport_dirty();

  if (astate->config_vote_pool.is_voting_active(DISABLE_MOUSE_CONTROL_IN_WORKSPACE)){
      //std::cout<<"VOTING POINT\n";
      astate->mouse_in_3d_area = false;
      astate->mouse_lb_pressed = false;
      return;
    }

  if (astate->cur_task == app_task_type::TASK_WORKSPACE_EDITOR &&
      astate->camera != nullptr ){

      if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
          astate->mouse_lb_pressed = true;
          astate->ws_manager->mouse_click();
        }

      if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        astate->mouse_lb_pressed = false;

      if (!astate->mouse_in_3d_area) astate->mouse_lb_pressed = false;

      astate->camera->update_camera_rotation(
            button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS);

      astate->camera->update_camera_translation(
            button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS);
    }

}

void qpp::cad::c_app::update_window_title (const std::string &new_title) {

  std::string tmp_title = "qpp::cad " + new_title;
  glfwSetWindowTitle(qpp::cad::c_app::curWindow, tmp_title.c_str());

}

void qpp::cad::c_app::log (const std::string &logText) {

  std::setlocale(LC_ALL, "C");
  std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::string ts( ctime( &t) );
  std::cout << fmt::format("[{}] {}\n", ts.substr( 0, ts.length() -1  ), logText);

}

GLFWwindow* qpp::cad::c_app::curWindow;
ImGuiContext* qpp::cad::c_app::m_context_ui;
ImGuiContext* qpp::cad::c_app::m_context_overlay;
qpp::cad::app_state_t* qpp::cad::c_app::app_state;
bool qpp::cad::c_app::m_is_state_initialized;
