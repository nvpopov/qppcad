#include <qppcad/shader_program.hpp>
#include <qppcad/app.hpp>

qpp::shader_program_t::shader_program_t(const std::string _program_name, const std::string &_vs_text,
                                        const std::string &_fs_text){

  unf_rec.resize(qpp::map_u2s.size());
  std::fill(unf_rec.begin(), unf_rec.end(), uniform_record({false, 0}));

  program_name = _program_name;
  GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
  program_id = glCreateProgram();

  GLint proc_res = GL_FALSE;
  GLint vs_proc_res = GL_FALSE;
  GLint fs_proc_res = GL_FALSE;

  int infoLogLength = 0;

  const char *_vs_text_c = _vs_text.c_str();
  const char *_fs_text_c = _fs_text.c_str();

  glShaderSource(vertexShaderID, 1, &_vs_text_c, NULL);
  glCompileShader(vertexShaderID);
  glShaderSource(fragmentShaderID, 1, &_fs_text_c, NULL);
  glCompileShader(fragmentShaderID);

  glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &vs_proc_res);
  glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &fs_proc_res);

  c_app::log(fmt::format("Program[{}] vs_sh_stat = {}, fs_sh_stat = {}",
                         program_name, vs_proc_res, fs_proc_res));
  glAttachShader(program_id, vertexShaderID);
  glAttachShader(program_id, fragmentShaderID);
  glBindAttribLocation(program_id, 0, "vs_position");
  glBindAttribLocation(program_id, 1, "vs_normal");

  glLinkProgram(program_id);

  glGetProgramiv(program_id, GL_LINK_STATUS, &proc_res);
  glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &infoLogLength);

  c_app::log("Shader program["+program_name+"] compilation status:" +
             std::to_string(proc_res));

  if (infoLogLength > 0){
      c_app::log("Shader/Program compilation/linking failed:");
      std::vector<char> ProgramErrorMessage(infoLogLength+1);
      glGetProgramInfoLog(program_id, infoLogLength, NULL, &ProgramErrorMessage[0]);
      std::string str(ProgramErrorMessage.begin(), ProgramErrorMessage.end());
      c_app::log(str);
    }

  glDeleteShader(vertexShaderID);
  glDeleteShader(fragmentShaderID);

}

void qpp::shader_program_t::u_on(qpp::sp_u_name _val){
  //std::cout<<_val << " "<< unfRec.size() << std::endl;
  unf_rec[_val].bEnabled = true;
  unf_rec[_val].iProgVar = glGetUniformLocation(program_id, map_u2s[_val].c_str());
}

void qpp::shader_program_t::set_u(qpp::sp_u_name _ut,
                                GLfloat *_val){
  if (unf_rec[_ut].bEnabled){
      qpp::sp_u_type _utype = qpp::map_u2at[_ut];
      GLint uloc = unf_rec[_ut].iProgVar;
      switch(_utype){

        case qpp::sp_u_type::a_v3f :
          glUniform3fv(uloc, 1, _val);
          break;

        case qpp::sp_u_type::a_m4f :
          glUniformMatrix4fv(uloc, 1, GL_FALSE, _val);
          break;

        case qpp::sp_u_type::a_m3f :
          glUniformMatrix3fv(uloc, 1, GL_FALSE, _val);
          break;

        case qpp::sp_u_type::a_sf :
          glUniform1fv(uloc, 1, _val);
          break;

         default:
          break;
        }
    }

  else {

    }

}

void qpp::shader_program_t::begin_shader_program(){
  glUseProgram(program_id);

  /*
  set_u(sp_u_name::mModelViewProj, c_app::get_state().mViewProjection.data());
  set_u(sp_u_name::mModelView, c_app::get_state().mView.data());
  set_u(sp_u_name::mViewInvTr, c_app::get_state().mViewInvTr.data());
  */
   set_u(sp_u_name::v_light_pos, c_app::get_state().vLightPosTr.data());
  //std::cout<<std::endl<<c_app::get_state().mView<<std::endl;
}

void qpp::shader_program_t::end_shader_program(){
  glUseProgram(0);
}

