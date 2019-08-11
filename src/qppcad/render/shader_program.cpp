#include <qppcad/render/shader_program.hpp>
#include <qppcad/core/app_state.hpp>
//#include <qppcad/app.hpp>

qpp::cad::shader_program_t::shader_program_t(const std::string &_program_name,
                                             const std::string &_vs_text,
                                             const std::string &_fs_text) {

  app_state_t* astate = app_state_t::get_inst();
  glapi_t* glapi = astate->glapi;
  unf_rec.resize(qpp::cad::map_u2s.size());
  std::fill(unf_rec.begin(), unf_rec.end(), uniform_record({false, 0}));

  program_name = _program_name;
  GLuint vertexShaderID = glapi->glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentShaderID = glapi->glCreateShader(GL_FRAGMENT_SHADER);
  program_id = glapi->glCreateProgram();

  GLint proc_res = GL_FALSE;
  GLint vs_proc_res = GL_FALSE;
  GLint fs_proc_res = GL_FALSE;

  int infoLogLength = 0;

  const char *_vs_text_c = _vs_text.c_str();
  const char *_fs_text_c = _fs_text.c_str();

  glapi->glShaderSource(vertexShaderID, 1, &_vs_text_c, nullptr);
  glapi->glCompileShader(vertexShaderID);
  glapi->glShaderSource(fragmentShaderID, 1, &_fs_text_c, nullptr);
  glapi->glCompileShader(fragmentShaderID);

  glapi->glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &vs_proc_res);
  glapi->glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &fs_proc_res);

  astate->tlog("Program[{}] vs_sh_stat = {}, fs_sh_stat = {}",
               program_name, vs_proc_res, fs_proc_res);

  glapi->glAttachShader(program_id, vertexShaderID);
  glapi->glAttachShader(program_id, fragmentShaderID);
  glapi->glBindAttribLocation(program_id, 0, "vs_position");
  glapi->glBindAttribLocation(program_id, 1, "vs_normal");

  glapi->glLinkProgram(program_id);

  glapi->glGetProgramiv(program_id, GL_LINK_STATUS, &proc_res);
  glapi->glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &infoLogLength);

  astate->tlog("Shader program[{}] compilation status: {}", program_name, proc_res);

  if (infoLogLength > 0) {
      std::vector<char> ProgramErrorMessage(infoLogLength+1);
      glapi->glGetProgramInfoLog(program_id, infoLogLength, nullptr, &ProgramErrorMessage[0]);
      std::string str(ProgramErrorMessage.begin(), ProgramErrorMessage.end());
      astate->tlog("Shader/Program compilation/linking failed: {}", str);
    }

  glapi->glDeleteShader(vertexShaderID);
  glapi->glDeleteShader(fragmentShaderID);

}

void qpp::cad::shader_program_t::u_on(qpp::cad::sp_u_name _val) {

  app_state_t* astate = app_state_t::get_inst();
  glapi_t* glapi = astate->glapi;
  unf_rec[_val].enabled = true;
  unf_rec[_val].h_prog = glapi->glGetUniformLocation(program_id, map_u2s[_val].c_str());

  if (unf_rec[_val].h_prog == -1) {
      astate->tlog("WARNING: invalid uniform[{}] in program {}", map_u2s[_val], program_name);
    }
}

void qpp::cad::shader_program_t::set_u(qpp::cad::sp_u_name _ut, GLfloat *_val) {

  app_state_t* astate = app_state_t::get_inst();
  glapi_t* glapi = astate->glapi;

  if (unf_rec[_ut].enabled) {
      qpp::cad::sp_u_type _utype = qpp::cad::map_u2at[_ut];
      GLint uloc = unf_rec[_ut].h_prog;
      switch(_utype){

        case qpp::cad::sp_u_type::a_v3f :
          glapi->glUniform3fv(uloc, 1, _val);
          break;

        case qpp::cad::sp_u_type::a_m4f :
          glapi->glUniformMatrix4fv(uloc, 1, GL_FALSE, _val);
          break;

        case qpp::cad::sp_u_type::a_m3f :
          glapi->glUniformMatrix3fv(uloc, 1, GL_FALSE, _val);
          break;

        case qpp::cad::sp_u_type::a_sf :
          glapi->glUniform1fv(uloc, 1, _val);
          break;

        default:
          break;
        }
    }

  else {

    }
}

void qpp::cad::shader_program_t::set_u_sampler(qpp::cad::sp_u_name _ut, GLint val) {

  app_state_t* astate = app_state_t::get_inst();
  glapi_t* glapi = astate->glapi;

  if (unf_rec[_ut].enabled) {
      //qpp::cad::sp_u_type _utype = qpp::cad::map_u2at[_ut];
      GLint uloc = unf_rec[_ut].h_prog;
      glapi->glUniform1i(uloc, val);
    }

}

void qpp::cad::shader_program_t::begin_shader_program() {

  app_state_t* astate = app_state_t::get_inst();
  glapi_t* glapi = astate->glapi;

  glapi->glUseProgram(program_id);

  //  if (unf_rec[sp_u_name::v_light_pos].enabled)
  //    set_u(sp_u_name::v_light_pos, c_app::get_state().light_pos_tr.data());

  if (unf_rec[sp_u_name::texture_0].enabled)
    glapi->glUniform1i(unf_rec[sp_u_name::texture_0].h_prog, 0);

  //  if (unf_rec[sp_u_name::screen_width].enabled)
  //    glUniform1i(unf_rec[sp_u_name::screen_width].h_prog,
  //        int(astate->viewport_size_c(0)));

  //  if (unf_rec[sp_u_name::screen_height].enabled)
  //    glUniform1i(unf_rec[sp_u_name::screen_height].h_prog,
  //        int(astate->viewport_size_c(1)));
  //  if (unf_rec[sp_u_name::m_model_view_inv_tr].enabled && astate->camera) {
  //      set_u(sp_u_name::m_model_view_inv_tr, astate->camera->m);
  //    }

  if (unf_rec[sp_u_name::v_eye_pos].enabled && astate->camera) {
      set_u(sp_u_name::v_eye_pos, astate->camera->m_view_point.data());
    }

}

void qpp::cad::shader_program_t::end_shader_program() {

  app_state_t* astate = app_state_t::get_inst();
  glapi_t* glapi = astate->glapi;
  glapi->glUseProgram(0);

}



