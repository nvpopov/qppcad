#include <qppcad/draw_pipeline.hpp>
#include <qppcad/app.hpp>

using namespace qpp;

draw_pipeline::draw_pipeline(){

}

void draw_pipeline::load_pipeline(){

}

void draw_pipeline::unload_pipeline(){

}

void draw_pipeline::render(){

}

void draw_pipeline::begin_atom_render(){
  app_state* astate = &(c_app::get_state());
  astate->def_shader->begin_shader_program();
}

void draw_pipeline::render_atom(const vector3<float> &color,
                                const vector3<float> &pos,
                                const float radius){

  app_state* astate = &(c_app::get_state());
  // std::cout<<"render_atom"<<std::endl;

  astate->def_shader->set_u(sp_u_name::vTranslate, (GLfloat*)(pos.data()));
  astate->def_shader->set_u(sp_u_name::fScale, (GLfloat*)(&radius));
  astate->def_shader->set_u(sp_u_name::vColor, (GLfloat*)(color.data()));

  matrix4<float> mModelViewInvTr =
      (astate->_camera->mView*matrix4<float>::Identity()).inverse().transpose();
  // our Model matrix equals unity matrix, so just pass matrix from app state
  astate->def_shader->set_u(sp_u_name::mModelViewProj,
                            astate->_camera->mViewProjection.data());
  astate->def_shader->set_u(sp_u_name::mModelView, astate->_camera->mView.data());
  astate->def_shader->set_u(sp_u_name::mModelViewInvTr,
                            mModelViewInvTr.data());

  astate->_sph_meshes[0]->render();
  // astate->trm->render();

}

void draw_pipeline::end_atom_render(){
  app_state* astate = &(c_app::get_state());
  astate->def_shader->end_shader_program();
}

void draw_pipeline::begin_render_bond(){
  app_state* astate = &(c_app::get_state());
  astate->bond_shader->begin_shader_program();
}

void draw_pipeline::render_bond(const vector3<float> &color,
                                const vector3<float> &vBondStart,
                                const vector3<float> &vBondEnd,
                                const float fBondRadius){
  app_state* astate = &(c_app::get_state());

  vector3<float> vBondEndNew = (vBondEnd - vBondStart)*(-0.498f) + vBondEnd;

  matrix4<float> mModelTr = matrix4<float>::Identity();
  mModelTr.block<3,1>(0,3) = vBondStart;
  mModelTr.block<3,1>(0,2) = vBondEndNew - vBondStart;

  vector3<float> vAxisNorm = mModelTr.block<3,1>(0,2).normalized();
  mModelTr.block<3,1>(0,0) = vAxisNorm.unitOrthogonal() * fBondRadius;
  mModelTr.block<3,1>(0,1) = vAxisNorm.cross(mModelTr.block<3,1>(0,0));
  mModelTr.block<3,1>(0,3) = vBondStart;

  matrix4<float> mModel = mModelTr  /** rotM * mModelSc */;
  matrix4<float> mModelViewInvTr =
      (astate->_camera->mView * mModelTr).inverse().transpose(); /* * rotM*/;

  matrix4<float> mModelView = astate->_camera->mView * mModel;
  matrix4<float> mModelViewProjection = astate->_camera->mViewProjection * mModel;

  astate->bond_shader->set_u(sp_u_name::mModelViewProj,
                             mModelViewProjection.data());
  astate->bond_shader->set_u(sp_u_name::mModelView, mModelView.data());
  astate->bond_shader->set_u(sp_u_name::vColor, (GLfloat*)(color.data()));
  astate->bond_shader->set_u(sp_u_name::mModelViewInvTr,
                             mModelViewInvTr.data());

  astate->cylinder_mesh->render();

}

void draw_pipeline::end_render_bond(){
  app_state* astate = &(c_app::get_state());
  astate->bond_shader->end_shader_program();
}

void draw_pipeline::render_molecule(){

}

void draw_pipeline::render_cell_3d(const vector3<float> &a,
                                   const vector3<float> &b,
                                   const vector3<float> &c ){
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
      vector3<float> vLStart = a*disp[i][0] + b*disp[i][1] + c*disp[i][2];
      vector3<float> vLEnd   = a*disp[i][3] + b*disp[i][4] + c*disp[i][5];

      render_line(clr_black, vLStart, vLEnd );

    }
}

void draw_pipeline::render_vector(){

}

void draw_pipeline::render_primitive(){

}

void draw_pipeline::begin_render_aabb(){
  begin_render_line();
}

void draw_pipeline::render_aabb(const vector3<float> &vColor,
                                const vector3<float> &vBoxMin,
                                const vector3<float> &vBoxMax){

  vector3<float> vHalfBoxSize = (vBoxMax - vBoxMin) * 0.5f;
  vector3<float> vBoxCenter   = (vBoxMax + vBoxMin) * 0.5f;

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

  for (unsigned int i = 0; i < 12; i++){
      vector3<float>
          vLStart(
            vHalfBoxSize[0]*disp[i][0],
            vHalfBoxSize[1]*disp[i][1],
            vHalfBoxSize[2]*disp[i][2]);

      vector3<float>
          vLEnd(
            vHalfBoxSize[0]*disp[i][3],
            vHalfBoxSize[1]*disp[i][4],
            vHalfBoxSize[2]*disp[i][5]);

      render_line(vColor, vBoxCenter+vLStart,vBoxCenter+vLEnd );

    }

}

void draw_pipeline::render_aabb_segmented(const vector3<float> &vColor,
                                          const vector3<float> &vBoxMin,
                                          const vector3<float> &vBoxMax){

  vector3<float> vHalfBoxSize = (vBoxMax - vBoxMin) * 0.5f;
  vector3<float> vBoxCenter   = (vBoxMax + vBoxMin) * 0.5f;

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

   for (unsigned int i = 0; i < 24; i++){
      vector3<float>
          vLStart(
            vHalfBoxSize[0]*disp2[i][0],
            vHalfBoxSize[1]*disp2[i][1],
            vHalfBoxSize[2]*disp2[i][2]);

      vector3<float>
          vLEnd(
            vHalfBoxSize[0]*disp2[i][3],
            vHalfBoxSize[1]*disp2[i][4],
            vHalfBoxSize[2]*disp2[i][5]);

      render_line(vColor, vBoxCenter+vLStart, vBoxCenter+vLEnd, 4.0f);

    }

}

void draw_pipeline::end_render_aabb(){
  end_render_line();
}

void draw_pipeline::begin_render_line(){
  app_state* astate = &(c_app::get_state());
  astate->unit_line_shader->begin_shader_program();
}

void draw_pipeline::render_line(const vector3<float> &color,
                                const vector3<float> &vStart,
                                const vector3<float> &vEnd,
                                const float fLineWidth){
  app_state* astate = &(c_app::get_state());

  glLineWidth(fLineWidth);
  astate->unit_line_shader->set_u(sp_u_name::vColor,
                                  (GLfloat*)color.data());
  astate->unit_line_shader->set_u(sp_u_name::vLineStart,
                                  (GLfloat*)vStart.data());
  astate->unit_line_shader->set_u(sp_u_name::vLineEnd,
                                  (GLfloat*)vEnd.data());
  astate->unit_line_shader->set_u(sp_u_name::mModelViewProj,
                                  astate->_camera->mViewProjection.data());
  astate->unit_line_shader->set_u(sp_u_name::mModelView,
                                  astate->_camera->mView.data());
  astate->unit_line->render();
  glLineWidth(1.0f);

}

void draw_pipeline::end_render_line(){
  app_state* astate = &(c_app::get_state());
  astate->unit_line_shader->end_shader_program();

}
