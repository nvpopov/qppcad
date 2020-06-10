#ifndef HISTORY_STREAM_XGEOM_PROXY
#define HISTORY_STREAM_XGEOM_PROXY

#include <qppcad/core/history_stream.hpp>
#include <geom/xgeom.hpp>
#include <cassert>

namespace qpp {

namespace cad {

template<typename REAL, typename CELL>
class hist_doc_xgeom_proxy_t : public hist_doc_base_t,
                               public geometry_observer<REAL> {

private:

  xgeometry<REAL, CELL> *p_xgeom;

public:



};

}

}

#endif
