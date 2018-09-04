#ifndef QPP_CAD_FILE_DIALOG
#define QPP_CAD_FILE_DIALOG

#include <qppcad/qppcad.hpp>
#include <imgui.h>
#include <imgui_internal.h>

namespace qpp {

  ///
  /// \brief The file_dialog_kind enum
  ///
  enum file_dialog_kind {
    save_file_dialog,
    open_file_dialog,
    select_dir_dialog
  };

  const std::string file_dialog_headers[3]  = {
    "Save file",
    "Open file",
    "Select directory"
  };

  ///
  /// \brief The file_dialog_t class
  ///
  class file_dialog_t {
    public:

      std::vector<std::string> dialog_file_ext;
      std::vector<std::string> current_dir_ls;
      std::string              current_dir;
      file_dialog_kind kind;
      bool activated;

      ///
      /// \brief file_dialog_t
      /// \param activate_when_created
      ///
      file_dialog_t(bool activate_when_created = false){
        activated = activate_when_created;
      }
      void require_dialog(file_dialog_kind kind_of_dialog);
      void render();
  };
}

#endif
