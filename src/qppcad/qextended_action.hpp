#ifndef QPP_CAD_EXTENDED_QACTION
#define QPP_CAD_EXTENDED_QACTION

#include <QAction>

namespace qpp {

  namespace cad {


    class qextended_action : public QAction {
        Q_OBJECT
      public:
        std::array<size_t,2> m_joined_data{0,0};
        explicit qextended_action(QObject *parent = 0);
    };

  }

}


#endif
