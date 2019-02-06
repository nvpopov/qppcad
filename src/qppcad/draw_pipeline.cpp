#include <qppcad/draw_pipeline.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

draw_pipeline_t::draw_pipeline_t(){

}

void draw_pipeline_t::load_pipeline(){

}

void draw_pipeline_t::unload_pipeline(){

}

void draw_pipeline_t::render(){

}

void draw_pipeline_t::depth_func(draw_pipeline_depth_func _action) {

  app_state_t* astate = app_state_t::get_inst();

  switch (_action) {
    case draw_pipeline_depth_func::depth_always :
      astate->glapi->glDepthFunc(GL_ALWAYS);
      break;
    case draw_pipeline_depth_func::depth_disabled :
      astate->glapi->glDisable(GL_DEPTH_TEST);
      break;
    case draw_pipeline_depth_func::depth_enabled :
      astate->glapi->glEnable(GL_DEPTH_TEST);
      break;
    case draw_pipeline_depth_func::depth_equal :
      astate->glapi->glDepthFunc(GL_EQUAL);
      break;
    case draw_pipeline_depth_func::depth_lequal:
      astate->glapi->glDepthFunc(GL_LEQUAL);
      break;
    case draw_pipeline_depth_func::depth_less :
      astate->glapi->glDepthFunc(GL_LESS);
      break;
    }

}

void draw_pipeline_t::cull_func(draw_pipeline_cull_func _action) {

  app_state_t* astate = app_state_t::get_inst();

  switch (_action) {
    case draw_pipeline_cull_func::cull_front :
      astate->glapi->glCullFace(GL_FRONT);
      break;
    case draw_pipeline_cull_func::cull_back :
      astate->glapi->glCullFace(GL_BACK);
      break;
    case draw_pipeline_cull_func::cull_enable :
      astate->glapi->glEnable(GL_CULL_FACE);
      break;
    case draw_pipeline_cull_func::cull_disable :
      astate->glapi->glDisable(GL_CULL_FACE);
      break;
    }

}

void draw_pipeline_t::begin_atom_render (float specular_power, float specular_alpha) {

  app_state_t* astate = app_state_t::get_inst();

  astate->sp_default->begin_shader_program();
  astate->sp_default->set_u(sp_u_name::f_specular_intensity, &specular_power);
  astate->sp_default->set_u(sp_u_name::f_specular_alpha, &specular_alpha);
  astate->mesh_spheres[0]->begin_render_batch();
}

void draw_pipeline_t::render_atom (const vector3<float> &color,
                                   const vector3<float> &pos,
                                   const float radius) {

  app_state_t* astate = app_state_t::get_inst();

  // std::cout<<"render_atom"<<std::endl;

  astate->sp_default->set_u(sp_u_name::v_translate, (GLfloat*)(pos.data()));
  astate->sp_default->set_u(sp_u_name::f_scale, (GLfloat*)(&radius));
  astate->sp_default->set_u(sp_u_name::v_color, (GLfloat*)(color.data()));

  matrix4<float> mat_model_view_inv_tr =
      (astate->camera->m_mat_view*matrix4<float>::Identity()).inverse().transpose();
  // our Model matrix equals unity matrix, so just pass matrix from app state
  astate->sp_default->set_u(sp_u_name::m_model_view_proj, astate->camera->m_proj_view.data());
  astate->sp_default->set_u(sp_u_name::m_model_view, astate->camera->m_mat_view.data());
  astate->sp_default->set_u(sp_u_name::m_model_view_inv_tr, mat_model_view_inv_tr.data());

  astate->mesh_spheres[0]->render_batch();
  // astate->trm->render();

}

void draw_pipeline_t::end_atom_render () {
  app_state_t* astate = app_state_t::get_inst();
  astate->sp_default->end_shader_program();
  astate->mesh_spheres[0]->end_render_batch();
}

void draw_pipeline_t::begin_render_bond (float specular_power, float specular_alpha) {
  app_state_t* astate = app_state_t::get_inst();
  astate->sp_mvp_ssl->begin_shader_program();
  astate->sp_mvp_ssl->set_u(sp_u_name::f_specular_intensity, &specular_power);
  astate->sp_mvp_ssl->set_u(sp_u_name::f_specular_alpha, &specular_alpha);
}

