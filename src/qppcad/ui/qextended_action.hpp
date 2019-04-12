#ifndef QPPCAD_EXTENDED_QACTION
#define QPPCAD_EXTENDED_QACTION

#include <QAction>
#include <array>

namespace qpp {

  namespace cad {

    class qextended_action : public QAction {
        Q_OBJECT
      public:
        std::array<size_t,2> m_joined_data{0,0};
        explicit qextended_action(QObject *parent = 0);
    };

  } // namespace qpp::cad

} // namespace qpp

#endif
