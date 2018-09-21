#ifndef QPP_CAD_FILE_FORMATS
#define QPP_CAD_FILE_FORMATS

namespace qpp{

  enum qc_file_format{
    format_standart_xyz   = 0,
    format_advanced_xyz   = 1,
    format_vasp_poscar    = 2,
    format_vasp_outcar_md = 3
  };

}

#endif
