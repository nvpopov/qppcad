#include <qppcad/render/volume_tools.hpp>
#include <qppcad/render/marching_cubes_tables.hpp>

using namespace qpp;
using namespace qpp::cad;

void volume_helper::polygonise_vol_mc(mesh_t &mesh,
                                      scalar_volume_t<float> &volume,
                                      float isolevel,
                                      int steps,
                                      int addr_mode) {

  mesh.vertecies.clear();
  mesh.normals.clear();
  mesh.indices.clear();

  int num_idx = 0;
  //iterate over all sub-cubes
  std::array<float, 8> gv;
  std::array<vector3<float>, 8> gp;
  std::array<vector3<float>, 8> gn;
  std::array<vector3<float>, 12> vertlist;
  std::array<vector3<float>, 12> normlist;

  vector3<float> p0;
  vector3<float> p1;
  vector3<float> p2;

  vector3<float> n0;
  vector3<float> n1;
  vector3<float> n2;

  vector3<float> n_ot;

  int ntriang = 0;
  int cubeindex = 0;

  for (int ix = -1; ix < volume.m_steps[0]; ix++)
    for (int iy = -1; iy < volume.m_steps[1]; iy++)
      for (int iz = 0-1; iz < volume.m_steps[2]; iz++) {

          //restore cube coords 0,0 - top
          //for ref http://paulbourke.net/geometry/polygonise/polygonise1.gif

          gp[0] = ix * volume.m_axis[0] + iy * volume.m_axis[1] + iz * volume.m_axis[2];  //0 0 0

          gp[1] = gp[0] + volume.m_axis[0]; // +1  0 0
          gp[2] = gp[1] + volume.m_axis[1]; // +1 +1 0
          gp[3] = gp[0] + volume.m_axis[1]; //  0 +1 0
          gp[4] = gp[0] + volume.m_axis[2]; //  0  0 +1
          gp[5] = gp[4] + volume.m_axis[0]; // +1  0 +1
          gp[6] = gp[5] + volume.m_axis[1]; // +1 +1 +1
          gp[7] = gp[4] + volume.m_axis[1]; //  0 +1 +1

          if (addr_mode == 0) {
              gv[0] = get_field_value_at(ix,     iy,     iz,     volume);
              gv[1] = get_field_value_at(ix + 1, iy,     iz,     volume);
              gv[2] = get_field_value_at(ix + 1, iy + 1, iz,     volume);
              gv[3] = get_field_value_at(ix,     iy + 1, iz,     volume);
              gv[4] = get_field_value_at(ix,     iy,     iz + 1, volume);
              gv[5] = get_field_value_at(ix + 1, iy,     iz + 1, volume);
              gv[6] = get_field_value_at(ix + 1, iy + 1, iz + 1, volume);
              gv[7] = get_field_value_at(ix,     iy + 1, iz + 1, volume);
            } else {
              gv[0] = get_field_value_at_v2(ix,     iy,     iz,     volume);
              gv[1] = get_field_value_at_v2(ix + 1, iy,     iz,     volume);
              gv[2] = get_field_value_at_v2(ix + 1, iy + 1, iz,     volume);
              gv[3] = get_field_value_at_v2(ix,     iy + 1, iz,     volume);
              gv[4] = get_field_value_at_v2(ix,     iy,     iz + 1, volume);
              gv[5] = get_field_value_at_v2(ix + 1, iy,     iz + 1, volume);
              gv[6] = get_field_value_at_v2(ix + 1, iy + 1, iz + 1, volume);
              gv[7] = get_field_value_at_v2(ix,     iy + 1, iz + 1, volume);
            }

          gn[0] = get_vertex_normal_from_cube(ix,     iy,     iz,     volume, addr_mode);
          gn[1] = get_vertex_normal_from_cube(ix + 1, iy,     iz,     volume, addr_mode);
          gn[2] = get_vertex_normal_from_cube(ix + 1, iy + 1, iz,     volume, addr_mode);
          gn[3] = get_vertex_normal_from_cube(ix,     iy + 1, iz,     volume, addr_mode);
          gn[4] = get_vertex_normal_from_cube(ix,     iy,     iz + 1, volume, addr_mode);
          gn[5] = get_vertex_normal_from_cube(ix + 1, iy,     iz + 1, volume, addr_mode);
          gn[6] = get_vertex_normal_from_cube(ix + 1, iy + 1, iz + 1, volume, addr_mode);
          gn[7] = get_vertex_normal_from_cube(ix,     iy + 1, iz + 1, volume, addr_mode);

          cubeindex = 0;

          if (gv[0] < isolevel) cubeindex |= 1;
          if (gv[1] < isolevel) cubeindex |= 2;
          if (gv[2] < isolevel) cubeindex |= 4;
          if (gv[3] < isolevel) cubeindex |= 8;
          if (gv[4] < isolevel) cubeindex |= 16;
          if (gv[5] < isolevel) cubeindex |= 32;
          if (gv[6] < isolevel) cubeindex |= 64;
          if (gv[7] < isolevel) cubeindex |= 128;

          if (mc_edge_table[cubeindex] == 0) continue;

          if (mc_edge_table[cubeindex] & 1) {
              vertlist[0] = vertex_interp(isolevel, gp[0], gp[1], gv[0], gv[1]);
              normlist[0] = vertex_interp(isolevel, gn[0], gn[1], gv[0], gv[1]);
            }

          if (mc_edge_table[cubeindex] & 2) {
              vertlist[1] = vertex_interp(isolevel, gp[1], gp[2], gv[1], gv[2]);
              normlist[1] = vertex_interp(isolevel, gn[1], gn[2], gv[1], gv[2]);
            }

          if (mc_edge_table[cubeindex] & 4) {
              vertlist[2] = vertex_interp(isolevel, gp[2], gp[3], gv[2], gv[3]);
              normlist[2] = vertex_interp(isolevel, gn[2], gn[3], gv[2], gv[3]);
            }

          if (mc_edge_table[cubeindex] & 8) {
              vertlist[3] = vertex_interp(isolevel, gp[3], gp[0], gv[3], gv[0]);
              normlist[3] = vertex_interp(isolevel, gn[3], gn[0], gv[3], gv[0]);
            }

          if (mc_edge_table[cubeindex] & 16) {
              vertlist[4] = vertex_interp(isolevel, gp[4], gp[5], gv[4], gv[5]);
              normlist[4] = vertex_interp(isolevel, gn[4], gn[5], gv[4], gv[5]);
            }

          if (mc_edge_table[cubeindex] & 32) {
              vertlist[5] = vertex_interp(isolevel, gp[5], gp[6], gv[5], gv[6]);
              normlist[5] = vertex_interp(isolevel, gn[5], gn[6], gv[5], gv[6]);
            }

          if (mc_edge_table[cubeindex] & 64) {
              vertlist[6] = vertex_interp(isolevel, gp[6], gp[7], gv[6], gv[7]);
              normlist[6] = vertex_interp(isolevel, gn[6], gn[7], gv[6], gv[7]);
            }

          if (mc_edge_table[cubeindex] & 128) {
              vertlist[7] = vertex_interp(isolevel, gp[7], gp[4], gv[7], gv[4]);
              normlist[7] = vertex_interp(isolevel, gn[7], gn[4], gv[7], gv[4]);
            }

          if (mc_edge_table[cubeindex] & 256) {
              vertlist[8] = vertex_interp(isolevel, gp[0], gp[4], gv[0], gv[4]);
              normlist[8] = vertex_interp(isolevel, gn[0], gn[4], gv[0], gv[4]);
            }

          if (mc_edge_table[cubeindex] & 512) {
              vertlist[9] = vertex_interp(isolevel, gp[1], gp[5], gv[1], gv[5]);
              normlist[9] = vertex_interp(isolevel, gn[1], gn[5], gv[1], gv[5]);
            }

          if (mc_edge_table[cubeindex] & 1024) {
              vertlist[10] = vertex_interp(isolevel, gp[2], gp[6], gv[2], gv[6]);
              normlist[10] = vertex_interp(isolevel, gn[2], gn[6], gv[2], gv[6]);
            }

          if (mc_edge_table[cubeindex] & 2048) {
              vertlist[11] = vertex_interp(isolevel, gp[3], gp[7], gv[3], gv[7]);
              normlist[11] = vertex_interp(isolevel, gn[3], gn[7], gv[3], gv[7]);
            }

          ntriang = 0;

          for (int i = 0; mc_tri_table[cubeindex][i] != -1;i += 3) {

              if (isolevel > 0) {
                  p0 = vertlist[mc_tri_table[cubeindex][i  ]];
                  p1 = vertlist[mc_tri_table[cubeindex][i+1]];
                  p2 = vertlist[mc_tri_table[cubeindex][i+2]];

                  n0 = normlist[mc_tri_table[cubeindex][i  ]];
                  n1 = normlist[mc_tri_table[cubeindex][i+1]];
                  n2 = normlist[mc_tri_table[cubeindex][i+2]];

                } else {
                  p2 = vertlist[mc_tri_table[cubeindex][i  ]];
                  p1 = vertlist[mc_tri_table[cubeindex][i+1]];
                  p0 = vertlist[mc_tri_table[cubeindex][i+2]];

                  n2 = normlist[mc_tri_table[cubeindex][i  ]];
                  n1 = normlist[mc_tri_table[cubeindex][i+1]];
                  n0 = normlist[mc_tri_table[cubeindex][i+2]];

                  n0 *= -1;
                  n1 *= -1;
                  n2 *= -1;

                }

              ntriang++;

              p0 += volume.m_offset;
              p1 += volume.m_offset;
              p2 += volume.m_offset;

              //emit vertecies
              mesh.vertecies.push_back(p0[0]);
              mesh.vertecies.push_back(p0[1]);
              mesh.vertecies.push_back(p0[2]);

              mesh.vertecies.push_back(p1[0]);
              mesh.vertecies.push_back(p1[1]);
              mesh.vertecies.push_back(p1[2]);

              mesh.vertecies.push_back(p2[0]);
              mesh.vertecies.push_back(p2[1]);
              mesh.vertecies.push_back(p2[2]);
              //emit normals

              //n_ot = an.normalized();
              //n_ot = (p1-p0).cross(p2-p0).normalized();
              //              vector3<float> n0 = p0.normalized();
              //              vector3<float> n1 = p1.normalized();
              //              vector3<float> n2 = p2.normalized();

              mesh.normals.push_back(n0[0]);
              mesh.normals.push_back(n0[1]);
              mesh.normals.push_back(n0[2]);

              mesh.normals.push_back(n1[0]);
              mesh.normals.push_back(n1[1]);
              mesh.normals.push_back(n1[2]);

              mesh.normals.push_back(n2[0]);
              mesh.normals.push_back(n2[1]);
              mesh.normals.push_back(n2[2]);

              //emit indices
              mesh.indices.push_back(num_idx);
              mesh.indices.push_back(num_idx + 1);
              mesh.indices.push_back(num_idx + 2);

              num_idx += 3;
            }

        }

  mesh.num_primitives = mesh.indices.size()*3;
  mesh.bind_data();

}


