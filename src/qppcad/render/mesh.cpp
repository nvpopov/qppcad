#include <qppcad/render/mesh.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

mesh_t::mesh_t() {
  construct_explicit();
}

void mesh_t::construct_explicit() {

  mesh_rt = GL_TRIANGLES;

  app_state_t* astate = app_state_t::get_inst();
  glapi_t* glapi = astate->glapi;

  glapi->glGenVertexArrays(1, &vao);
  glapi->glBindVertexArray(vao);

  glapi->glGenBuffers(1, &vbo);
  glapi->glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glapi->glGenBuffers(1, &nbo);
  glapi->glBindBuffer(GL_ARRAY_BUFFER, nbo);

  glapi->glGenBuffers(1, &vio);
  glapi->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
  glapi->glBindVertexArray(0);

}

void mesh_t::render() {

  app_state_t* astate = app_state_t::get_inst();
  glapi_t* glapi = astate->glapi;
  glapi->glBindVertexArray(vao);
  glapi->glDrawElements(mesh_rt, num_primitives, GL_UNSIGNED_INT, nullptr);

}

void mesh_t::begin_render_batch() {

  app_state_t* astate = app_state_t::get_inst();
  glapi_t* glapi = astate->glapi;
  glapi->glBindVertexArray(vao);

}

void mesh_t::render_batch() {

  app_state_t* astate = app_state_t::get_inst();
  glapi_t* glapi = astate->glapi;
  glapi->glDrawElements(mesh_rt, num_primitives, GL_UNSIGNED_INT, nullptr);

}

void mesh_t::end_render_batch() {

  app_state_t* astate = app_state_t::get_inst();
  glapi_t* glapi = astate->glapi;
  glapi->glBindVertexArray(0);

}

void mesh_t::bind_data() {
  app_state_t* astate = app_state_t::get_inst();
  glapi_t* glapi = astate->glapi;
  glapi->glBindVertexArray(0);

  astate->tlog("Pre bind data to mesh[vao={}, vbo={}, nbo={}, vio={}]",
               vao, vbo, nbo, vio);

  glapi->glBindVertexArray(vao);

  glapi->glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glapi->glBufferData(GL_ARRAY_BUFFER,
                      vertecies.size()*sizeof(float),
                      &vertecies[0],GL_STATIC_DRAW);
  glapi->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glapi->glEnableVertexAttribArray(0);

  glapi->glBindBuffer(GL_ARRAY_BUFFER, nbo);
  glapi->glBufferData(GL_ARRAY_BUFFER,
                      normals.size()*sizeof(float),
                      &normals[0], GL_STATIC_DRAW);
  glapi->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glapi->glEnableVertexAttribArray(1);

  glapi->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
  glapi->glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                      indices.size()*sizeof(int),
                      &indices[0], GL_STATIC_DRAW);
  glapi->glBindVertexArray(0);

  astate->tlog("Binded data to mesh[vao={}, vbo={}, nbo={}, vio={}] with vs={}, ns={}, is={}",
               vao, vbo, nbo, vio,
               vertecies.size(),
               normals.size(),
               indices.size());

}




