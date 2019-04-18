#ifndef QPPCAD_QBINDED_INPUTS
#define QPPCAD_QBINDED_INPUTS

#include <qppcad/qppcad.hpp>
#include <QWidget>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <geom/xgeom.hpp>
#include <qppcad/ws_item/ws_item.hpp>

namespace qpp {

  namespace cad {

    template<typename T>
    class generic_binded_input_t {

      public:

        bool m_ignore_state_change{false};
        bool m_updated_externally_event{false};
        T *m_binded_value{nullptr};
        ws_item_t *m_binded_ws_item{nullptr};
        uint32_t m_upd_flag{ws_item_updf_generic};

        void bind_value(T *_binded_value, ws_item_t *item_to_bind = nullptr) {
          m_binded_value = _binded_value;
          m_binded_ws_item = item_to_bind;
          m_ignore_state_change = true;
          if (m_binded_value) load_value_ex();
          m_ignore_state_change = false;
        }

        void unbind_value() {
          m_binded_value = nullptr;
          m_binded_ws_item = nullptr;
        }

        void on_value_changed() {
          if (m_binded_ws_item && m_updated_externally_event)
            m_binded_ws_item->updated_externally(m_upd_flag);
        }

        virtual void load_value_ex() = 0;

    };

    class qbinded_checkbox_t : public QCheckBox, public generic_binded_input_t<bool> {

        Q_OBJECT

      public:

        void load_value_ex() override;
        qbinded_checkbox_t(QWidget *parent = nullptr);

      public slots:

        void check_state_changed(int state);
    };

    class qbinded_line_edit_t : public QLineEdit, public generic_binded_input_t<std::string> {

        Q_OBJECT

      public:

        void load_value_ex() override;
        qbinded_line_edit_t(QWidget *parent = nullptr);

      public slots:

        void editing_finished();

    };

    class qbinded_float_spinbox_t : public QDoubleSpinBox, public generic_binded_input_t<float> {

        Q_OBJECT

      public:

        void load_value_ex() override;
        void set_min_max_step(double new_min,
                              double new_max,
                              double new_step,
                              int decimals = 2);
        qbinded_float_spinbox_t(QWidget *parent = nullptr);
        void set_suffix(QString &new_suffix);
        void set_default_suffix();

      public slots:

        void value_changed(double d);

    };

    class qbinded_int_spinbox_t : public QSpinBox, public generic_binded_input_t<int> {

        Q_OBJECT

      public:

        void load_value_ex() override;
        void set_min_max_step(int new_min,
                              int new_max,
                              int new_step);
        qbinded_int_spinbox_t(QWidget *parent = nullptr);
        void set_suffix(QString &new_suffix);
        void set_default_suffix();

      public slots:

        void value_changed(int value);

    };

    class qbinded_combobox_t : public QComboBox, public generic_binded_input_t<int> {

        Q_OBJECT

      public:

        void load_value_ex() override;
        qbinded_combobox_t(QWidget *parent = nullptr);

      public slots:

        void value_changed(int i);

    };

    class qbinded_int3_input_t : public QWidget, public generic_binded_input_t<vector3<int> > {

        Q_OBJECT

      public:

        QHBoxLayout *widget_layout;
        QSpinBox *sb_x;
        QSpinBox *sb_y;
        QSpinBox *sb_z;
        void load_value_ex() override;
        void set_min_max_step(int min, int max, int step);
        qbinded_int3_input_t(QWidget *parent = nullptr);

      private slots:

        void spinbox_value_changed(int newval);

    };

    class qbinded_int2b_input_t : public QWidget, public generic_binded_input_t<vector3<int> > {

        Q_OBJECT

      public:

        QHBoxLayout *widget_layout;
        QSpinBox *sb_x;
        QSpinBox *sb_y;
        void load_value_ex() override;
        qbinded_int2b_input_t(QWidget *parent = nullptr);

      private slots:

        void spinbox_value_changed(int newval);

    };

    class qbinded_float3_input_t : public QWidget, public generic_binded_input_t<vector3<float> > {

        Q_OBJECT

      public:

        QHBoxLayout *widget_layout;

        QDoubleSpinBox *sb_x;
        QDoubleSpinBox *sb_y;
        QDoubleSpinBox *sb_z;

        void load_value_ex() override;
        void set_min_max_step(double min, double max, double step);
        void set_suffix(QString &new_suffix);
        void set_empty_suffix();
        void set_default_suffix();
        qbinded_float3_input_t(QWidget *parent = nullptr);

      private slots:

        void spinbox_value_changed(double newval);

    };

    class qbinded_color3_input_t : public QWidget,
        public generic_binded_input_t<vector3<float> > {

        Q_OBJECT

      public:

        QColor m_stored_color;
        explicit qbinded_color3_input_t(QWidget *parent = nullptr);
        void load_value_ex() override;
        void mousePressEvent(QMouseEvent *event) override;

      protected:
          void paintEvent(QPaintEvent *event) override;

    };


    class qbinded_xgeom_color3_input_t : public QFrame {

        Q_OBJECT

      public:

        xgeometry<float, periodic_cell<float>> *m_binded_xgeom{nullptr};
        std::array<int, 3> m_binding_indicies;
        size_t m_binded_atom_id;
        QColor m_stored_color;
        bool m_apply_to_selected{false};
        qbinded_xgeom_color3_input_t(QWidget *parent = nullptr);
        void bind_value(xgeometry<float, periodic_cell<float>> *_binded_xgeom,
                        std::array<int, 3> _binding_indicies,
                        size_t _binded_atom_id);
        void load_value();
        void unbind_value();
        void mousePressEvent(QMouseEvent *event) override;

    };

    class qbinded_xgeom_float_spinbox_t : public QDoubleSpinBox {

        Q_OBJECT

      public:

        xgeometry<float, periodic_cell<float>> *m_binded_xgeom{nullptr};
        int m_binding_index;
        size_t m_binded_atom_id;
        bool m_apply_to_selected{false};
        void bind_value(xgeometry<float, periodic_cell<float>> *_binded_xgeom,
                        int _binding_index,
                        size_t _binded_atom_id);
        void load_value();
        void unbind_value();
        void set_min_max_step(double new_min,
                              double new_max,
                              double new_step,
                              int decimals = 2);
        qbinded_xgeom_float_spinbox_t(QWidget *parent = nullptr);
        void set_suffix(QString &new_suffix);
        void set_default_suffix();

      public slots:

        void value_changed(double d);

    };

    class qbinded_ws_item_combobox_t : public QComboBox {

        Q_OBJECT

      public:

        std::shared_ptr<ws_item_t> *m_binded_ws_item_p{nullptr};
        ws_item_t *m_binded_ws_item{nullptr};
        workspace_t *m_binded_ws{nullptr};
        size_t m_type_id{0};
        bool m_updated_internally_event{false};
        uint32_t m_upd_flag{ws_item_updf_generic};

        qbinded_ws_item_combobox_t(QWidget *parent = nullptr);

        void bind_value(std::shared_ptr<ws_item_t> *_binded_value,
                        ws_item_t *item_to_bind = nullptr);
        void load_value();
        void unbind_value();
        void rebuild_variants();

      public slots:

        void value_changed(int i);

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
