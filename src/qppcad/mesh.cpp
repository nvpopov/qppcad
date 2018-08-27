#include <qppcad/mesh.hpp>

using namespace qpp;

mesh::mesh(){
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

void mesh::render(){
  glBindVertexArray(vao);
  glDrawElements(mesh_rt, num_vertices, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}

void mesh::bind_data(){
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER,
               vertexData.size()*sizeof(float),
               &vertexData[0],GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, nbo);
  glBufferData(GL_ARRAY_BUFFER,
               normalData.size()*sizeof(float),
               &normalData[0], GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               indicesData.size()*sizeof(int),
               &indicesData[0], GL_STATIC_DRAW);
  glBindVertexArray(0);

  std::cout << fmt::format(
                 "Binded data to mesh with vs={}, ns={}, is={}", vertexData.size(),
                 normalData.size(),
                 indicesData.size()) << std::endl;
}

mesh* mesh::generate_sphere_mesh(const int latBands, const int longBands){
  mesh* _mesh = new mesh();

  //float radius = 1.0f;

  for (int latNumber = 0; latNumber <= latBands; latNumber++){
      float theta = latNumber * qpp::pi / latBands;
      float sinTheta = sin(theta);
      float cosTheta = cos(theta);

      for (int longNumber = 0; longNumber <= longBands; longNumber++) {
          float phi = longNumber * 2 * qpp::pi / longBands;
          float sinPhi = sin(phi);
          float cosPhi = cos(phi);

          float x = cosPhi * sinTheta;   // x
          float y = sinPhi * sinTheta;            // y
          float z = cosTheta;   // z

          _mesh->vertexData.push_back(x);
          _mesh->vertexData.push_back(y);
          _mesh->vertexData.push_back(z);

          vector3<float> vx(x, y, z);
          //vector3<float> norm = vx.normalized();

          for (int i = 0; i < 3; i++) _mesh->normalData.push_back(vx(i));

          //          _mesh->normalData.push_back((x+1.0)*0.5);
          //          _mesh->normalData.push_back((y+1.0)*0.5);
          //          _mesh->normalData.push_back((z+1.0)*0.5);
        }
    }

  for (int latNumber = 0; latNumber < latBands; latNumber++) {
      for (int longNumber = 0; longNumber < longBands; longNumber++) {
          int first = (latNumber * (longBands + 1)) + longNumber;
          int second = first + longBands + 1;

          _mesh->indicesData.push_back(first);
          _mesh->indicesData.push_back(second);
          _mesh->indicesData.push_back(first + 1);

          _mesh->indicesData.push_back(second);
          _mesh->indicesData.push_back(second + 1);
          _mesh->indicesData.push_back(first + 1);

        }
    }
  _mesh->num_vertices = _mesh->indicesData.size()*3;
  _mesh->bind_data();
  return _mesh;
}

mesh *mesh::generate_cylinder_whole(const int numPhi, const int numZ){

  mesh* _mesh = new mesh();
  float R = 1.0f;

  float dZ = 1.0f/(numZ - 1);
  float dPhi = (2.0f*qpp::pi)/(numPhi - 1);
  size_t id = 0;
  int numIdx = 0;

  for (int i = 0; i < numZ-1; i++){
      float zC = dZ*float(i);
      float zN = dZ*float(i+1);
      for (int j = 0; j < numPhi-1; j++){
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
          dump_vector3_to_vector<float>(_mesh->vertexData, p1);
          dump_vector3_to_vector<float>(_mesh->vertexData, p2);
          dump_vector3_to_vector<float>(_mesh->vertexData, p3);
          dump_vector3_to_vector<float>(_mesh->vertexData, p4);

          dump_vector3_to_vector<float>(_mesh->normalData, n1.normalized());
          dump_vector3_to_vector<float>(_mesh->normalData, n2.normalized());
          dump_vector3_to_vector<float>(_mesh->normalData, n3.normalized());
          dump_vector3_to_vector<float>(_mesh->normalData, n4.normalized());

          _mesh->indicesData.push_back(numIdx*4);
          _mesh->indicesData.push_back(numIdx*4 + 1);
          _mesh->indicesData.push_back(numIdx*4 + 2);

          _mesh->indicesData.push_back(numIdx*4);
          _mesh->indicesData.push_back(numIdx*4 + 2);
          _mesh->indicesData.push_back(numIdx*4 + 3);

          //tri = new Triangle(p1, p2, p3);
          //tri = new Triangle(p1, p3, p4);
          numIdx += 1;
        }
    }

  _mesh->num_vertices = _mesh->indicesData.size()*3;
    std::cout<<_mesh->num_vertices<<std::endl;
  //  std::cout<<_mesh->vertexData[6]<<std::endl;
  _mesh->bind_data();
  return _mesh;
}

mesh *mesh::generate_unit_line(){
  mesh* _mesh = new mesh();

  for(int i = 0; i < 3; i++){
      _mesh->vertexData.push_back(0.0);
      _mesh->normalData.push_back(0.0);
    }

  for(int i = 0; i < 3; i++){
      _mesh->vertexData.push_back(1.0);
      _mesh->normalData.push_back(1.0);
    }

  for(int i = 0; i < 2; i++) _mesh->indicesData.push_back(i);
  _mesh->num_vertices = 2;
  _mesh->bind_data();
  _mesh->mesh_rt = GL_LINES;
  return _mesh;

}

mesh *mesh::generate_xz_plane(const int iNumX,
                              const float fDeltaX,
                              const int iNumZ,
                              const float fDeltaZ){
  mesh* _mesh = new mesh();

  for (int ix = 0; ix < iNumX; ix++){
      _mesh->vertexData.push_back(fDeltaX * ix);
      _mesh->vertexData.push_back(0.0);
      _mesh->vertexData.push_back(0);

      _mesh->normalData.push_back(0.0);
      _mesh->normalData.push_back(0.0);
      _mesh->normalData.push_back(0.0);

      _mesh->vertexData.push_back(fDeltaX * ix);
      _mesh->vertexData.push_back(iNumZ*fDeltaZ);
      _mesh->vertexData.push_back(0.0);

      _mesh->normalData.push_back(0.0);
      _mesh->normalData.push_back(0.0);
      _mesh->normalData.push_back(0.0);
    }

  for (int iz = 0; iz < iNumZ; iz++){
      _mesh->vertexData.push_back(0);
      _mesh->vertexData.push_back(fDeltaZ * iz);
      _mesh->vertexData.push_back(0.0);

      _mesh->normalData.push_back(0.0);
      _mesh->normalData.push_back(0.0);
      _mesh->normalData.push_back(0.0);

      _mesh->vertexData.push_back(iNumX*fDeltaX);
      _mesh->vertexData.push_back(fDeltaZ * iz);
      _mesh->vertexData.push_back(0.0);

      _mesh->normalData.push_back(0.0);
      _mesh->normalData.push_back(0.0);
      _mesh->normalData.push_back(0.0);
    }

  _mesh->num_vertices = _mesh->vertexData.size() / 2;

  for (int i = 0; i < _mesh->num_vertices; i++){
     _mesh->indicesData.push_back(i * 2 );
     _mesh->indicesData.push_back(i * 2 + 1);
    }


  _mesh->bind_data();
  _mesh->mesh_rt = GL_LINES;
  return _mesh;
}


