#ifndef QPP_CAD_QSPOILER_WIDGET
#define QPP_CAD_QSPOILER_WIDGET

#include <QFrame>
#include <QGridLayout>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>
#include <qppcad/qppcad.hpp>

namespace qpp {

  namespace cad {

    //stackoverflow.com/questions/32476006/how-to-make-an-expandable-collapsable-section-widget-in-qt
    class qspoiler_widget_t : public QWidget {
        Q_OBJECT
      private:
        QGridLayout mainLayout;
        QToolButton toggleButton;
        QFrame headerLine;
        QParallelAnimationGroup toggleAnimation;
        QScrollArea contentArea;
        int animationDuration{300};
      public:
        explicit qspoiler_widget_t(const QString & title = "",
                                   const int animationDuration = 300,
                                   QWidget *parent = 0);
        void setContentLayout(QLayout & contentLayout);
    };

  }

}

#endif
