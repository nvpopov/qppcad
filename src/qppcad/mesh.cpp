#include <qppcad/mesh.hpp>
#include <qppcad/app.hpp>


using namespace qpp;

mesh_t::mesh_t(){
  mesh_rt = GL_TRIANGLES;

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glGenBuffers(1, &nbo);
  glBindBuffer(GL_ARRAY_BUFFER, nbo);

  glGenBuffers(1, &vio);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
  glBindVertexArray(0);
}

void mesh_t::render(){
  glBindVertexArray(vao);
  glDrawElements(mesh_rt, num_primitives, GL_UNSIGNED_INT, 0);
}

void mesh_t::begin_render_batch(){
  glBindVertexArray(vao);
}

void mesh_t::render_batch(){
  glDrawElements(mesh_rt, num_primitives, GL_UNSIGNED_INT, 0);
}

void mesh_t::end_render_batch(){
  glBindVertexArray(0);
}

void mesh_t::bind_data(){
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER,
               vertecies.size()*sizeof(float),
               &vertecies[0],GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, nbo);
  glBufferData(GL_ARRAY_BUFFER,
               normals.size()*sizeof(float),
               &normals[0], GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               indices.size()*sizeof(int),
               &indices[0], GL_STATIC_DRAW);
  glBindVertexArray(0);

  c_app::log(fmt::format("Binded data to mesh with vs={}, ns={}, is={}",
                         vertecies.size(),
                         normals.size(),
                         indices.size()));
}