qpp::shader_program_t* qpp::gen_default_program(){
  std::string vs =
      "#version 330\n"
      "uniform mat4 m_model_view;\n"
      "uniform mat4 m_model_view_proj;\n"
      "uniform mat4 m_model_view_inv_tr;\n"
      "uniform vec3 v_translate;\n"
      "uniform float f_scale;\n"
      "in vec3 vs_position;\n"
      "in vec3 vs_normal;\n"
      "out vec3 fs_normal;\n"
      "out vec3 fs_position;\n"
      "void main(void)\n"
      "{\n"
      "  vec3 tr_position = (vs_position*f_scale + v_translate);\n"
      "  fs_normal = vec3(m_model_view_inv_tr * vec4(vs_normal,0.0));\n"
      "  fs_position = vec3(m_model_view * vec4(tr_position, 1.0));\n"
      "  gl_Position = m_model_view_proj * vec4(tr_position, 1.0);\n"
      "}";

  std::string fs =
      "#version 330\n"
      "uniform vec3 v_light_pos;\n"
      "uniform vec3 v_color;\n"
      "in vec3 fs_normal;\n"
      "in vec3 fs_position;\n"
      "out vec4 color;\n"
      "void main(void)\n"
      "{\n"
      "  //float distance = length(v_light_pos - fs_position);\n"
      "  vec3 sp_light_pos = normalize(vec3(0, 1, 1));\n"
      "  vec3 light_vector = normalize(sp_light_pos - fs_position);\n"
      "  float diffuse = max(dot(fs_normal, light_vector), 0.05);\n"
      "  // diffuse = diffuse * (1.0 / (1.0 + (0.25 * distance * distance)));\n"
      "  vec4 ambient = vec4(0.01, 0.01, 0.01, 1.0);\n"
      "  vec3 gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2);\n"
      "  vec4 linear_color =  vec4(v_color, 1.0) * diffuse;\n"
      "  color = vec4(pow(linear_color.r, gamma.r),\n"
      "                  pow(linear_color.g, gamma.g),\n"
      "                  pow(linear_color.b, gamma.b), 1.0);\n"
      "}\n";

  qpp::shader_program_t *sp = new qpp::shader_program_t(std::string("default_program"), vs, fs);
  sp->u_on(sp_u_name::m_model_view_proj);
  sp->u_on(sp_u_name::m_model_view);
  sp->u_on(sp_u_name::m_model_view_inv_tr);
  sp->u_on(sp_u_name::v_light_pos);
  sp->u_on(sp_u_name::v_translate);
  sp->u_on(sp_u_name::f_scale);
  sp->u_on(sp_u_name::v_color);
  return sp;
}

qpp::shader_program_t *qpp::gen_unit_line_program(){
  std::string vs =
      "#version 330\n"
      "uniform mat4 m_model_view;\n"
      "uniform mat3 m_model_view_inv_tr;\n"
      "uniform mat4 m_model_view_proj;\n"
      "uniform vec3 v_line_start;\n"
      "uniform vec3 v_line_end;\n"
      "in vec3 vs_position;\n"
      "out vec3 fs_position;\n"
      "void main(void)\n"
      "{\n"
      "  vec3 tr_position = mix(v_line_start, v_line_end, vs_position.x);\n"
      "  gl_Position = m_model_view_proj * vec4(tr_position, 1.0);\n"
      "}";

  std::string fs =
      "#version 330\n"
      "uniform vec3 v_color;\n"
      "in vec3 fs_position;\n"
      "out vec4 color;\n"
      "void main(void)\n"
      "{\n"
      "  color = vec4(v_color, 1.0);\n"
      "}\n";

  qpp::shader_program_t *sp = new qpp::shader_program_t(std::string("unit_line_program"), vs, fs);
  sp->u_on(sp_u_name::m_model_view_proj);
  sp->u_on(sp_u_name::m_view);
//  sp->u_on(sp_u_name::vLightPos);
  sp->u_on(sp_u_name::m_view_inv_tr);
//  sp->u_on(sp_u_name::vTranslate);
//  sp->u_on(sp_u_name::fScale);
  sp->u_on(sp_u_name::v_color);
  sp->u_on(sp_u_name::v_line_start);
  sp->u_on(sp_u_name::v_line_end);
  return sp;
}

qpp::shader_program_t *qpp::gen_bond_draw_program(){
  std::string vs =
      "#version 330\n"
      "uniform mat4 m_model_view;\n"
      "uniform mat4 m_model_view_inv_tr;\n"
      "uniform mat4 m_model_view_proj;\n"
      "in vec3 vs_position;\n"
      "in vec3 vs_normal;\n"
      "out vec3 fs_normal;\n"
      "out vec3 fs_position;\n"
      "void main(void)\n"
      "{\n"
      "  vec3 tr_position = (vs_position);\n"
      "  fs_normal = normalize(vec3(m_model_view_inv_tr * vec4(vs_normal,0.0)));\n"
      "  fs_position = vec3(m_model_view * vec4(tr_position, 1.0));\n"
      "  gl_Position = m_model_view_proj * vec4(tr_position, 1.0);\n"
      "}";

  std::string fs =
      "#version 330\n"
      "uniform vec3 v_light_pos;\n"
      "uniform vec3 v_color;\n"
      "in vec3 fs_normal;\n"
      "in vec3 fs_position;\n"
      "out vec4 color;\n"
      "void main(void)\n"
      "{\n"
      "  //float distance = length(v_light_pos - fs_position);\n"
     // "  vec3 lightVector = normalize(vLightPos - fs_Position);\n"
      "  vec3 sp_light_pos = (vec3(0, 1, 1));\n"
      "  vec3 light_vector = normalize(sp_light_pos - fs_position);\n"
      "  float diffuse = max(dot(fs_normal, light_vector), 0.1);\n"
      "  // diffuse = diffuse * (1.0 / (1.0 + (0.25 * distance * distance)));\n"
      "  vec4 ambient = vec4(0.05, 0.05, 0.05, 1.0);\n"
      "  vec3 gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2);\n"
      "  vec4 linear_color =  vec4(v_color, 1.0) * diffuse;\n"
      "  color = vec4(pow(linear_color.r, gamma.r),\n"
      "                  pow(linear_color.g, gamma.g),\n"
      "                  pow(linear_color.b, gamma.b), 1.0);\n"
      "}\n";

  qpp::shader_program_t *sp = new qpp::shader_program_t(std::string("bond_program"), vs, fs);
  sp->u_on(sp_u_name::m_model_view_proj);
  sp->u_on(sp_u_name::m_model_view);
  sp->u_on(sp_u_name::m_model_view_inv_tr);
  sp->u_on(sp_u_name::v_light_pos);
  sp->u_on(sp_u_name::v_color);
 // sp->u_on(sp_u_name::mModelViewNoScale);
  return sp;
}

