#include <qppcad/shader_program.hpp>
#include <qppcad/app.hpp>

qpp::shader_program_t::shader_program_t(const std::string _programName, const std::string &_vs_text,
                                        const std::string &_fs_text){

  unfRec.resize(qpp::map_u2s.size());
  std::fill(unfRec.begin(), unfRec.end(), uniform_record({false, 0}));

  programName = _programName;
  GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
  programID = glCreateProgram();

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
                         programName, vs_proc_res, fs_proc_res));
  glAttachShader(programID, vertexShaderID);
  glAttachShader(programID, fragmentShaderID);
  glBindAttribLocation(programID, 0, "vs_Position");
  glBindAttribLocation(programID, 1, "vs_Normal");

  glLinkProgram(programID);

  glGetProgramiv(programID, GL_LINK_STATUS, &proc_res);
  glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

  c_app::log("Shader program["+programName+"] compilation status:" +
             std::to_string(proc_res));

  if (infoLogLength > 0){
      c_app::log("Shader/Program compilation/linking failed:");
      std::vector<char> ProgramErrorMessage(infoLogLength+1);
      glGetProgramInfoLog(programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
      std::string str(ProgramErrorMessage.begin(), ProgramErrorMessage.end());
      c_app::log(str);
    }

  glDeleteShader(vertexShaderID);
  glDeleteShader(fragmentShaderID);

}

void qpp::shader_program_t::u_on(qpp::sp_u_name _val){
  //std::cout<<_val << " "<< unfRec.size() << std::endl;
  unfRec[_val].bEnabled = true;
  unfRec[_val].iProgVar = glGetUniformLocation(programID, map_u2s[_val].c_str());
}

