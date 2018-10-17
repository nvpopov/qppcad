#ifndef QPP_CAD_FILE_FORMATS
#define QPP_CAD_FILE_FORMATS

namespace qpp{

  namespace cad {

    enum qc_file_fmt{
      standart_xyz,
      advanced_xyz,
      multi_frame_xyz,
      vasp_poscar,
      vasp_outcar_md,
      cp2k_coord_cell_section
    };

    struct qc_file_fmt_helper {

        static bool need_to_auto_center(qc_file_fmt file_format){
          if (file_format == qc_file_fmt::multi_frame_xyz ||
              file_format == qc_file_fmt::standart_xyz) return true;
          return false;
        }

    };
  }

}

#endif
