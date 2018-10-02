#include <qppcad/draw_pipeline.hpp>
#include <qppcad/app.hpp>

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

void draw_pipeline_t::begin_atom_render (float specular_power, float specular_alpha) {

  app_state_t* astate = &(c_app::get_state());
  astate->default_program->begin_shader_program();
  astate->default_program->set_u(sp_u_name::f_specular_intensity, &specular_power);
  astate->default_program->set_u(sp_u_name::f_specular_alpha, &specular_alpha);
  astate->_sph_meshes[0]->begin_render_batch();
}

void draw_pipeline_t::render_atom (const vector3<float> &color,
                                   const vector3<float> &pos,
                                   const float radius) {

  app_state_t* astate = &(c_app::get_state());
  // std::cout<<"render_atom"<<std::endl;

  astate->default_program->set_u(sp_u_name::v_translate, (GLfloat*)(pos.data()));
  astate->default_program->set_u(sp_u_name::f_scale, (GLfloat*)(&radius));
  astate->default_program->set_u(sp_u_name::v_color, (GLfloat*)(color.data()));

  matrix4<float> mat_model_view_inv_tr =
      (astate->camera->m_mat_view*matrix4<float>::Identity()).inverse().transpose();
  // our Model matrix equals unity matrix, so just pass matrix from app state
  astate->default_program->set_u(sp_u_name::m_model_view_proj, astate->camera->m_view_proj.data());
  astate->default_program->set_u(sp_u_name::m_model_view, astate->camera->m_mat_view.data());
  astate->default_program->set_u(sp_u_name::m_model_view_inv_tr, mat_model_view_inv_tr.data());

  astate->_sph_meshes[0]->render_batch();
  // astate->trm->render();

}

void draw_pipeline_t::end_atom_render () {
  app_state_t* astate = &(c_app::get_state());
  astate->default_program->end_shader_program();
  astate->_sph_meshes[0]->end_render_batch();
}

void draw_pipeline_t::begin_render_bond (float specular_power, float specular_alpha) {
  app_state_t* astate = &(c_app::get_state());
  astate->mvp_ssl_program->begin_shader_program();
  astate->mvp_ssl_program->set_u(sp_u_name::f_specular_intensity, &specular_power);
  astate->mvp_ssl_program->set_u(sp_u_name::f_specular_alpha, &specular_alpha);
}

void draw_pipeline_t::render_bond (const vector3<float> &color,
                                   const vector3<float> &bond_start,
                                   const vector3<float> &bond_end,
                                   const float bond_radius) {
  app_state_t* astate = &(c_app::get_state());

  vector3<float> bond_end_new = (bond_end - bond_start)*(-0.498f) + bond_end;

  matrix4<float> mat_model = matrix4<float>::Identity();
  mat_model.block<3,1>(0,3) = bond_start;
  mat_model.block<3,1>(0,2) = bond_end_new - bond_start;

  vector3<float> vec_axis_norm = mat_model.block<3,1>(0,2).normalized();
  mat_model.block<3,1>(0,0) = vec_axis_norm.unitOrthogonal() * bond_radius;
  mat_model.block<3,1>(0,1) = vec_axis_norm.cross(mat_model.block<3,1>(0,0));
  mat_model.block<3,1>(0,3) = bond_start;

  matrix4<float> mat_model_view_inv_tr = (astate->camera->m_mat_view * mat_model).inverse().transpose();

  matrix4<float> mat_model_view = astate->camera->m_mat_view * mat_model;
  matrix4<float> mat_model_view_proj = astate->camera->m_view_proj * mat_model;

  astate->mvp_ssl_program->set_u(sp_u_name::m_model_view_proj, mat_model_view_proj.data());
  astate->mvp_ssl_program->set_u(sp_u_name::m_model_view, mat_model_view.data());
  astate->mvp_ssl_program->set_u(sp_u_name::m_view_proj, astate->camera->m_view_proj.data());
  astate->mvp_ssl_program->set_u(sp_u_name::v_color, (GLfloat*)(color.data()));
  astate->mvp_ssl_program->set_u(sp_u_name::m_model_view_inv_tr, mat_model_view_inv_tr.data());

  astate->cylinder_mesh->render();

}

void draw_pipeline_t::end_render_bond () {
  app_state_t* astate = &(c_app::get_state());
  astate->mvp_ssl_program->end_shader_program();
}

void draw_pipeline_t::render_molecule () {

}

void draw_pipeline_t::render_cell_3d (const vector3<float> &color,
                                      const vector3<float> &a,
                                      const vector3<float> &b,
                                      const vector3<float> &c,
                                      const vector3<float> &shift,
                                      const float line_width = 1.0f) {
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

      render_line(color, vec_line_start + shift, vec_line_end + shift, line_width);

    }
}

void draw_pipeline_t::render_vector () {

}

void draw_pipeline_t::render_primitive () {

}

void draw_pipeline_t::begin_render_general_mesh () {
  app_state_t* astate = &(c_app::get_state());
  astate->mvp_ssl_program->begin_shader_program();
}

void draw_pipeline_t::render_general_mesh (const vector3<float> &mesh_pos,
                                           const vector3<float> &mesh_scale,
                                           const vector3<float> &mesh_rotation,
                                           const vector3<float> &mesh_color,
                                           mesh_t *mesh) {

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
  render_general_mesh(mat_model, mesh_color, mesh);
}