void qpp::shader_program_t::set_u(qpp::sp_u_name _ut,
                                GLfloat *_val){
  if (unfRec[_ut].bEnabled){
      qpp::sp_u_type _utype = qpp::map_u2at[_ut];
      GLint uloc = unfRec[_ut].iProgVar;
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
  glUseProgram(programID);

  /*
  set_u(sp_u_name::mModelViewProj, c_app::get_state().mViewProjection.data());
  set_u(sp_u_name::mModelView, c_app::get_state().mView.data());
  set_u(sp_u_name::mViewInvTr, c_app::get_state().mViewInvTr.data());
  */
   set_u(sp_u_name::vLightPos, c_app::get_state().vLightPosTr.data());
  //std::cout<<std::endl<<c_app::get_state().mView<<std::endl;
}

void qpp::shader_program_t::end_shader_program(){
  glUseProgram(0);
}

qpp::shader_program_t* qpp::gen_default_program(){
  std::string vs =
      "#version 330\n"
      "uniform mat4 mMV;\n"
      "uniform mat4 mMVP;\n"
      "uniform mat4 mMV_InvTr;\n"
      "uniform vec3 vTranslate;\n"
      "uniform float fScale;\n"
      "in vec3 vs_Position;\n"
      "in vec3 vs_Normal;\n"
      "out vec3 fs_Normal;\n"
      "out vec3 fs_Position;\n"
      "void main(void)\n"
      "{\n"
      "  vec3 tr_Position = (vs_Position*fScale + vTranslate);\n"
      "  fs_Normal = vec3(mMV_InvTr * vec4(vs_Normal,0.0));\n"
      "  fs_Position = vec3(mMV * vec4(tr_Position, 1.0));\n"
      "  gl_Position = mMVP * vec4(tr_Position, 1.0);\n"
      "}";

  std::string fs =
      "#version 330\n"
      "uniform vec3 vLightPos;\n"
      "uniform vec3 vColor;\n"
      "in vec3 fs_Normal;\n"
      "in vec3 fs_Position;\n"
      "out vec4 Color;\n"
      "void main(void)\n"
      "{\n"
      "  //float distance = length(vLightPos - fs_Position);\n"
      "  vec3 spLightPos = normalize(vec3(0, 1, 1));\n"
      "  vec3 lightVector = normalize(spLightPos - fs_Position);\n"
      "  float diffuse = max(dot(fs_Normal, lightVector), 0.05);\n"
      "  // diffuse = diffuse * (1.0 / (1.0 + (0.25 * distance * distance)));\n"
      "  vec4 ambient = vec4(0.01, 0.01, 0.01, 1.0);\n"
      "  vec3 gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2);\n"
      "  vec4 linearColor =  vec4(vColor, 1.0) * diffuse;\n"
      "  Color = vec4(pow(linearColor.r, gamma.r),\n"
      "                  pow(linearColor.g, gamma.g),\n"
      "                  pow(linearColor.b, gamma.b), 1.0);\n"
      "}\n";

  qpp::shader_program_t *sp = new qpp::shader_program_t(std::string("default_program"), vs, fs);
  sp->u_on(sp_u_name::mModelViewProj);
  sp->u_on(sp_u_name::mModelView);
  sp->u_on(sp_u_name::mModelViewInvTr);
  sp->u_on(sp_u_name::vLightPos);
  sp->u_on(sp_u_name::vTranslate);
  sp->u_on(sp_u_name::fScale);
  sp->u_on(sp_u_name::vColor);
  return sp;
}

qpp::shader_program_t *qpp::gen_unit_line_program(){
  std::string vs =
      "#version 330\n"
      "uniform mat4 mMV;\n"
      "uniform mat3 mV_InvTr;\n"
      "uniform mat4 mMVP;\n"
      "uniform vec3 vLineStart;\n"
      "uniform vec3 vLineEnd;\n"
      "in vec3 vs_Position;\n"
      "out vec3 fs_Position;\n"
      "void main(void)\n"
      "{\n"
      "  vec3 tr_Position = mix(vLineStart, vLineEnd, vs_Position.x);\n"
      "  gl_Position = mMVP * vec4(tr_Position, 1.0);\n"
      "}";

  std::string fs =
      "#version 330\n"
      "uniform vec3 vColor;\n"
      "in vec3 fs_Position;\n"
      "out vec4 Color;\n"
      "void main(void)\n"
      "{\n"
      "  Color = vec4(vColor, 1.0);\n"
      "}\n";

  qpp::shader_program_t *sp = new qpp::shader_program_t(std::string("unit_line_program"), vs, fs);
  sp->u_on(sp_u_name::mModelViewProj);
  sp->u_on(sp_u_name::mModelView);
//  sp->u_on(sp_u_name::vLightPos);
  sp->u_on(sp_u_name::mViewInvTr);
//  sp->u_on(sp_u_name::vTranslate);
//  sp->u_on(sp_u_name::fScale);
  sp->u_on(sp_u_name::vColor);
  sp->u_on(sp_u_name::vLineStart);
  sp->u_on(sp_u_name::vLineEnd);
  return sp;
}

qpp::shader_program_t *qpp::gen_bond_draw_program(){
  std::string vs =
      "#version 330\n"
      "uniform mat4 mMV;\n"
      "uniform mat4 mMV_InvTr;\n"
      "uniform mat4 mMVP;\n"
      "in vec3 vs_Position;\n"
      "in vec3 vs_Normal;\n"
      "out vec3 fs_Normal;\n"
      "out vec3 fs_Position;\n"
      "void main(void)\n"
      "{\n"
      "  vec3 tr_Position = (vs_Position);\n"
      "  fs_Normal = normalize(vec3(mMV_InvTr * vec4(vs_Normal,0.0)));\n"
      "  fs_Position = vec3(mMV * vec4(tr_Position, 1.0));\n"
      "  gl_Position = mMVP * vec4(tr_Position, 1.0);\n"
      "}";

  std::string fs =
      "#version 330\n"
      "uniform vec3 vLightPos;\n"
      "uniform vec3 vColor;\n"
      "in vec3 fs_Normal;\n"
      "in vec3 fs_Position;\n"
      "out vec4 Color;\n"
      "void main(void)\n"
      "{\n"
      "  //float distance = length(vLightPos - fs_Position);\n"
     // "  vec3 lightVector = normalize(vLightPos - fs_Position);\n"
      "  vec3 spLightPos = (vec3(0, 1, 1));\n"
      "  vec3 lightVector = normalize(spLightPos - fs_Position);\n"
      "  float diffuse = max(dot(fs_Normal, lightVector), 0.1);\n"
      "  // diffuse = diffuse * (1.0 / (1.0 + (0.25 * distance * distance)));\n"
      "  vec4 ambient = vec4(0.05, 0.05, 0.05, 1.0);\n"
      "  vec3 gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2);\n"
      "  vec4 linearColor =  vec4(vColor, 1.0) * diffuse;\n"
      "  Color = vec4(pow(linearColor.r, gamma.r),\n"
      "                  pow(linearColor.g, gamma.g),\n"
      "                  pow(linearColor.b, gamma.b), 1.0);\n"
      "}\n";

  qpp::shader_program_t *sp = new qpp::shader_program_t(std::string("bond_program"), vs, fs);
  sp->u_on(sp_u_name::mModelViewProj);
  sp->u_on(sp_u_name::mModelView);
  sp->u_on(sp_u_name::mModelViewInvTr);
  sp->u_on(sp_u_name::vLightPos);
  sp->u_on(sp_u_name::vColor);
 // sp->u_on(sp_u_name::mModelViewNoScale);
  return sp;
}

qpp::shader_program_t *qpp::gen_line_mesh_program(){
  std::string vs =
      "#version 330\n"
      "uniform mat4 mMV;\n"
      "uniform mat3 mV_InvTr;\n"
      "uniform mat4 mMVP;\n"
      "uniform vec3 vTranslate;\n"
      "in vec3 vs_Position;\n"
      "out float fs_Z;\n"
      "void main(void)\n"
      "{\n"
      "  vec4 tr_Position =  mMVP * vec4(vs_Position + vTranslate, 1.0);\n"
      "  fs_Z = tr_Position.z ;\n"
      "  gl_Position = tr_Position;\n"
      "}";

  std::string fs =
      "#version 330\n"
      "uniform vec3 vColor;\n"
      "in float fs_Z;\n"
      "out vec4 Color;\n"
      "void main(void)\n"
      "{\n"
         "float effectAlpha = exp(-0.6 * max(0.0, 76- fs_Z));"
      "  vec4 bgColor = vec4(0.4, 0.4, 0.4, 1.0);\n"
      "  Color = mix(vec4(vColor, 1), bgColor, effectAlpha);\n"
      "}\n";

  qpp::shader_program_t *sp = new qpp::shader_program_t(std::string("grid_program"), vs, fs);
  sp->u_on(sp_u_name::mModelViewProj);
  sp->u_on(sp_u_name::mModelView);
//  sp->u_on(sp_u_name::vLightPos);
  sp->u_on(sp_u_name::mViewInvTr);
  sp->u_on(sp_u_name::vTranslate);
//  sp->u_on(sp_u_name::fScale);
  sp->u_on(sp_u_name::vColor);
  return sp;
}

qpp::shader_program_t *qpp::gen_screen_space_lighting_program(){
  std::string vs =
      "#version 330\n"
      "uniform mat4 mMV;\n"
      "uniform mat4 mMVP;\n"
      "uniform mat4 mMV_InvTr;\n"
      "uniform vec3 vTranslate;\n"
      "uniform float fScale;\n"
      "in vec3 vs_Position;\n"
      "in vec3 vs_Normal;\n"
      "out vec3 fs_Normal;\n"
      "out vec3 fs_Position;\n"
      "void main(void)\n"
      "{\n"
      "  vec3 tr_Position = (vs_Position*fScale + vTranslate);\n"
      "  fs_Normal = vec3(mMV_InvTr * vec4(vs_Normal,0.0));\n"
      "  fs_Position = vec3(mMV * vec4(tr_Position, 1.0));\n"
      "  gl_Position = mMVP * vec4(tr_Position, 1.0);\n"
      "}";

  std::string fs =
      "#version 330\n"
      "uniform vec3 vLightPos;\n"
      "uniform vec3 vColor;\n"
      "in vec3 fs_Normal;\n"
      "in vec3 fs_Position;\n"
      "out vec4 Color;\n"
      "void main(void)\n"
      "{\n"
      "  //float distance = length(vLightPos - fs_Position);\n"
      "  vec3 lightVector = normalize(vLightPos - fs_Position);\n"
      "  float diffuse = max(dot(fs_Normal, lightVector), 0.1);\n"
      "  // diffuse = diffuse * (1.0 / (1.0 + (0.25 * distance * distance)));\n"
      "  vec4 ambient = vec4(0.05, 0.05, 0.05, 1.0);\n"
      "  vec3 gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2);\n"
      "  vec4 linearColor =  vec4(vColor, 1.0) * diffuse;\n"
      "  Color = vec4(pow(linearColor.r, gamma.r),\n"
      "                  pow(linearColor.g, gamma.g),\n"
      "                  pow(linearColor.b, gamma.b), 1.0);\n"
      "}\n";

  qpp::shader_program_t *sp = new qpp::shader_program_t(std::string("default_program"), vs, fs);
  sp->u_on(sp_u_name::mModelViewProj);
  sp->u_on(sp_u_name::mModelView);
  sp->u_on(sp_u_name::mModelViewInvTr);
  sp->u_on(sp_u_name::vLightPos);
  sp->u_on(sp_u_name::vTranslate);
  sp->u_on(sp_u_name::fScale);
  sp->u_on(sp_u_name::vColor);
  return sp;
}
