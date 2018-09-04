#ifndef QPP_CAD_FILE_DIALOG
#define QPP_CAD_FILE_DIALOG

#include <qppcad/qppcad.hpp>
#include <imgui.h>
#include <imgui_internal.h>

namespace qpp {


  const std::string file_dialog_headers[3]  = {
    "Save file",
    "Open file",
    "Select directory"
  };

  ///
  /// \brief The file_dialog_t class
  ///
  class file_dialog_manager_t {
    public:
      ///
      /// \brief file_dialog_t
      /// \param activate_when_created
      ///
      file_dialog_manager_t(){

      }

      std::string request_open_file(std::string &filter, bool &succes);
  };
}

#endif
