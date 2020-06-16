#ifndef QPPCAD_QBINDED_INPUTS
#define QPPCAD_QBINDED_INPUTS

#include <qppcad/core/qppcad.hpp>
#include <QWidget>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <geom/xgeom.hpp>
#include <qppcad/ws_item/ws_item.hpp>

namespace qpp {

namespace cad {

template<typename T>
class generic_binded_input_t {

public:

  hs_doc_t<T> *m_binded_value{nullptr};
  iupdatable_externally_t *m_binded_item{nullptr};
  uint32_t m_upd_flag{ws_item_updf_generic};

  bool m_ignore_state_change{false};
  bool m_updated_externally_event{false};

  void bind_value(hs_doc_t<T> *binded_value, iupdatable_externally_t *item_to_bind = nullptr) {

    m_binded_value = binded_value;
    m_binded_item = item_to_bind;
    m_ignore_state_change = true;
    if (m_binded_value) load_value_ex();
    m_ignore_state_change = false;

  }

  void set_value(T&& new_value) {
    if (!is_binded()) return;
    auto val(new_value);
    m_binded_value->set_value(std::move(new_value));
  }

  std::optional<T> get_value() {
    if (!is_binded()) return std::nullopt;
    return m_binded_value->get_value();
  }

  bool is_binded() { return m_binded_value;}
  bool is_item_binded() { return m_binded_item;}

  void unbind_value() {
    m_binded_value = nullptr;
    m_binded_item = nullptr;
  }

  void on_value_changed() {
    if (m_binded_item && m_updated_externally_event) m_binded_item->updated_externally(m_upd_flag);
  }

