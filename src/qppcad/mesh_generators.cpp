#include <qppcad/mesh_generators.hpp>

using namespace qpp;
using namespace qpp::cad;


mesh_t *mesh_generators::plane_zl() {

  mesh_t* _mesh = new mesh_t();

  float quad_vert[12] = {
    -0.5, -0.5, 0,
    0.5, -0.5,  0,
    0.5,  0.5,  0,
    -0.5,  0.5,  0
  };

  float quad_n[12] = {
    0,  0,   1,
    0,  0,   1,
    0,  0,   1,
    0,  0,   1
  };

  uint8_t quad_idx[6] = {
    0, 1, 2,
    2, 3, 0
  };

  for (uint8_t i = 0; i < 12; i++){
      _mesh->vertecies.push_back(quad_vert[i]);
      _mesh->normals.push_back(quad_n[i]);
    }

  for (uint8_t i = 0; i < 6; i++) _mesh->indices.push_back(quad_idx[i]);

  _mesh->num_primitives = _mesh->indices.size();
  _mesh->bind_data();
  return _mesh;
}

mesh_t *mesh_generators::quad_zup( ) {
  mesh_t* _mesh = new mesh_t();

  float quad_vert[12] = {
    -0.5, -0.5,  0,
    0.5, -0.5,  0,
    0.5,  0.5,  0,
    -0.5,  0.5,  0
  };

  float quad_n[12] = {
    0,  0,  1,
    0,  0,  1,
    0,  0,  1,
    0,  0,  1
  };

  uint8_t quad_idx[6] = {
    0, 1, 2,
    2, 3, 0
  };

  for (uint8_t i = 0; i < 12; i++){
      _mesh->vertecies.push_back(quad_vert[i]);
      _mesh->normals.push_back(quad_n[i]);
    }

  for (uint8_t i = 0; i < 6; i++) _mesh->indices.push_back(quad_idx[i]);

  _mesh->num_primitives = _mesh->indices.size();
  _mesh->bind_data();
  return _mesh;

}

mesh_t *mesh_generators::quad () {
  mesh_t* _mesh = new mesh_t();

  float quad_vert[12] = {
    -1, -1,  0,
    1, -1,  0,
    1,  1,  0,
    -1,  1,  0
  };

  float quad_texc[12] = {
    0,  0,  0,
    1,  0,  0,
    1,  1,  0,
    0,  1,  0
  };

  uint8_t quad_idx[6] = {
    0, 1, 2,
    2, 3, 0
  };

  for (uint8_t i = 0; i < 12; i++){
      _mesh->vertecies.push_back(quad_vert[i]);
      _mesh->normals.push_back(quad_texc[i]);
    }

  for (uint8_t i = 0; i < 6; i++) _mesh->indices.push_back(quad_idx[i]);


  _mesh->num_primitives = _mesh->indices.size()*3;
  _mesh->bind_data();
  return _mesh;
}