void draw_pipeline_t::render_bond (const vector3<float> &color,
                                   const vector3<float> &bond_start,
                                   const vector3<float> &bond_end,
                                   const float bond_radius) {
  app_state_t* astate = app_state_t::get_inst();

  vector3<float> bond_end_new = (bond_end - bond_start)*(-0.498f) + bond_end;

  matrix4<float> mat_model = matrix4<float>::Identity();
  mat_model.block<3,1>(0,3) = bond_start;
  mat_model.block<3,1>(0,2) = bond_end_new - bond_start;

  vector3<float> vec_axis_norm = mat_model.block<3,1>(0,2).normalized();
  mat_model.block<3,1>(0,0) = vec_axis_norm.unitOrthogonal() * bond_radius;
  mat_model.block<3,1>(0,1) = vec_axis_norm.cross(mat_model.block<3,1>(0,0));
  mat_model.block<3,1>(0,3) = bond_start;

  matrix4<float> mat_model_view_inv_tr = (astate->camera->m_mat_view *
                                          mat_model).inverse().transpose();

  matrix4<float> mat_model_view = astate->camera->m_mat_view * mat_model;
  matrix4<float> mat_model_view_proj = astate->camera->m_proj_view * mat_model;

  astate->sp_mvp_ssl->set_u(sp_u_name::m_model_view_proj, mat_model_view_proj.data());
  astate->sp_mvp_ssl->set_u(sp_u_name::m_model_view, mat_model_view.data());
  astate->sp_mvp_ssl->set_u(sp_u_name::m_view_proj, astate->camera->m_proj_view.data());
  astate->sp_mvp_ssl->set_u(sp_u_name::v_color, (GLfloat*)(color.data()));
  astate->sp_mvp_ssl->set_u(sp_u_name::m_model_view_inv_tr, mat_model_view_inv_tr.data());

  astate->mesh_cylinder->render();

}

void draw_pipeline_t::end_render_bond () {
  app_state_t* astate = app_state_t::get_inst();
  astate->sp_mvp_ssl->end_shader_program();
}

void draw_pipeline_t::begin_render_2c_bond(float specular_power, float specular_alpha) {

  app_state_t* astate = app_state_t::get_inst();
  astate->sp_2c_cylinder->begin_shader_program();
  astate->sp_2c_cylinder->set_u(sp_u_name::f_specular_intensity, &specular_power);
  astate->sp_2c_cylinder->set_u(sp_u_name::f_specular_alpha, &specular_alpha);

}

void draw_pipeline_t::render_2c_bond(const vector3<float> &color1,
                                     const vector3<float> &color2,
                                     const vector3<float> &bond_start,
                                     const vector3<float> &bond_end,
                                     const float bond_radius) {
  app_state_t* astate = app_state_t::get_inst();

  vector3<float> bond_end_new =  bond_end;

  matrix4<float> mat_model = matrix4<float>::Identity();
  mat_model.block<3,1>(0,3) = bond_start;
  mat_model.block<3,1>(0,2) = bond_end_new - bond_start;

  vector3<float> vec_axis_norm = mat_model.block<3,1>(0,2).normalized();
  mat_model.block<3,1>(0,0) = vec_axis_norm.unitOrthogonal() * bond_radius;
  mat_model.block<3,1>(0,1) = vec_axis_norm.cross(mat_model.block<3,1>(0,0));
  mat_model.block<3,1>(0,3) = bond_start;

  matrix4<float> mat_model_view_inv_tr = (astate->camera->m_mat_view *
                                          mat_model).inverse().transpose();

  matrix4<float> mat_model_view = astate->camera->m_mat_view * mat_model;
  matrix4<float> mat_model_view_proj = astate->camera->m_proj_view * mat_model;

  astate->sp_2c_cylinder->set_u(sp_u_name::m_model_view_proj, mat_model_view_proj.data());
  astate->sp_2c_cylinder->set_u(sp_u_name::m_model_view, mat_model_view.data());
  astate->sp_2c_cylinder->set_u(sp_u_name::m_view_proj, astate->camera->m_proj_view.data());
  astate->sp_2c_cylinder->set_u(sp_u_name::v_color1, (GLfloat*)(color1.data()));
  astate->sp_2c_cylinder->set_u(sp_u_name::v_color2, (GLfloat*)(color2.data()));
  astate->sp_2c_cylinder->set_u(sp_u_name::m_model_view_inv_tr, mat_model_view_inv_tr.data());

  astate->mesh_cylinder->render();

}