  virtual void load_value_ex() = 0;

};

/**
 * @brief The qbinded_checkbox_t class
 */
class qbinded_checkbox_t : public QCheckBox,
                           public generic_binded_input_t<bool> {

  Q_OBJECT

public:

  void load_value_ex() override;
  explicit qbinded_checkbox_t(QWidget *parent = nullptr);

public slots:

  void check_state_changed(int state);

};

/**
 * @brief The qbinded_line_edit_t class
 */
class qbinded_line_edit_t : public QLineEdit,
                            public generic_binded_input_t<std::string> {

  Q_OBJECT

public:

  void load_value_ex() override;
  explicit qbinded_line_edit_t(QWidget *parent = nullptr);

public slots:

  void editing_finished();

};

/**
 * @brief The qbinded_float_spinbox_t class
 */
class qbinded_float_spinbox_t : public QDoubleSpinBox,
                                public generic_binded_input_t<float> {

  Q_OBJECT

public:

  void load_value_ex() override;
  void set_min_max_step(double new_min, double new_max, double new_step, int decimals = 2);
  explicit qbinded_float_spinbox_t(QWidget *parent = nullptr);
  void set_suffix(QString &new_suffix);
  void set_default_suffix();

public slots:

  void value_changed(double d);

};

/**
 * @brief The qbinded_int_spinbox_t class
 */
class qbinded_int_spinbox_t : public QSpinBox,
                              public generic_binded_input_t<int> {

  Q_OBJECT

public:

  void load_value_ex() override;
  void set_min_max_step(int new_min, int new_max, int new_step);
  explicit qbinded_int_spinbox_t(QWidget *parent = nullptr);
  void set_suffix(QString &new_suffix);
  void set_default_suffix();

public slots:

  void value_changed(int value);

};

/**
 * @brief The qbinded_combobox_t class
 */
class qbinded_combobox_t : public QComboBox,
                           public generic_binded_input_t<int> {

  Q_OBJECT

public:

  void load_value_ex() override;

  explicit qbinded_combobox_t(QWidget *parent = nullptr);

public slots:

  void value_changed(int i);

};

/**
 * @brief The qbinded_int3_input_t class
 */
class qbinded_int3_input_t : public QWidget,
                             public generic_binded_input_t<vector3<int> > {

  Q_OBJECT

public:

  QHBoxLayout *widget_layout;

  QSpinBox *sb_x;
  QSpinBox *sb_y;
  QSpinBox *sb_z;

  void load_value_ex() override;
  void set_min_max_step(int min, int max, int step);
  void set_min_max_step(vector3<int> min, vector3<int> max, vector3<int> step);

  explicit qbinded_int3_input_t(QWidget *parent = nullptr);

private slots:

  void spinbox_value_changed(int newval);

};

/**
 * @brief The qbinded_int2b_input_t class
 */
class qbinded_int2b_input_t : public QWidget,
                              public generic_binded_input_t<vector3<int> > {

  Q_OBJECT

public:

  QHBoxLayout *widget_layout;

  QSpinBox *sb_x;
  QSpinBox *sb_y;

  void set_min_max_step(int min, int max, int step);
  void load_value_ex() override;
  explicit qbinded_int2b_input_t(QWidget *parent = nullptr);

private slots:

  void spinbox_value_changed(int newval);

};

/**
 * @brief The qbinded_float2_input_t class
 */
class qbinded_float2_input_t : public QWidget,
                               public generic_binded_input_t<vector2<float> > {

  Q_OBJECT

public:

  QHBoxLayout *widget_layout;

  QDoubleSpinBox *sb_x;
  QDoubleSpinBox *sb_y;

  void load_value_ex() override;
  void set_min_max_step(double min, double max, double step);
  void set_suffix(QString &new_suffix);
  void set_empty_suffix();
  void set_default_suffix();
  explicit qbinded_float2_input_t(QWidget *parent = nullptr);

private slots:

  void spinbox_value_changed(double newval);

};

/**
 * @brief The qbinded_float3_input_t class
 */
class qbinded_float3_input_t : public QWidget,
                               public generic_binded_input_t<vector3<float> > {

  Q_OBJECT

public:

  QHBoxLayout *widget_layout;

  QDoubleSpinBox *sb_x;
  QDoubleSpinBox *sb_y;
  QDoubleSpinBox *sb_z;

  void load_value_ex() override;
  void set_min_max_step_dec(double min, double max, double step,
                            std::optional<int> dec = std::nullopt);
  void set_suffix(QString &new_suffix);
  void set_empty_suffix();
  void set_default_suffix();
  void set_size(int new_size);
  explicit qbinded_float3_input_t(QWidget *parent = nullptr);

private slots:

  void spinbox_value_changed(double newval);

};

/**
 * @brief The qbinded_matrix3_input_t class
 */
class qbinded_matrix3_input_t : public QWidget,
                                public generic_binded_input_t<matrix3<float> > {

  Q_OBJECT

public:

  std::vector<QDoubleSpinBox*> m_sbx;
  std::map<QDoubleSpinBox*, std::tuple<size_t, size_t> > m_sbx_lookup;
  QGridLayout *m_main_lt;

  void load_value_ex() override;
  void set_min_max_step(double min, double max, double step);
  void set_suffix(QString &new_suffix);
  void set_empty_suffix();
  void set_default_suffix();

  explicit qbinded_matrix3_input_t(QWidget *parent = nullptr);

private slots:

  void spinbox_value_changed(double newval);

};

/**
 * @brief The qbinded_color3_input_t class
 */
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

/**
 * @brief The qbinded_xgeom_color3_input_t class
 */
class qbinded_xgeom_color3_input_t : public QFrame {

  Q_OBJECT

public:

  xgeometry<float, periodic_cell<float>> *m_binded_xgeom{nullptr};
  std::array<int, 3> m_binding_indicies;
  std::set<size_t> m_binded_atom_id;
  QColor m_stored_color;
  bool m_apply_to_selected{false};

