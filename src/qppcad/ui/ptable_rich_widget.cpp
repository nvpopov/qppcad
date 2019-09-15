#include <qppcad/ui/ptable_rich_widget.hpp>
#include <data/ptable.hpp>

using namespace qpp;
using namespace qpp::cad;

ptable_rich_widget_t::ptable_rich_widget_t(QWidget *parent) : QDialog(parent) {

  setWindowTitle(tr("Periodic table"));

  m_main_lt = new QHBoxLayout;
  m_elem_lt = new QGridLayout;
  m_main_lt->addLayout(m_elem_lt);
  setLayout(m_main_lt);

  construct_widgets();

}

void ptable_rich_widget_t::construct_widgets() {

  ptable *pt_inst = ptable::get_inst();

  for (size_t i = 0; i < PTABLE_ELEM_N; i++) {

      std::optional<std::tuple<int, int> > elem_pos = get_ptable_x_y(i);

      if (!elem_pos) continue;

      QPushButton *tst = new QPushButton;
      tst->setText(QString::fromStdString(pt_inst->arecs[i].m_symbol));
      m_elem_lt->addWidget(tst, std::get<1>(*elem_pos), std::get<0>(*elem_pos));

    }

}

std::optional<std::tuple<int, int> > ptable_rich_widget_t::get_ptable_x_y(size_t elem_id) {

  auto elem = elem_id + 1;

  using pair_int = std::optional<std::tuple<int, int> >;

  if (elem == 1) return pair_int({0, 0});
  if (elem == 2) return pair_int({17, 0});

  if (elem == 3) return pair_int({0, 1});
  if (elem == 4) return pair_int({1, 1});
  if (elem >= 5 && elem <= 10) return pair_int({elem + 7, 1});

  if (elem == 11) return pair_int({0, 2});
  if (elem == 12) return pair_int({1, 2});
  if (elem >= 13 && elem <= 18) return pair_int({elem - 1, 2});
  if (elem >= 19 && elem <= 36) return pair_int({elem - 19, 3});
  if (elem >= 37 && elem <= 54) return pair_int({elem - 37, 4});

  if (elem >= 55 && elem <= 56) return pair_int({elem - 55, 5});
  if (elem >= 72 && elem <= 86) return pair_int({elem - 69, 5});

  if (elem >= 87 && elem <= 88) return pair_int({elem - 87, 6});
  if (elem >= 104 && elem <= 118) return pair_int({elem - 101, 6});

  /* lanthanides */
  if (elem >= 57 && elem <= 71) return pair_int({elem - 55, 8});

  /* actinides */
  if (elem >= 89 && elem <= 103) return pair_int({elem - 87, 9});

  return std::nullopt;

}