void draw_pipeline_t::end_render_2c_bond() {
  app_state_t* astate = app_state_t::get_inst();
  astate->sp_2c_cylinder->end_shader_program();
}

void draw_pipeline_t::render_molecule () {

}

void draw_pipeline_t::render_cell_3d (const vector3<float> &color,
                                      const vector3<float> &a,
                                      const vector3<float> &b,
                                      const vector3<float> &c,
                                      const vector3<float> &shift) {
  static int disp[][6] = {
    {0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 1, 0},
    {0, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 1, 1},
    {0, 0, 1, 1, 0, 1},
    {0, 1, 0, 0, 1, 1},
    {0, 1, 0, 1, 1, 0},
    {0, 1, 1, 1, 1, 1},
    {1, 0, 0, 1, 0, 1},
    {1, 0, 0, 1, 1, 0},
    {1, 0, 1, 1, 1, 1},
    {1, 1, 0, 1, 1, 1}
  };

  for (unsigned int i = 0; i < 12; i++){
      vector3<float> vec_line_start = a*disp[i][0] + b*disp[i][1] + c*disp[i][2];
      vector3<float> vec_line_end   = a*disp[i][3] + b*disp[i][4] + c*disp[i][5];

      render_line(color, vec_line_start + shift, vec_line_end + shift);

    }
}

void draw_pipeline_t::render_vector () {

}

void draw_pipeline_t::render_primitive () {

}


void draw_pipeline_t::begin_render_general_mesh (shader_program_t *custom_sp) {

  if (custom_sp) {
      custom_sp->begin_shader_program();
    } else {
      app_state_t* astate = app_state_t::get_inst();
      astate->sp_mvp_ssl->begin_shader_program();
      //float specular_power = 12.0f;
      //float specular_alpha = 0.0f;
    }
}

void draw_pipeline_t::render_general_mesh (const vector3<float> &mesh_pos,
                                           const vector3<float> &mesh_scale,
                                           const vector3<float> &mesh_rotation,
                                           const vector3<float> &mesh_color,
                                           mesh_t *mesh,
                                           float alpha,
                                           shader_program_t *custom_sp) {

  Eigen::Transform<float, 3, Eigen::Affine>
      t = Eigen::Transform<float, 3, Eigen::Affine>::Identity();


  Eigen::AngleAxisf roll_angle (mesh_rotation[0], vector3<float>::UnitZ());
  Eigen::AngleAxisf yaw_angle  (mesh_rotation[1], vector3<float>::UnitY());
  Eigen::AngleAxisf pitch_angle(mesh_rotation[2], vector3<float>::UnitX());

  Eigen::Quaternion<float> q = roll_angle * yaw_angle * pitch_angle;

  Eigen::Matrix3f rot_m = q.matrix();

  t.prescale(mesh_scale);
  t.prerotate(rot_m);
  t.pretranslate(mesh_pos);

  matrix4<float> mat_model = t.matrix()*matrix4<float>::Identity();
  render_general_mesh(mat_model, mesh_color, mesh, alpha, custom_sp);
}



