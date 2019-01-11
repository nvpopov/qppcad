#ifndef QPP_CAD_FILE_FORMATS
#define QPP_CAD_FILE_FORMATS
#include <string>

namespace qpp{

  namespace cad {

    enum qc_file_fmt : int {
      standart_xyz, //0
      vasp_poscar,  //1
      vasp_outcar_md, //2
      firefly_output, //3
      firefly_input, //4
      cp2k_output, //5
      cp2k_coord_cell_section, //6
      qppcad_json, //7
      qpp_uc, //8
      cube, //9
      unknown_fileformat
    };

    struct qc_file_fmt_helper {

        static bool need_to_auto_center(qc_file_fmt file_format){
          if (file_format == qc_file_fmt::standart_xyz) return true;
          return false;
        }

        static qc_file_fmt file_format_from_string(const std::string &fn) {
          //qppcad json

          if (fn.find("json") != std::string::npos) {
              return qc_file_fmt::qppcad_json;
            }

          //XYZ
          if (fn.find("xyz") != std::string::npos) {
              return qc_file_fmt::standart_xyz;
            }

          //VASP POSCAR
          if (fn.find("poscar") != std::string::npos || fn.find("vasp") != std::string::npos) {
              return qc_file_fmt::vasp_poscar;
            }

          //VASP OUTCAR
          if (fn.find("outcar") != std::string::npos) {
              return qc_file_fmt::vasp_outcar_md;
            }

          if (fn.find("cp2k") != std::string::npos) {
              return qc_file_fmt::cp2k_output;
            }

          if (fn.find("uc") != std::string::npos) {
              return qc_file_fmt::qpp_uc;
            }

          if (fn.find(".cube") != std::string::npos) {
              return qc_file_fmt::cube;
            }


          return qc_file_fmt::unknown_fileformat;

        }

        static qc_file_fmt file_name_to_file_format(const std::string &fn) {

          //qppcad json

          if (fn.find("json") != std::string::npos) {
              return qc_file_fmt::qppcad_json;
            }

          //XYZ
          if (fn.find(".XYZ") != std::string::npos || fn.find(".xyz") != std::string::npos) {
              return qc_file_fmt::standart_xyz;
            }

          //VASP POSCAR
          if (fn.find("POSCAR") != std::string::npos || fn.find(".poscar") != std::string::npos ||
              fn.find(".vasp") != std::string::npos || fn.find(".VASP") != std::string::npos) {
              return qc_file_fmt::vasp_poscar;
            }

          //VASP OUTCAR
          if (fn.find("OUTCAR") != std::string::npos) {
              return qc_file_fmt::vasp_outcar_md;
            }

          if (fn.find(".uc") != std::string::npos) {
              return qc_file_fmt::qpp_uc;
            }

          if (fn.find(".cube") != std::string::npos) {
              return qc_file_fmt::cube;
            }

          return qc_file_fmt::unknown_fileformat;
        }

    };
  }

}

#endif