vector3<float> volume_helper::get_vertex_normal_from_cube(const int ix,
                                                          const int iy,
                                                          const int iz,
                                                          scalar_volume_t<float> &volume,
                                                          int &addr_mode) {
  vector3<float> res;

  if (addr_mode == 0) {
      res[0] = get_field_value_at(ix-1, iy, iz, volume) -
               get_field_value_at(ix+1, iy, iz, volume);
      res[1] = get_field_value_at(ix, iy-1, iz, volume) -
               get_field_value_at(ix, iy+1, iz, volume);
      res[2] = get_field_value_at(ix, iy, iz-1, volume) -
               get_field_value_at(ix, iy, iz+1, volume);
    } else {
      res[0] = get_field_value_at_v2(ix-1, iy, iz, volume) -
               get_field_value_at_v2(ix+1, iy, iz, volume);
      res[1] = get_field_value_at_v2(ix, iy-1, iz, volume) -
               get_field_value_at_v2(ix, iy+1, iz, volume);
      res[2] = get_field_value_at_v2(ix, iy, iz-1, volume) -
               get_field_value_at_v2(ix, iy, iz+1, volume);
    }

  return res;
}

float volume_helper::get_value_from_cube_interpolated(vector3<float> &pos,
                                                      scalar_volume_t<float> &volume,
                                                      int &addr_mode) {
  periodic_cell<float> cube_cell(3);
  for (int i = 0 ; i < 3; i++) cube_cell.v[i] = volume.m_axis[i] * volume.m_steps[i];

  vector3<float> pos_f = cube_cell.cart2frac(pos);

  if (cube_cell.within_already_frac(pos_f)) {
      int cube_a = std::trunc(pos_f[0] * volume.m_steps[0]);
      int cube_b = std::trunc(pos_f[1] * volume.m_steps[1]);
      int cube_c = std::trunc(pos_f[2] * volume.m_steps[2]);

      float acc{0};
      for (int i = -1; i < 2; i++)
        for (int j = -1; j < 2; j++)
          for (int k = -1; k < 2; k++) {
              int _i = std::clamp<int>(cube_a + i, 0, volume.m_steps[0]);
              int _j = std::clamp<int>(cube_b + j, 0, volume.m_steps[1]);
              int _k = std::clamp<int>(cube_c + k, 0, volume.m_steps[2]);
              acc += get_field_value_at(_i, _j, _k, volume);
            }
      return acc / 27.0f;

    } else {
      return 0.0f;
    }
}

vector3<float> volume_helper::vertex_interp(float isolevel,
                                            vector3<float> &p1,
                                            vector3<float> &p2,
                                            float val_p1,
                                            float val_p2) {

  if (std::abs(isolevel - val_p1) < 0.00005f) return p1;
  if (std::abs(isolevel - val_p2) < 0.00005f) return p2;
  if (std::abs(val_p1 - val_p2) < 0.00005f) return p1;

  float mu = (isolevel - val_p1) / (val_p2 - val_p1);

  return p1 + mu * (p2 - p1);

}