mesh_t *mesh_generators::sphere (const int lat_bands, const int long_bands) {
  mesh_t* _mesh = new mesh_t();

  //float radius = 1.0f;

  for (int lat_num = 0; lat_num <= lat_bands; lat_num++){
      float theta = lat_num * float(qpp::pi / lat_bands);
      float sin_theta = std::sin(theta);
      float cos_theta = std::cos(theta);

      for (int long_num = 0; long_num <= long_bands; long_num++) {
          float phi = long_num * 2.0f * float(qpp::pi / long_bands);
          float sin_phi = std::sin(phi);
          float cos_phi = std::cos(phi);

          float x = cos_phi * sin_theta;   // x
          float y = sin_phi * sin_theta;            // y
          float z = cos_theta;   // z

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
  _mesh->num_primitives = _mesh->indices.size()*3;
  _mesh->bind_data();
  return _mesh;
}

mesh_t *mesh_generators::cylinder_whole (const int num_phi, const int num_z) {

  mesh_t* _mesh = new mesh_t();
  float R = 1.0f;

  float dZ = 1.0f/(num_z - 1);
  float dPhi = (2.0f * float(qpp::pi)) / (num_phi - 1);

  int numIdx = 0;

  for (int i = 0; i < num_z-1; i++){

      float zC = dZ*float(i);
      float zN = dZ*float(i+1);

      for (int j = 0; j < num_phi-1; j++){

          float phiC = dPhi*float(j);
          float phiN = dPhi*float(j+1);

          vector3<float> p1(R*std::cos(phiC), R*std::sin(phiC), zC);
          vector3<float> p2(R*std::cos(phiN), R*std::sin(phiN), zC);
          vector3<float> p3(R*std::cos(phiN), R*std::sin(phiN), zN);
          vector3<float> p4(R*std::cos(phiC), R*std::sin(phiC), zN);

          vector3<float> n1(R*std::cos(phiC),R*std::sin(phiC), zC);
          vector3<float> n2(R*std::cos(phiN),R*std::sin(phiN), zC);
          vector3<float> n3(R*std::cos(phiN),R*std::sin(phiN), zN);
          vector3<float> n4(R*std::cos(phiC),R*std::sin(phiC), zN);

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

          numIdx += 1;

        }
    }

  _mesh->num_primitives = _mesh->indices.size()*3;
  _mesh->bind_data();
  return _mesh;

}

mesh_t *mesh_generators::cylinder_mk2 (const int num_segment_height,
                                       const int num_segment_base,
                                       const float radius, const float height) {

  mesh_t* _mesh = new mesh_t();

  float delta_angle  = (static_cast<float>(qpp::pi) * 2.0f) / num_segment_base;
  float delta_height = height / static_cast<float>(num_segment_height);
  uint32_t offset = 0;

  for (uint8_t i = 0; i <= num_segment_height; i++) {
      for (uint8_t j = 0; j <= num_segment_base; j++) {

          float x_0 = std::sin(j * delta_angle);
          float y_0 = std::cos(j * delta_angle);

          vector3<float> normal1 = vector3<float>(x_0, y_0, 0.0f);
          normal1 = normal1.normalized();

          _mesh->vertecies.push_back(x_0);
          _mesh->vertecies.push_back(y_0);
          _mesh->vertecies.push_back(i * delta_height);

          _mesh->normals.push_back(normal1[0]);
          _mesh->normals.push_back(normal1[1]);
          _mesh->normals.push_back(normal1[2]);

          if (i != num_segment_height) {
              _mesh->indices.push_back(offset + num_segment_base + 1);
              _mesh->indices.push_back(offset);
              _mesh->indices.push_back(offset + num_segment_base);
              _mesh->indices.push_back(offset + num_segment_base + 1);
              _mesh->indices.push_back(offset + 1);
              _mesh->indices.push_back(offset);
            }
          offset ++;
        }
    }

  uint32_t center_index = offset;
  _mesh->vertecies.push_back(0.0f);
  _mesh->vertecies.push_back(0.0f);
  _mesh->vertecies.push_back(0.0f);
  _mesh->normals.push_back( 0.0f);
  _mesh->normals.push_back( 0.0);
  _mesh->normals.push_back(-1.0f);

  offset++;

  for (uint32_t j = 0; j <= num_segment_base; j++) {

      float x_0 = sinf(j * delta_angle);
      float y_0 = cosf(j * delta_angle);

      _mesh->vertecies.push_back(x_0);
      _mesh->vertecies.push_back(y_0);
      _mesh->vertecies.push_back(0.0f);

      _mesh->normals.push_back( 0.0f);
      _mesh->normals.push_back( 0.0f);
      _mesh->normals.push_back( -1.0f);

      if (j!= num_segment_base) {
          _mesh->indices.push_back(center_index);
          _mesh->indices.push_back(offset);
          _mesh->indices.push_back(offset+1);
        }
      offset++;
    }

  center_index = offset;
  _mesh->vertecies.push_back(0.0f);
  _mesh->vertecies.push_back(0.0f);
  _mesh->vertecies.push_back(height);
  _mesh->normals.push_back(0.0f);
  _mesh->normals.push_back(0.0f);
  _mesh->normals.push_back(1.0f);

  offset++;

  for (uint32_t j = 0; j <= num_segment_base; j++){
      float x_0 = std::sin(j * delta_angle);
      float y_0 = std::cos(j * delta_angle);

      _mesh->vertecies.push_back(x_0);
      _mesh->vertecies.push_back(y_0);
      _mesh->vertecies.push_back(height);

      _mesh->normals.push_back( 0.0f);
      _mesh->normals.push_back( 0.0f);
      _mesh->normals.push_back( 1.0f);

      if (j != num_segment_base) {
          _mesh->indices.push_back(center_index);
          _mesh->indices.push_back(offset+1);
          _mesh->indices.push_back(offset);
        }
      offset++;
    }


  _mesh->num_primitives = _mesh->indices.size();
  _mesh->bind_data();
  _mesh->mesh_rt = GL_TRIANGLES;
  return _mesh;
}

mesh_t *mesh_generators::unit_line() {

  mesh_t* _mesh = new mesh_t();

  for (int i = 0; i < 3; i++) {
      _mesh->vertecies.push_back(0.0);
      _mesh->normals.push_back(0.0);
    }

  for (int i = 0; i < 3; i++) {
      _mesh->vertecies.push_back(1.0);
      _mesh->normals.push_back(1.0);
    }

  for (int i = 0; i < 2; i++) _mesh->indices.push_back(i);
  _mesh->num_primitives = 2;
  _mesh->bind_data();
  _mesh->mesh_rt = GL_LINES;
  return _mesh;

}

mesh_t *mesh_generators::cone(const float radius,
                              const float height,
                              const uint8_t num_segment_height,
                              const uint8_t num_segment_base) {

  mesh_t* _mesh = new mesh_t();

  float delta_angle  = float(qpp::pi * 2) / num_segment_base;
  float delta_height = height / static_cast<float>(num_segment_height);

  vector3<float> normal = (vector3<float>(radius, 0.0f, height)).normalized();
  uint32_t offset = 0;

  for (uint8_t i = 0; i <= num_segment_height; i++) {
      float r_0 = radius * (1 - i / static_cast<float>(num_segment_height));

      for (uint8_t j = 0; j <= num_segment_base; j++) {
          float x_0 = r_0 * cosf(j * delta_angle);
          float y_0 = r_0 * sinf(j * delta_angle);

          Eigen::Matrix3f rotm;
          rotm = Eigen::AngleAxisf(j * delta_angle, vector3<float>::UnitZ());
          vector3<float> normal_rotated = rotm * normal;
          _mesh->vertecies.push_back(x_0);
          _mesh->vertecies.push_back(y_0);
          _mesh->vertecies.push_back(i * delta_height);

          _mesh->normals.push_back(normal_rotated[0]);
          _mesh->normals.push_back(normal_rotated[1]);
          _mesh->normals.push_back(normal_rotated[2]);

          if (i != num_segment_height && j != num_segment_base) {

              _mesh->indices.push_back(offset + num_segment_base + 2);
              _mesh->indices.push_back(offset + num_segment_base + 1);
              _mesh->indices.push_back(offset);

              _mesh->indices.push_back(offset + num_segment_base + 2);
              _mesh->indices.push_back(offset);
              _mesh->indices.push_back(offset + 1);

            }

          offset ++;
        }
    }

  //lo cap

  uint32_t center_idx = offset;
  _mesh->vertecies.push_back(0.0f);
  _mesh->vertecies.push_back(0.0f);
  _mesh->vertecies.push_back(0.0f);

  _mesh->normals.push_back(0.0f);
  _mesh->normals.push_back(-1.0f);
  _mesh->normals.push_back(1.0f);

  offset++;

  for (uint8_t j=0; j<= num_segment_base; j++){

      float x_0 = radius * cosf(j * delta_angle);
      float y_0 = radius * sinf(j * delta_angle);

      _mesh->vertecies.push_back(x_0);
      _mesh->vertecies.push_back(y_0);
      _mesh->vertecies.push_back(0.0f);

      _mesh->normals.push_back(0.0f);
      _mesh->normals.push_back(0.0f);
      _mesh->normals.push_back(-1.0f);


      if (j != num_segment_base){
          _mesh->indices.push_back(center_idx);
          _mesh->indices.push_back(offset + 1);
          _mesh->indices.push_back(offset);
        }

      offset++;
    }

  _mesh->num_primitives = _mesh->indices.size();
  _mesh->bind_data();
  _mesh->mesh_rt = GL_TRIANGLES;
  return _mesh;
}

mesh_t *mesh_generators::unit_cube() {
  mesh_t* _mesh = new mesh_t();
  float cv[] = {
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
  };

  for (uint8_t i = 0 ; i < 12; i++){
      _mesh->vertecies.push_back(cv[i*9]);
      _mesh->vertecies.push_back(cv[i*9+1]);
      _mesh->vertecies.push_back(cv[i*9+2]);

      _mesh->vertecies.push_back(cv[(i*9)+3]);
      _mesh->vertecies.push_back(cv[(i*9)+4]);
      _mesh->vertecies.push_back(cv[(i*9)+5]);

      _mesh->vertecies.push_back(cv[(i*9)+6]);
      _mesh->vertecies.push_back(cv[(i*9)+7]);
      _mesh->vertecies.push_back(cv[(i*9)+8]);


      vector3<float> pos1(cv[i*9],     cv[i*9+1],     cv[i*9+2]);
      vector3<float> pos2(cv[(i*9)+3], cv[(i*9)+4],   cv[(i*9)+5]);
      vector3<float> pos3(cv[(i*9)+6], cv[(i*9)+7],   cv[(i*9)+8]);

      vector3<float> surf_normal = ((pos2-pos1).cross(pos3-pos1)).normalized();

      _mesh->normals.push_back(surf_normal[0]);
      _mesh->normals.push_back(surf_normal[1]);
      _mesh->normals.push_back(surf_normal[2]);

      _mesh->normals.push_back(surf_normal[0]);
      _mesh->normals.push_back(surf_normal[1]);
      _mesh->normals.push_back(surf_normal[2]);

      _mesh->normals.push_back(surf_normal[0]);
      _mesh->normals.push_back(surf_normal[1]);
      _mesh->normals.push_back(surf_normal[2]);

      _mesh->indices.push_back(i*3);
      _mesh->indices.push_back((i*3) + 1);
      _mesh->indices.push_back((i*3) + 2);


    }

  _mesh->num_primitives = 36;
  _mesh->bind_data();
  _mesh->mesh_rt = GL_TRIANGLES;
  return _mesh;
}

mesh_t *mesh_generators::xz_plane(const int n_x, const float dx, const int n_z, const float dz){
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

  _mesh->num_primitives = _mesh->vertecies.size() / 2;

  for (int i = 0; i < _mesh->num_primitives; i++){
      _mesh->indices.push_back(i * 2 );
      _mesh->indices.push_back(i * 2 + 1);
    }


  _mesh->bind_data();
  _mesh->mesh_rt = GL_LINES;
  return _mesh;
}

mesh_t *mesh_generators::cross_line_atom () {
  mesh_t* _mesh = new mesh_t();

  // 0 - 0
  _mesh->vertecies.push_back(1.0);
  _mesh->vertecies.push_back(0.0);
  _mesh->vertecies.push_back(0.0);

  _mesh->normals.push_back(0.0);
  _mesh->normals.push_back(0.0);
  _mesh->normals.push_back(0.0);

  // 0 - 1
  _mesh->vertecies.push_back(-1.0);
  _mesh->vertecies.push_back(0.0);
  _mesh->vertecies.push_back(0.0);

  _mesh->normals.push_back(0.0);
  _mesh->normals.push_back(0.0);
  _mesh->normals.push_back(0.0);

  // 1 - 0
  _mesh->vertecies.push_back(0.0);
  _mesh->vertecies.push_back(1.0);
  _mesh->vertecies.push_back(0.0);

  _mesh->normals.push_back(0.0);
  _mesh->normals.push_back(0.0);
  _mesh->normals.push_back(0.0);

  // 1 - 1
  _mesh->vertecies.push_back(0.0);
  _mesh->vertecies.push_back(-1.0);
  _mesh->vertecies.push_back(0.0);

  _mesh->normals.push_back(0.0);
  _mesh->normals.push_back(0.0);
  _mesh->normals.push_back(0.0);

  // 2 - 0
  _mesh->vertecies.push_back(0.0);
  _mesh->vertecies.push_back(0.0);
  _mesh->vertecies.push_back(1.0);

  _mesh->normals.push_back(0.0);
  _mesh->normals.push_back(0.0);
  _mesh->normals.push_back(0.0);

  // 2 - 1
  _mesh->vertecies.push_back(0.0);
  _mesh->vertecies.push_back(0.0);
  _mesh->vertecies.push_back(-1.0);

  _mesh->normals.push_back(0.0);
  _mesh->normals.push_back(0.0);
  _mesh->normals.push_back(0.0);

  _mesh->indices.push_back(0);
  _mesh->indices.push_back(1);
  _mesh->indices.push_back(2);
  _mesh->indices.push_back(3);
  _mesh->indices.push_back(4);
  _mesh->indices.push_back(5);

  _mesh->bind_data();
  _mesh->num_primitives = 6;
  _mesh->mesh_rt = GL_LINES;

  return _mesh;

}
