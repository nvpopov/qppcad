#ifndef QPP_CAD_FILE_FORMATS
#define QPP_CAD_FILE_FORMATS
#include <string>

namespace qpp{

  namespace cad {

    enum qc_file_fmt{
      standart_xyz,
      advanced_xyz,
      multi_frame_xyz,
      vasp_poscar,
      vasp_outcar_md,
      firefly_output,
      firefly_input,
      cp2k_coord_cell_section,
      unknown_fileformat
    };

    struct qc_file_fmt_helper {

        static bool need_to_auto_center(qc_file_fmt file_format){
          if (file_format == qc_file_fmt::multi_frame_xyz ||
              file_format == qc_file_fmt::standart_xyz) return true;
          return false;
        }

        static qc_file_fmt file_name_to_file_format(const std::string &fn) {

          //XYZ
          if (fn.find(".XYZ") != std::string::npos || fn.find(".xyz") != std::string::npos) {
              return qc_file_fmt::multi_frame_xyz;
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

          return qc_file_fmt::unknown_fileformat;
        }

    };
  }

}

#endif
