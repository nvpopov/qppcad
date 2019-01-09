#include <qppcad/marching_cubes.hpp>

using namespace qpp;
using namespace qpp::cad;


void marching_cubes_helper::polygonise_volume_mc(mesh_t &mesh,
                                                 cube_header_t<float> &ch,
                                                 std::vector<float> &field,
                                                 float isolevel) {

  //iterate over all sub-cubes
  for (int ix = 0; ix < ch.steps[0] - 1; ix++)
    for (int iy = 0; iy < ch.steps[1] - 1; iy++)
      for (int iz = 0; iz < ch.steps[2] - 2; iz++) {

          //restore cube coords 0,0 - top
          //for ref http://paulbourke.net/geometry/polygonise/polygonise1.gif
          vector3<float> gp0(ix * ch.axis[0][0], iy * ch.axis[1][1], ix * ch.axis[0][0]); //0 0 0
          vector3<float> gp1 = gp0 + ch.axis[0]; // +1  0 0
          vector3<float> gp2 = gp1 + ch.axis[1]; // +1 +1 0
          vector3<float> gp3 = gp0 + ch.axis[1]; //  0 +1 0

          vector3<float> gp4 = gp0 + ch.axis[2]; //  0  0 +1
          vector3<float> gp5 = gp4 + ch.axis[0]; // +1  0 +1
          vector3<float> gp6 = gp5 + ch.axis[1]; // +1 +1 +1
          vector3<float> gp7 = gp4 + ch.axis[1]; //  0 +1 +1

          //          int i,ntriang;
          //          int cubeindex;
          //          XYZ vertlist[12];

          std::array<float, 8> gv;
          gv[0] = get_field_value_at(ix,     iy,     iz, ch.steps[0], ch.steps[1], field);
          gv[1] = get_field_value_at(ix + 1, iy,     iz, ch.steps[0], ch.steps[1], field);
          gv[2] = get_field_value_at(ix + 1, iy + 1, iz, ch.steps[0], ch.steps[1], field);
          gv[3] = get_field_value_at(ix,     iy + 1, iz, ch.steps[0], ch.steps[1], field);
          gv[4] = get_field_value_at(ix,     iy,     iz + 1, ch.steps[0], ch.steps[1], field);
          gv[5] = get_field_value_at(ix + 1, iy,     iz + 1, ch.steps[0], ch.steps[1], field);
          gv[6] = get_field_value_at(ix + 1, iy + 1, iz + 1, ch.steps[0], ch.steps[1], field);
          gv[7] = get_field_value_at(ix,     iy + 1, iz + 1, ch.steps[0], ch.steps[1], field);

          int cubeindex = 0;
          if (gv[0] < isolevel) cubeindex |= 1;
          if (gv[1] < isolevel) cubeindex |= 2;
          if (gv[2] < isolevel) cubeindex |= 4;
          if (gv[3] < isolevel) cubeindex |= 8;
          if (gv[4] < isolevel) cubeindex |= 16;
          if (gv[5] < isolevel) cubeindex |= 32;
          if (gv[6] < isolevel) cubeindex |= 64;
          if (gv[7] < isolevel) cubeindex |= 128;

          std::array<vector3<float>, 12> vertlist;

          if (mc_edge_table[cubeindex] == 0) continue;

          if (mc_edge_table[cubeindex] & 1)
            vertlist[0] = vertex_interp(isolevel, gp0, gp1, gv[0], gv[1]);

          if (mc_edge_table[cubeindex] & 2)
            vertlist[1] = vertex_interp(isolevel, gp1, gp2, gv[1], gv[2]);

          if (mc_edge_table[cubeindex] & 4)
            vertlist[2] = vertex_interp(isolevel, gp2, gp3, gv[2], gv[3]);

          if (mc_edge_table[cubeindex] & 8)
            vertlist[3] = vertex_interp(isolevel, gp3, gp0, gv[3], gv[0]);

          if (mc_edge_table[cubeindex] & 16)
            vertlist[4] = vertex_interp(isolevel, gp4, gp5, gv[4], gv[5]);

          if (mc_edge_table[cubeindex] & 32)
            vertlist[5] = vertex_interp(isolevel, gp5, gp6, gv[5], gv[6]);

          if (mc_edge_table[cubeindex] & 64)
            vertlist[6] = vertex_interp(isolevel, gp6, gp7, gv[6], gv[7]);

          if (mc_edge_table[cubeindex] & 128)
            vertlist[7] = vertex_interp(isolevel, gp7, gp4, gv[7], gv[4]);

          if (mc_edge_table[cubeindex] & 256)
            vertlist[8] = vertex_interp(isolevel, gp0, gp4, gv[0], gv[4]);

          if (mc_edge_table[cubeindex] & 512)
            vertlist[9] = vertex_interp(isolevel, gp1, gp5, gv[1], gv[5]);

          if (mc_edge_table[cubeindex] & 1024)
            vertlist[10] = vertex_interp(isolevel, gp2, gp6, gv[2], gv[6]);

          if (mc_edge_table[cubeindex] & 2048)
            vertlist[11] = vertex_interp(isolevel, gp3, gp7, gv[3], gv[7]);

          int ntriang = 0;
          for (int i = 0; mc_tri_table[cubeindex][i] != -1;i += 3) {
              vector3<float> p0 = vertlist[mc_tri_table[cubeindex][i  ]];
              vector3<float> p1 = vertlist[mc_tri_table[cubeindex][i+1]];
              vector3<float> p2 = vertlist[mc_tri_table[cubeindex][i+2]];
              ntriang++;
            }

        }
}

vector3<float> marching_cubes_helper::vertex_interp(float isolevel,
                                                    vector3<float> &p1, vector3<float> &p2,
                                                    float val_p1, float val_p2) {
  //  double mu;
  //  XYZ p;

  //  if (ABS(isolevel-valp1) < 0.00001)
  //     return(p1);
  //  if (ABS(isolevel-valp2) < 0.00001)
  //     return(p2);
  //  if (ABS(valp1-valp2) < 0.00001)
  //     return(p1);
  //  mu = (isolevel - valp1) / (valp2 - valp1);
  //  p.x = p1.x + mu * (p2.x - p1.x);
  //  p.y = p1.y + mu * (p2.y - p1.y);
  //  p.z = p1.z + mu * (p2.z - p1.z);

  //  return(p);
  if (std::abs(isolevel - val_p1) < 0.0001f) return p1;
  if (std::abs(isolevel - val_p2) < 0.0001f) return p2;
  if (std::abs(val_p1 - val_p2) < 0.0001f) return p1;

  float mu = (isolevel - val_p1) / (val_p2 - val_p1);

  return p1 + mu * (p2 - p1);

}