  explicit qbinded_xgeom_color3_input_t(QWidget *parent = nullptr);
  void bind_value(xgeometry<float, periodic_cell<float> > *binded_xgeom,
                  std::array<int, 3> binding_indicies,
                  std::set<size_t> &&binded_atom_id);
  void load_value();
  void unbind_value();
  void mousePressEvent(QMouseEvent *event) override;

};

/**
 * @brief The qbinded_xgeom_float_spinbox_t class
 */
class qbinded_xgeom_float_spinbox_t : public QDoubleSpinBox {

  Q_OBJECT

public:

  xgeometry<float, periodic_cell<float> > *m_binded_xgeom{nullptr};
  int m_binding_index;
  std::set<size_t> m_binded_atom_id;
  bool m_apply_to_selected{false};

  void bind_value(xgeometry<float, periodic_cell<float> > *binded_xgeom,
                  int binding_index,
                  std::set<size_t> &&binded_atom_id);

  void load_value();
  void unbind_value();
  void set_min_max_step(double new_min, double new_max, double new_step, int decimals = 2);
  explicit qbinded_xgeom_float_spinbox_t(QWidget *parent = nullptr);
  void set_suffix(QString &new_suffix);
  void set_default_suffix();

public slots:

  void value_changed(double d);

};

/**
 * @brief The qbinded_ws_item_combobox_t class
 */
class qbinded_ws_item_combobox_t : public QComboBox {

  Q_OBJECT

public:

  std::shared_ptr<ws_item_t> *m_binded_ws_item_p{nullptr};
  iupdatable_externally_t *m_binded_item{nullptr};
  workspace_t *m_binded_ws{nullptr};
  size_t m_type_id{0};
  bool m_allow_other_ws{false};
  bool m_updated_externally_event{false};
  uint32_t m_upd_flag{ws_item_updf_generic};

  explicit qbinded_ws_item_combobox_t(QWidget *parent = nullptr);

  void bind_value(std::shared_ptr<ws_item_t> *_binded_value,
                  iupdatable_externally_t *item_to_bind = nullptr,
                  workspace_t *binded_ws = nullptr);
  void load_value();
  void unbind_value();
  void rebuild_variants();

public slots:

  void value_changed(int i);

};

/**
 * @brief The qbinded_bool_named_vector_t class
 */
class qbinded_bool_named_vector_t : public QFrame {

  Q_OBJECT

public:

  std::vector<hs_prop_bool_t*> m_binded_data;
  std::vector<QString> m_binded_names;
  QHBoxLayout *widget_layout;
  std::vector<QCheckBox*> m_boxes;
  std::vector<QLabel*> m_labels;

  explicit qbinded_bool_named_vector_t(std::vector<QString> &&names, QWidget *parent = nullptr);
  void bind_value(std::vector<hs_prop_bool_t*> &&binded_data);
  void load_value();
  void unbind_value();

};

/**
 * @brief The qbinded_float_named_vector_t class
 */
class qbinded_float_named_vector_t : public QFrame {

  Q_OBJECT

public:

  iupdatable_externally_t *m_binded_item{nullptr};
  std::vector<uint32_t> m_upd_flag{ws_item_updf_generic};

  bool m_ignore_state_change{false};
  std::vector<bool> m_updated_externally_event{false};

  std::vector<hs_prop_float_t*> m_binded_data;
  std::vector<QString> m_binded_names;
  QHBoxLayout *widget_layout;
  std::vector<QDoubleSpinBox*> m_boxes;
  std::vector<QLabel*> m_labels;

  explicit qbinded_float_named_vector_t(std::vector<QString> &&names, QWidget *parent = nullptr);

  void set_min_max_step(float vmin, float vmax, float vstep,
                        std::optional<int> decs = std::nullopt);

  void set_min_max_step_ex(std::vector<float> &&vmin, std::vector<float> &&vmax,
                           std::vector<float> &&vstep, std::vector<std::optional<int>> &&decs);

  void bind_value(std::vector<hs_prop_float_t*> &&binded_data,
                  iupdatable_externally_t *item_to_bind = nullptr);

  void load_value();
  void unbind_value();

};

} // namespace qpp::cad

} // namespace qpp

#endif
