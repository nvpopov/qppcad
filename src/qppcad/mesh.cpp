#include <qppcad/mesh.hpp>

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
  glDrawElements(mesh_rt, num_vertices, GL_UNSIGNED_INT, 0);

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

  std::cout << fmt::format("Binded data to mesh with vs={}, ns={}, is={}", vertecies.size(),
                 normals.size(),
                 indices.size()) << std::endl;
}

mesh_t* mesh_t::generate_sphere_mesh(const int lat_bands, const int long_bands){
  mesh_t* _mesh = new mesh_t();

  //float radius = 1.0f;

  for (int lat_num = 0; lat_num <= lat_bands; lat_num++){
      float theta = lat_num * qpp::pi / lat_bands;
      float sinTheta = sin(theta);
      float cosTheta = cos(theta);

      for (int long_num = 0; long_num <= long_bands; long_num++) {
          float phi = long_num * 2 * qpp::pi / long_bands;
          float sinPhi = sin(phi);
          float cosPhi = cos(phi);

          float x = cosPhi * sinTheta;   // x
          float y = sinPhi * sinTheta;            // y
          float z = cosTheta;   // z

          _mesh->vertecies.push_back(x);
          _mesh->vertecies.push_back(y);
          _mesh->vertecies.push_back(z);

          vector3<float> vx(x, y, z);
          //vector3<float> norm = vx.normalized();

          for (int i = 0; i < 3; i++) _mesh->normals.push_back(vx(i));

          //          _mesh->normalData.push_back((x+1.0)*0.5);
          //          _mesh->normalData.push_back((y+1.0)*0.5);
          //          _mesh->normalData.push_back((z+1.0)*0.5);
        }
    }

  for (int lat_num = 0; lat_num < lat_bands; lat_num++) {
      for (int long_num = 0; long_num < long_bands; long_num++) {
          int first = (lat_num * (long_bands + 1)) + long_num;
          int second = first + long_bands + 1;

          _mesh->indices.push_back(first);
          _mesh->indices.push_back(second);
          _mesh->indices.push_back(first + 1);

          _mesh->indices.push_back(second);
          _mesh->indices.push_back(second + 1);
          _mesh->indices.push_back(first + 1);

        }
    }
  _mesh->num_vertices = _mesh->indices.size()*3;
  _mesh->bind_data();
  return _mesh;
}

mesh_t *mesh_t::generate_cylinder_whole(const int num_phi, const int num_z){

  mesh_t* _mesh = new mesh_t();
  float R = 1.0f;

  float dZ = 1.0f/(num_z - 1);
  float dPhi = (2.0f*qpp::pi)/(num_phi - 1);
  size_t id = 0;
  int numIdx = 0;

  for (int i = 0; i < num_z-1; i++){
      float zC = dZ*float(i);
      float zN = dZ*float(i+1);
      for (int j = 0; j < num_phi-1; j++){
          float phiC = dPhi*float(j);
          float phiN = dPhi*float(j+1);

          vector3<float> p1(R*cos(phiC), R*sin(phiC), zC);
          vector3<float> p2(R*cos(phiN), R*sin(phiN), zC);
          vector3<float> p3(R*cos(phiN), R*sin(phiN), zN);
          vector3<float> p4(R*cos(phiC), R*sin(phiC), zN);

          vector3<float> n1(R*cos(phiC),R*sin(phiC), zC);
          vector3<float> n2(R*cos(phiN),R*sin(phiN), zC);
          vector3<float> n3(R*cos(phiN),R*sin(phiN), zN);
          vector3<float> n4(R*cos(phiC),R*sin(phiC), zN);
//          std::cout << p1 << std::endl << std::endl
//                    << p2 << std::endl << std::endl
//                    << p3 << std::endl << std::endl
//                    << p4 << std::endl << std::endl;
          dump_vector3_to_vector<float>(_mesh->vertecies, p1);
          dump_vector3_to_vector<float>(_mesh->vertecies, p2);
          dump_vector3_to_vector<float>(_mesh->vertecies, p3);
          dump_vector3_to_vector<float>(_mesh->vertecies, p4);

          dump_vector3_to_vector<float>(_mesh->normals, n1.normalized());
          dump_vector3_to_vector<float>(_mesh->normals, n2.normalized());
          dump_vector3_to_vector<float>(_mesh->normals, n3.normalized());
          dump_vector3_to_vector<float>(_mesh->normals, n4.normalized());

          _mesh->indices.push_back(numIdx*4);
          _mesh->indices.push_back(numIdx*4 + 1);
          _mesh->indices.push_back(numIdx*4 + 2);

          _mesh->indices.push_back(numIdx*4);
          _mesh->indices.push_back(numIdx*4 + 2);
          _mesh->indices.push_back(numIdx*4 + 3);

          //tri = new Triangle(p1, p2, p3);
          //tri = new Triangle(p1, p3, p4);
          numIdx += 1;
        }
    }

  _mesh->num_vertices = _mesh->indices.size()*3;
    std::cout<<_mesh->num_vertices<<std::endl;
  //  std::cout<<_mesh->vertexData[6]<<std::endl;
  _mesh->bind_data();
  return _mesh;
}

mesh_t *mesh_t::generate_unit_line(){
  mesh_t* _mesh = new mesh_t();

  for(int i = 0; i < 3; i++){
      _mesh->vertecies.push_back(0.0);
      _mesh->normals.push_back(0.0);
    }

  for(int i = 0; i < 3; i++){
      _mesh->vertecies.push_back(1.0);
      _mesh->normals.push_back(1.0);
    }

  for(int i = 0; i < 2; i++) _mesh->indices.push_back(i);
  _mesh->num_vertices = 2;
  _mesh->bind_data();
  _mesh->mesh_rt = GL_LINES;
  return _mesh;

}

mesh_t *mesh_t::generate_xz_plane(const int n_x,const float dx, const int n_z, const float dz){
  mesh_t* _mesh = new mesh_t();

  for (int ix = 0; ix < n_x; ix++){
      _mesh->vertecies.push_back(dx * ix);
      _mesh->vertecies.push_back(0.0);
      _mesh->vertecies.push_back(0);

      _mesh->normals.push_back(0.0);
      _mesh->normals.push_back(0.0);
      _mesh->normals.push_back(0.0);

      _mesh->vertecies.push_back(dx * ix);
      _mesh->vertecies.push_back(n_z * dz);
      _mesh->vertecies.push_back(0.0);

      _mesh->normals.push_back(0.0);
      _mesh->normals.push_back(0.0);
      _mesh->normals.push_back(0.0);
    }

  for (int iz = 0; iz < n_z; iz++){
      _mesh->vertecies.push_back(0);
      _mesh->vertecies.push_back(dz * iz);
      _mesh->vertecies.push_back(0.0);

      _mesh->normals.push_back(0.0);
      _mesh->normals.push_back(0.0);
      _mesh->normals.push_back(0.0);

      _mesh->vertecies.push_back(n_x * dz);
      _mesh->vertecies.push_back(dz * iz);
      _mesh->vertecies.push_back(0.0);

      _mesh->normals.push_back(0.0);
      _mesh->normals.push_back(0.0);
      _mesh->normals.push_back(0.0);
    }

  _mesh->num_vertices = _mesh->vertecies.size() / 2;

  for (int i = 0; i < _mesh->num_vertices; i++){
     _mesh->indices.push_back(i * 2 );
     _mesh->indices.push_back(i * 2 + 1);
    }


  _mesh->bind_data();
  _mesh->mesh_rt = GL_LINES;
  return _mesh;
}


