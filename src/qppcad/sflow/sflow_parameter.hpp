#ifndef QPPCAD_SFLOW_PARAMETER
#define QPPCAD_SFLOW_PARAMETER

#include <qppcad/core/qppcad.hpp>
#include <geom/xgeom.hpp>
#include <qppcad/core/qpp_object.hpp>
#include <qppcad/ws_item/ws_item.hpp>

namespace qpp {

namespace cad {

enum sf_parameter_e : int {
  sfpar_none,
  sfpar_int,
  sfpar_bool,
  sfpar_float,
  sfpar_v3f,
  sfpar_string,
  sfpar_xgeom,
  sfpar_ws_item
};

class sf_parameter_t : public qpp_object_t {

  QPP_OBJECT(sf_parameter_t, qpp_object_t)

public:

  virtual sf_parameter_e get_param_meta();
  virtual std::shared_ptr<sf_parameter_t> clone() = 0;
  explicit sf_parameter_t();

};

class sf_parameter_int_t : public sf_parameter_t {

  QPP_OBJECT(sf_parameter_int_t, sf_parameter_t)

public:

  hs_prop_int_t m_value;
  sf_parameter_e get_param_meta() override ;
  std::shared_ptr<sf_parameter_t> clone() override;

};

class sf_parameter_v3f_t : public sf_parameter_t {

  QPP_OBJECT(sf_parameter_v3f_t, sf_parameter_t)

public:

  hs_prop_v3f_t m_value;
  sf_parameter_e get_param_meta() override ;
  std::shared_ptr<sf_parameter_t> clone() override;

};

class sf_parameter_bool_t : public sf_parameter_t {

  QPP_OBJECT(sf_parameter_bool_t, sf_parameter_t)

public:

  hs_prop_bool_t m_value;
  sf_parameter_e get_param_meta() override ;
  std::shared_ptr<sf_parameter_t> clone() override;

};

class sf_parameter_float_t : public sf_parameter_t {

  QPP_OBJECT(sf_parameter_float_t, sf_parameter_t)

public:

  hs_prop_float_t m_value;
  sf_parameter_e get_param_meta() override ;
  std::shared_ptr<sf_parameter_t> clone() override;

};

class sf_parameter_xgeom_t : public sf_parameter_t {

  QPP_OBJECT(sf_parameter_xgeom_t, sf_parameter_t)

public:

  xgeometry<float, periodic_cell<float> > m_value{3};
  sf_parameter_e get_param_meta() override ;
  std::shared_ptr<sf_parameter_t> clone() override;

};

} // namespace cad

} // namespace qpp

#endif