void draw_pipeline_t::render_general_mesh (const matrix4<float> &model_matrix,
                                           const vector3<float> &mesh_color,
                                           mesh_t *mesh,
                                           float alpha,
                                           shader_program_t *custom_sp) {

  app_state_t* astate = app_state_t::get_inst();
  // glapi_t* glapi = astate->glapi;

  matrix4<float> mat_model_view      = astate->camera->m_mat_view * model_matrix;
  matrix4<float> mat_model_view_proj = astate->camera->m_proj_view * model_matrix;
  matrix4<float> mat_model_view_inv_tr = (mat_model_view).inverse().transpose();

  shader_program_t *_wrp{nullptr};
  if (custom_sp) _wrp = custom_sp;
  else custom_sp = astate->sp_mvp_ssl;

  custom_sp->set_u(sp_u_name::m_model_view_proj, mat_model_view_proj.data());
  custom_sp->set_u(sp_u_name::m_model_view, mat_model_view.data());
  custom_sp->set_u(sp_u_name::m_model_view_inv_tr, mat_model_view_inv_tr.data());
  custom_sp->set_u(sp_u_name::v_color, (GLfloat*)(mesh_color.data()));

  if (custom_sp->unf_rec[sp_u_name::f_color_alpha].h_prog != -1) {
      custom_sp->set_u(sp_u_name::f_color_alpha, &alpha);
      astate->glapi->glEnable(GL_BLEND);
      //astate->glapi->glCullFace(GL_FRONT_AND_BACK);
      astate->glapi->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

  mesh->render();
}

void draw_pipeline_t::render_cube (const vector3<float> &cube_pos,
                                   const vector3<float> &cube_size,
                                   const vector3<float> &cube_color) {
  app_state_t* astate = app_state_t::get_inst();


  Eigen::Transform<float, 3, Eigen::Affine>
      t = Eigen::Transform<float, 3, Eigen::Affine>::Identity();
  t.prescale(cube_size);
  t.pretranslate(cube_pos);


  // cube has a = 2 (-1 .. 1), so scale it
  matrix4<float> mat_model = t.matrix()*matrix4<float>::Identity();

  matrix4<float> mat_model_view      = astate->camera->m_mat_view * mat_model;
  matrix4<float> mat_model_view_proj = astate->camera->m_proj_view * mat_model;
  matrix4<float> mat_model_view_inv_tr = (mat_model_view).inverse().transpose();

  astate->sp_mvp_ssl->set_u(sp_u_name::m_model_view_proj, mat_model_view_proj.data());
  astate->sp_mvp_ssl->set_u(sp_u_name::m_model_view, mat_model_view.data());
  astate->sp_mvp_ssl->set_u(sp_u_name::m_model_view_inv_tr, mat_model_view_inv_tr.data());
  astate->sp_mvp_ssl->set_u(sp_u_name::v_color, (GLfloat*)(cube_color.data()));
  //glDisable(GL_CULL_FACE);
  astate->mesh_unit_cube->render();
  //glEnable(GL_CULL_FACE);
}

void draw_pipeline_t::render_cone (const vector3<float> &cone_pos,
                                   const vector3<float> &cone_size,
                                   const vector3<float> &cone_color) {
  app_state_t* astate = app_state_t::get_inst();


  Eigen::Transform<float, 3, Eigen::Affine>
      t = Eigen::Transform<float, 3, Eigen::Affine>::Identity();
  t.prescale(cone_size);
  t.pretranslate(cone_pos);
  matrix4<float> mat_model = t.matrix()*matrix4<float>::Identity();

  matrix4<float> mat_model_view      = astate->camera->m_mat_view * mat_model;
  matrix4<float> mat_model_view_proj = astate->camera->m_proj_view * mat_model;
  matrix4<float> mat_model_view_inv_tr = (mat_model_view).inverse().transpose();

  astate->sp_mvp_ssl->set_u(sp_u_name::m_model_view_proj, mat_model_view_proj.data());
  astate->sp_mvp_ssl->set_u(sp_u_name::m_model_view, mat_model_view.data());
  astate->sp_mvp_ssl->set_u(sp_u_name::m_model_view_inv_tr, mat_model_view_inv_tr.data());
  astate->sp_mvp_ssl->set_u(sp_u_name::v_color, (GLfloat*)(cone_color.data()));
  //glDisable(GL_CULL_FACE);
  astate->mesh_unit_cone->render();
  //glEnable(GL_CULL_FACE);
}

void draw_pipeline_t::end_render_general_mesh (shader_program_t *custom_sp) {

  app_state_t* astate = app_state_t::get_inst();

  shader_program_t *_wrp{nullptr};
  if (custom_sp) _wrp = custom_sp;
  else custom_sp = astate->sp_mvp_ssl;

  if (custom_sp->unf_rec[sp_u_name::f_color_alpha].h_prog != -1) {
      astate->glapi->glDisable(GL_BLEND);
      // astate->glapi->glCullFace(GL_BACK);
      astate->glapi->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
  custom_sp->end_shader_program();

}

void draw_pipeline_t::begin_render_aabb () {
  begin_render_line();
}

void draw_pipeline_t::render_aabb (const vector3<float> &color,
                                   const vector3<float> &box_min,
                                   const vector3<float> &box_max) {

  vector3<float> half_box_size = (box_max - box_min) * 0.5f;
  vector3<float> box_center   = (box_min + box_max) * 0.5f;

  static int disp[][6] = {
    {-1,-1,-1,  1,-1,-1},
    {-1,-1,-1, -1, 1,-1},
    {-1,-1,-1, -1,-1, 1},

    { 1, 1, 1, -1, 1, 1},
    { 1, 1, 1,  1,-1, 1},
    { 1, 1, 1,  1, 1,-1},

    {-1,-1, 1, -1, 1, 1},
    {-1,-1, 1,  1,-1, 1},

    { 1,-1, 1,  1,-1,-1},
    {-1, 1,-1, -1, 1, 1},

    { 1, 1,-1,  1,-1,-1},
    { 1, 1,-1, -1, 1,-1},
  };

  for (uint8_t i = 0; i < 12; i++){
      vector3<float> line_start(
            half_box_size[0]*disp[i][0],
          half_box_size[1]*disp[i][1],
          half_box_size[2]*disp[i][2]);

      vector3<float> line_end(
            half_box_size[0]*disp[i][3],
          half_box_size[1]*disp[i][4],
          half_box_size[2]*disp[i][5]);

      render_line(color, box_center+line_start,box_center+line_end );

    }

}

void draw_pipeline_t::render_aabb_segmented (const vector3<float> &color,
                                             const vector3<float> &box_min,
                                             const vector3<float> &box_max) {

  vector3<float> half_box_size = (box_max - box_min) * 0.5f;
  vector3<float> box_center    = (box_max + box_min) * 0.5f;

  /*
  sign = lambda x: (1, -1)[x<0]
  dt = 0.4
  for x in [-1,1]:
      for y in [-1,1]:
          for z in [-1,1]:
                  print("{{{:6.6}f,{:6.6}f,{:6.6}f,{:6.6}f,{:6.6}f,{:6.6}f}},".format(
                      float(x), float(y), float(z), x-sign(x)*dt, float(y), float(z)))
                  print("{{{:6.6}f,{:6.6}f,{:6.6}f,{:6.6}f,{:6.6}f,{:6.6}f}},".format(
                      float(x), float(y), float(z),  float(x),y-sign(y)*dt, float(z)))
                  print("{{{:6.6}f,{:6.6}f,{:6.6}f,{:6.6}f,{:6.6}f,{:6.6}f}},".format(
                      float(x), float(y), float(z),  float(x), float(y), z-sign(z)*dt))*/


  static float disp2[][6] = {
    {  -1.0f,  -1.0f,  -1.0f,  -0.6f,  -1.0f,  -1.0f},
    {  -1.0f,  -1.0f,  -1.0f,  -1.0f,  -0.6f,  -1.0f},
    {  -1.0f,  -1.0f,  -1.0f,  -1.0f,  -1.0f,  -0.6f},
    {  -1.0f,  -1.0f,   1.0f,  -0.6f,  -1.0f,   1.0f},
    {  -1.0f,  -1.0f,   1.0f,  -1.0f,  -0.6f,   1.0f},
    {  -1.0f,  -1.0f,   1.0f,  -1.0f,  -1.0f,   0.6f},
    {  -1.0f,   1.0f,  -1.0f,  -0.6f,   1.0f,  -1.0f},
    {  -1.0f,   1.0f,  -1.0f,  -1.0f,   0.6f,  -1.0f},
    {  -1.0f,   1.0f,  -1.0f,  -1.0f,   1.0f,  -0.6f},
    {  -1.0f,   1.0f,   1.0f,  -0.6f,   1.0f,   1.0f},
    {  -1.0f,   1.0f,   1.0f,  -1.0f,   0.6f,   1.0f},
    {  -1.0f,   1.0f,   1.0f,  -1.0f,   1.0f,   0.6f},
    {   1.0f,  -1.0f,  -1.0f,   0.6f,  -1.0f,  -1.0f},
    {   1.0f,  -1.0f,  -1.0f,   1.0f,  -0.6f,  -1.0f},
    {   1.0f,  -1.0f,  -1.0f,   1.0f,  -1.0f,  -0.6f},
    {   1.0f,  -1.0f,   1.0f,   0.6f,  -1.0f,   1.0f},
    {   1.0f,  -1.0f,   1.0f,   1.0f,  -0.6f,   1.0f},
    {   1.0f,  -1.0f,   1.0f,   1.0f,  -1.0f,   0.6f},
    {   1.0f,   1.0f,  -1.0f,   0.6f,   1.0f,  -1.0f},
    {   1.0f,   1.0f,  -1.0f,   1.0f,   0.6f,  -1.0f},
    {   1.0f,   1.0f,  -1.0f,   1.0f,   1.0f,  -0.6f},
    {   1.0f,   1.0f,   1.0f,   0.6f,   1.0f,   1.0f},
    {   1.0f,   1.0f,   1.0f,   1.0f,   0.6f,   1.0f},
    {   1.0f,   1.0f,   1.0f,   1.0f,   1.0f,   0.6f},
  };

  for (const auto &v : disp2){
      vector3<float> line_start(
            half_box_size[0] * v[0],
          half_box_size[1] * v[1],
          half_box_size[2] * v[2]);

      vector3<float> line_end(
            half_box_size[0] * v[3],
          half_box_size[1] * v[4],
          half_box_size[2] * v[5]);

      render_line(color, box_center + line_start, box_center + line_end);
    }

}

void draw_pipeline_t::end_render_aabb(){
  end_render_line();
}

void draw_pipeline_t::begin_render_line () {

  app_state_t* astate = app_state_t::get_inst();
  astate->sp_unit_line->begin_shader_program();
  astate->sp_unit_line->set_u(sp_u_name::m_model_view_proj,
                              astate->camera->m_proj_view.data());
  astate->sp_unit_line->set_u(sp_u_name::m_model_view, astate->camera->m_mat_view.data());
  astate->mesh_unit_line->begin_render_batch();
}

void draw_pipeline_t::render_line(const vector3<float> &color,
                                  const vector3<float> &line_start,
                                  const vector3<float> &line_end){

  app_state_t* astate = app_state_t::get_inst();
  glapi_t* glapi = astate->glapi;
  //glapi->glLineWidth(line_width);
  astate->sp_unit_line->set_u(sp_u_name::v_color, (GLfloat*)color.data());
  astate->sp_unit_line->set_u(sp_u_name::v_line_start, (GLfloat*)line_start.data());
  astate->sp_unit_line->set_u(sp_u_name::v_line_end, (GLfloat*)line_end.data());

  astate->mesh_unit_line->render_batch();

}

void draw_pipeline_t::end_render_line () {

  app_state_t* astate = app_state_t::get_inst();
  glapi_t* glapi = astate->glapi;
  // glapi->glLineWidth(1.0f);
  astate->sp_unit_line->end_shader_program();
  astate->mesh_unit_line->end_render_batch();

}

void draw_pipeline_t::begin_render_line_styled () {

  app_state_t* astate = app_state_t::get_inst();
  astate->sp_unit_line_styled->begin_shader_program();
  astate->sp_unit_line_styled->set_u(sp_u_name::m_model_view_proj,
                                     astate->camera->m_proj_view.data());
  astate->sp_unit_line_styled->set_u(sp_u_name::m_model_view,
                                     astate->camera->m_mat_view.data());
  astate->mesh_unit_line->begin_render_batch();

}

void draw_pipeline_t::render_line_styled (const vector3<float> &color,
                                          const vector3<float> &line_start,
                                          const vector3<float> &line_end) {

  app_state_t* astate = app_state_t::get_inst();

  //glLineWidth(line_width);
  astate->sp_unit_line_styled->set_u(sp_u_name::v_color, (GLfloat*)color.data());
  astate->sp_unit_line_styled->set_u(sp_u_name::v_line_start, (GLfloat*)line_start.data());
  astate->sp_unit_line_styled->set_u(sp_u_name::v_line_end, (GLfloat*)line_end.data());

  astate->mesh_unit_line->render_batch();

}

void draw_pipeline_t::end_render_line_styled () {

  //glLineWidth(1.0f);
  app_state_t* astate = app_state_t::get_inst();
  astate->sp_unit_line_styled->end_shader_program();
  astate->mesh_unit_line->end_render_batch();

}

void draw_pipeline_t::render_screen_quad () {

  app_state_t* astate = app_state_t::get_inst();
  astate->sp_fbo_quad->begin_shader_program();
  glDisable(GL_DEPTH_TEST);

  glActiveTexture(GL_TEXTURE0);
  //glBindTexture(GL_TEXTURE_2D, astate->frame_buffer->get_color_texture());

  astate->mesh_fbo_quad->render();
  glEnable(GL_DEPTH_TEST);
  astate->sp_fbo_quad->end_shader_program();

}
