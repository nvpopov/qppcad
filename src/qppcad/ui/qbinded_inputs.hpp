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
#include <qppcad/ws_item/ws_item.hpp>

namespace qpp {

  namespace cad {

    template<typename T>
    class generic_binded_input_t {

      public:
        bool m_ignore_state_change{false};
        bool m_updated_internally_event{false};
        T *m_binded_value{nullptr};
        ws_item_t *m_binded_ws_item{nullptr};

        void bind_value(T *_binded_value, ws_item_t *item_to_bind = nullptr) {
          m_binded_value = _binded_value;
          m_binded_ws_item = item_to_bind;
          m_ignore_state_change = true;
          load_value();
          m_ignore_state_change = false;
        }

        //from model to ui
        void load_value() {
          if (m_binded_value) load_value_ex();
        }

        void unbind_value() {
          m_binded_value = nullptr;
          m_binded_ws_item = nullptr;
        }

        void on_value_changed() {
          if (m_binded_ws_item && m_updated_internally_event)
            m_binded_ws_item->updated_internally();
        }

        virtual void load_value_ex() = 0;
    };

    class qbinded_checkbox : public QCheckBox ,
            public generic_binded_input_t<bool> {

        Q_OBJECT

      public:
        void load_value_ex() override;
        qbinded_checkbox(QWidget *parent = nullptr);

      public slots:
        void check_state_changed(int state);
    };

    class qbinded_float_spinbox : public QDoubleSpinBox,
            public generic_binded_input_t<float> {

        Q_OBJECT

      public:
        void load_value_ex() override;
        void set_min_max_step(double new_min,
                              double new_max,
                              double new_step,
                              int decimals = 2);
        qbinded_float_spinbox(QWidget *parent = nullptr);
        void set_suffix(QString &new_suffix);
        void set_default_suffix();
      public slots:
        void value_changed(double d);

    };

    class qbinded_int_spinbox : public QSpinBox,
            public generic_binded_input_t<int> {

        Q_OBJECT

      public:
        void load_value_ex() override;
        void set_min_max_step(int new_min,
                              int new_max,
                              int new_step);
        qbinded_int_spinbox(QWidget *parent = nullptr);
        void set_suffix(QString &new_suffix);
        void set_default_suffix();
      public slots:
        void value_changed(int value);

    };

    class qbinded_combobox : public QComboBox,
            public generic_binded_input_t<int> {

        Q_OBJECT

      public:
        void load_value_ex() override;
        qbinded_combobox(QWidget *parent = nullptr);

      public slots:
        void value_changed(int i);
    };

    class qbinded_int3_input : public QWidget,
            public generic_binded_input_t<vector3<int> > {

        Q_OBJECT

      public:
        QHBoxLayout *widget_layout;
        QSpinBox *sb_x;
        QSpinBox *sb_y;
        QSpinBox *sb_z;
        void load_value_ex() override;
        void set_min_max_step(int min, int max, int step);
        qbinded_int3_input(QWidget *parent = nullptr);

      private slots:
        void spinbox_value_changed(int newval);

    };

    class qbinded_float3_input : public QWidget,
            public generic_binded_input_t<vector3<float> > {

        Q_OBJECT

      public:
        QHBoxLayout *widget_layout;
        QDoubleSpinBox *sb_x;
        QDoubleSpinBox *sb_y;
        QDoubleSpinBox *sb_z;
        void load_value_ex() override;
        void set_min_max_step(double min, double max, double step);
        void set_suffix(QString &new_suffix);
        void set_default_suffix();
        qbinded_float3_input(QWidget *parent = nullptr);

      private slots:
        void spinbox_value_changed(double newval);

    };

    class qbinded_color3_input : public QWidget,
            public generic_binded_input_t<vector3<float> > {

        Q_OBJECT

      public:
        QColor m_stored_color;
        qbinded_color3_input(QWidget *parent = nullptr);
        void load_value_ex() override;
        void mousePressEvent(QMouseEvent *event) override;

      private slots:

    };


    class qbinded_ws_item_combobox : public QComboBox {

        Q_OBJECT

      public:

        std::shared_ptr<ws_item_t> *m_binded_ws_item_p{nullptr};
        ws_item_t *m_binded_ws_item{nullptr};
        workspace_t *m_binded_ws{nullptr};
        size_t m_type_id{0};
        qbinded_ws_item_combobox(QWidget *parent = nullptr);

        void bind_value(std::shared_ptr<ws_item_t> *_binded_value,
                        ws_item_t *item_to_bind = nullptr);
        void load_value();
        void unbind_value();

    };


  }

}

#endif
