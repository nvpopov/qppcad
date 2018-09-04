#include <qppcad/file_dialog.hpp>
#include <qppcad/app.hpp>
#include <nfd.h>

using namespace qpp;


std::string file_dialog_manager_t::request_open_file(std::string &filter, bool &succes){
  nfdchar_t *out_path;
  nfdchar_t *filter_ch = new char[filter.size()+1];
  std::copy(filter.begin(), filter.end(), filter_ch);
  filter_ch[filter.size()] = '\0';
  nfdresult_t result = NFD_OpenDialog( filter_ch, NULL, &out_path );
  if ( result == NFD_OKAY){
      succes = true;
      return std::string(out_path);
    } else if (result == NFD_CANCEL) {

    } else {
      //error
    }

//  delete[] out_path;
//  delete[] filter_ch;

  succes = false;
  return "";
}
