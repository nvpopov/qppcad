#ifndef QPP_CAD_QBINDED_INPUTS
#define QPP_CAD_QBINDED_INPUTS

#include <qppcad/qppcad.hpp>
#include <QWidget>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>

namespace qpp {

  namespace cad {

    class qbinded_checkbox : public QCheckBox {

        Q_OBJECT

      public:
        bool m_ignore_state_change{false};
        bool *m_binded_value{nullptr};
        void bind_value(bool *_binded_value);
        void load_value();
        void unbind_value();
        qbinded_checkbox(QWidget *parent = nullptr);

      public slots:
        void check_state_changed(int state);
    };

    class qbinded_spinbox : public QSpinBox {

        Q_OBJECT

      public:
        int *m_binded_value{nullptr};
        qbinded_spinbox(QWidget *parent = nullptr);

    };

    class qbinded_float_spinbox : public QDoubleSpinBox {

        Q_OBJECT

      public:
        float *m_binded_value{nullptr};
        bool m_ignore_state_change{false};
        void bind_value(float *_binded_value);
        void load_value();
        void unbind_value();
        void set_min_max_step(double new_min, double new_max, double new_step);
        qbinded_float_spinbox(QWidget *parent = nullptr);

      public slots:
        void value_changed(double d);

    };

    class qbinded_combobox : public QComboBox {

        Q_OBJECT

      public:
        int *m_binded_value{nullptr};
        bool m_ignore_state_change{false};
        void bind_value(int *_binded_value);
        void load_value();
        void unbind_value();
        qbinded_combobox(QWidget *parent = nullptr);

      public slots:
        void value_changed(int i);
    };

    class qbinded_int3_input : public QWidget {

         Q_OBJECT

       public:

        vector3<int> *m_binded_value{nullptr};

        QHBoxLayout *widget_layout;
        QSpinBox *sb_x;
        QSpinBox *sb_y;
        QSpinBox *sb_z;

        bool m_ignore_state_change{false};

        void bind_value(vector3<int> *_binded_value);
        void load_value();
        void unbind_value();
        void set_min_max_step(int min, int max, int step);

        qbinded_int3_input(QWidget *parent = nullptr);

      private slots:
        void spinbox_value_changed(int newval);

    };





  }

}

#endif
