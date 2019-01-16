#ifndef QPP_CAD_WS_ATOMS_LIST_IO_XYZ
#define QPP_CAD_WS_ATOMS_LIST_IO_XYZ

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_atoms_list/ws_atoms_list.hpp>
#include <qppcad/ws_item_behaviour_manager.hpp>

namespace qpp {

  namespace cad {

    class ws_atoms_list_io_xyz_t : public ws_item_io_inherited_bhv_t<ws_atoms_list_t> {
      public:
        bool can_save() override;
        bool can_load() override;

        void load_from_stream_ex(std::basic_istream<CHAR,TRAITS> &stream,
                                 ws_atoms_list_t *_item) override;

        void save_to_stream_ex(std::basic_ostream<CHAR,TRAITS> &stream,
                               ws_atoms_list_t *_item) override;

        bool deduce_from_file_name(std::string &file_name) override;
    };

  }

}

#endif