qpp::shader_program_t *qpp::gen_line_mesh_program(){
  std::string vs =
      "#version 330\n"
      "uniform mat4 m_model_view;\n"
      "uniform mat3 m_model_view_inv_tr;\n"
      "uniform mat4 m_model_view_proj;\n"
      "uniform vec3 v_translate;\n"
      "in vec3 vs_position;\n"
      "out float fs_z;\n"
      "void main(void)\n"
      "{\n"
      "  vec4 tr_position =  m_model_view_proj * vec4(vs_position + v_translate, 1.0);\n"
      "  fs_z = tr_position.z ;\n"
      "  gl_Position = tr_position;\n"
      "}";

  std::string fs =
      "#version 330\n"
      "uniform vec3 v_color;\n"
      "in float fs_z;\n"
      "out vec4 color;\n"
      "void main(void)\n"
      "{\n"
      "  float effect_alpha = exp(-0.6 * max(0.0, 76- fs_z));"
      "  vec4 bg_color = vec4(0.4, 0.4, 0.4, 1.0);\n"
      "  color = mix(vec4(v_color, 1), bg_color, effect_alpha);\n"
      "}\n";

  qpp::shader_program_t *sp = new qpp::shader_program_t(std::string("grid_program"), vs, fs);
  sp->u_on(sp_u_name::m_model_view_proj);
  sp->u_on(sp_u_name::m_model_view);
//  sp->u_on(sp_u_name::vLightPos);
  sp->u_on(sp_u_name::m_view_inv_tr);
  sp->u_on(sp_u_name::v_translate);
//  sp->u_on(sp_u_name::fScale);
  sp->u_on(sp_u_name::v_color);
  return sp;
}

qpp::shader_program_t *qpp::gen_screen_space_lighting_program(){
  std::string vs =
      "#version 330\n"
      "uniform mat4 m_model_view;\n"
      "uniform mat4 m_model_view_proj;\n"
      "uniform mat4 m_model_view_inv_tr;\n"
      "uniform vec3 v_translate;\n"
      "uniform float f_scale;\n"
      "in vec3 vs_position;\n"
      "in vec3 vs_normal;\n"
      "out vec3 fs_normal;\n"
      "out vec3 fs_position;\n"
      "void main(void)\n"
      "{\n"
      "  vec3 tr_position = (vs_position * f_scale + v_translate);\n"
      "  fs_normal = vec3(m_model_view_inv_tr * vec4(vs_normal,0.0));\n"
      "  fs_position = vec3(m_model_view * vec4(tr_position, 1.0));\n"
      "  gl_Position = mMVP * vec4(tr_position, 1.0);\n"
      "}";

  std::string fs =
      "#version 330\n"
      "uniform vec3 v_light_pos;\n"
      "uniform vec3 v_color;\n"
      "in vec3 fs_normal;\n"
      "in vec3 fs_position;\n"
      "out vec4 color;\n"
      "void main(void)\n"
      "{\n"
      "  //float distance = length(v_light_pos - fs_position);\n"
      "  vec3 light_vector = normalize(v_light_pos - fs_position);\n"
      "  float diffuse = max(dot(fs_normal, light_vector), 0.1);\n"
      "  // diffuse = diffuse * (1.0 / (1.0 + (0.25 * distance * distance)));\n"
      "  vec4 ambient = vec4(0.05, 0.05, 0.05, 1.0);\n"
      "  vec3 gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2);\n"
      "  vec4 linear_color =  vec4(v_color, 1.0) * diffuse;\n"
      "  Color = vec4(pow(linear_color.r, gamma.r),\n"
      "                  pow(linear_color.g, gamma.g),\n"
      "                  pow(linear_color.b, gamma.b), 1.0);\n"
      "}\n";

  qpp::shader_program_t *sp = new qpp::shader_program_t(std::string("default_program"), vs, fs);
  sp->u_on(sp_u_name::m_model_view_proj);
  sp->u_on(sp_u_name::m_model_view);
  sp->u_on(sp_u_name::m_model_view_inv_tr);
  sp->u_on(sp_u_name::v_light_pos);
  sp->u_on(sp_u_name::v_translate);
  sp->u_on(sp_u_name::f_scale);
  sp->u_on(sp_u_name::v_color);
  return sp;
}