void draw_pipeline_t::render_general_mesh (const matrix4<float> &model_matrix,
                                           const vector3<float> &mesh_color,
                                           mesh_t *mesh) {
  app_state_t* astate = &(c_app::get_state());

  matrix4<float> mat_model_view      = astate->camera->m_mat_view * model_matrix;
  matrix4<float> mat_model_view_proj = astate->camera->m_view_proj * model_matrix;
  matrix4<float> mat_model_view_inv_tr = (mat_model_view).inverse().transpose();

  astate->mvp_ssl_program->set_u(sp_u_name::m_model_view_proj, mat_model_view_proj.data());
  astate->mvp_ssl_program->set_u(sp_u_name::m_model_view, mat_model_view.data());
  astate->mvp_ssl_program->set_u(sp_u_name::m_model_view_inv_tr, mat_model_view_inv_tr.data());
  astate->mvp_ssl_program->set_u(sp_u_name::v_color, (GLfloat*)(mesh_color.data()));
  //glDisable(GL_CULL_FACE);
  mesh->render();
}

void draw_pipeline_t::render_cube (const vector3<float> &cube_pos,
                                   const vector3<float> &cube_size,
                                   const vector3<float> &cube_color) {
  app_state_t* astate = &(c_app::get_state());


  Eigen::Transform<float, 3, Eigen::Affine>
      t = Eigen::Transform<float, 3, Eigen::Affine>::Identity();
  t.prescale(cube_size);
  t.pretranslate(cube_pos);


  // cube has a = 2 (-1 .. 1), so scale it
  matrix4<float> mat_model = t.matrix()*matrix4<float>::Identity();

  matrix4<float> mat_model_view      = astate->camera->m_mat_view * mat_model;
  matrix4<float> mat_model_view_proj = astate->camera->m_view_proj * mat_model;
  matrix4<float> mat_model_view_inv_tr = (mat_model_view).inverse().transpose();

  astate->mvp_ssl_program->set_u(sp_u_name::m_model_view_proj, mat_model_view_proj.data());
  astate->mvp_ssl_program->set_u(sp_u_name::m_model_view, mat_model_view.data());
  astate->mvp_ssl_program->set_u(sp_u_name::m_model_view_inv_tr, mat_model_view_inv_tr.data());
  astate->mvp_ssl_program->set_u(sp_u_name::v_color, (GLfloat*)(cube_color.data()));
  //glDisable(GL_CULL_FACE);
  astate->unit_cube->render();
  //glEnable(GL_CULL_FACE);
}

void draw_pipeline_t::render_cone (const vector3<float> &cone_pos,
                                   const vector3<float> &cone_size,
                                   const vector3<float> &cone_color) {
  app_state_t* astate = &(c_app::get_state());


  Eigen::Transform<float, 3, Eigen::Affine>
      t = Eigen::Transform<float, 3, Eigen::Affine>::Identity();
  t.prescale(cone_size);
  t.pretranslate(cone_pos);
  matrix4<float> mat_model = t.matrix()*matrix4<float>::Identity();

  matrix4<float> mat_model_view      = astate->camera->m_mat_view * mat_model;
  matrix4<float> mat_model_view_proj = astate->camera->m_view_proj * mat_model;
  matrix4<float> mat_model_view_inv_tr = (mat_model_view).inverse().transpose();

  astate->mvp_ssl_program->set_u(sp_u_name::m_model_view_proj, mat_model_view_proj.data());
  astate->mvp_ssl_program->set_u(sp_u_name::m_model_view, mat_model_view.data());
  astate->mvp_ssl_program->set_u(sp_u_name::m_model_view_inv_tr, mat_model_view_inv_tr.data());
  astate->mvp_ssl_program->set_u(sp_u_name::v_color, (GLfloat*)(cone_color.data()));
  //glDisable(GL_CULL_FACE);
  astate->unit_cone->render();
  //glEnable(GL_CULL_FACE);
}

void draw_pipeline_t::end_render_general_mesh () {
  app_state_t* astate = &(c_app::get_state());
  astate->mvp_ssl_program->end_shader_program();
}

void draw_pipeline_t::begin_render_aabb(){
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

void draw_pipeline_t::render_aabb_segmented(const vector3<float> &color,
                                            const vector3<float> &box_min,
                                            const vector3<float> &box_max){

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

      render_line(color, box_center + line_start, box_center + line_end, 4.0f);
    }
}

void draw_pipeline_t::end_render_aabb(){
  end_render_line();
}

void draw_pipeline_t::begin_render_line(){
  app_state_t* astate = &(c_app::get_state());
  astate->unit_line_program->begin_shader_program();
}

void draw_pipeline_t::render_line(const vector3<float> &color,
                                  const vector3<float> &line_start,
                                  const vector3<float> &line_end,
                                  const float line_width){
  app_state_t* astate = &(c_app::get_state());

  glLineWidth(line_width);
  astate->unit_line_program->set_u(sp_u_name::v_color, (GLfloat*)color.data());
  astate->unit_line_program->set_u(sp_u_name::v_line_start, (GLfloat*)line_start.data());
  astate->unit_line_program->set_u(sp_u_name::v_line_end, (GLfloat*)line_end.data());
  astate->unit_line_program->set_u(sp_u_name::m_model_view_proj, astate->camera->m_view_proj.data());
  astate->unit_line_program->set_u(sp_u_name::m_model_view, astate->camera->m_mat_view.data());
  astate->unit_line->render();
}

void draw_pipeline_t::end_render_line(){
  glLineWidth(1.0f);
  app_state_t* astate = &(c_app::get_state());
  astate->unit_line_program->end_shader_program();
}

void draw_pipeline_t::render_screen_quad(){
  app_state_t* astate = &(c_app::get_state());
  astate->fbo_quad_program->begin_shader_program();
  glDisable(GL_DEPTH_TEST);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, astate->frame_buffer->get_color_texture());

  astate->fbo_quad->render();
  glEnable(GL_DEPTH_TEST);
  astate->fbo_quad_program->end_shader_program();
}
