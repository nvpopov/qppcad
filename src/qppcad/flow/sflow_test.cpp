#define CATCH_CONFIG_MAIN
#include <qppcad/flow/sflow.hpp>
#include <qppcad/qppcad.hpp>
#include <catch.hpp>

using namespace qpp;
using namespace qpp::cad;

TEST_CASE( "sflow base testing" ) {

  SECTION ("sflow |0| (0,0)-> |1| (0,0)-> |2| (0,0)-> |3|, direct flow testing") {

    auto fc = std::make_shared<sflow_context_t>();

    auto n0 = std::make_shared<sflow_soi_node_t>(); n0->m_node_name = "n0";
    auto n1 = std::make_shared<sflow_sgi_node_t>(); n1->m_node_name = "n1";
    auto n2 = std::make_shared<sflow_sgi_node_t>(); n2->m_node_name = "n2";
    auto n3 = std::make_shared<sflow_sii_node_t>(); n3->m_node_name = "n3";

    fc->add_node(n0);
    fc->add_node(n1);
    fc->add_node(n2);
    fc->add_node(n3);

    REQUIRE(fc->connect_node(n0, n1, 0, 0) == sflow_error_e::no_error);
    REQUIRE(fc->connect_node(n1, n2, 0, 0) == sflow_error_e::no_error);
    REQUIRE(fc->connect_node(n2, n3, 0, 0) == sflow_error_e::no_error);

    REQUIRE(n0->m_is_outer);
    REQUIRE(!n1->m_is_outer);
    REQUIRE(!n2->m_is_outer);
    REQUIRE(!n3->m_is_outer);

    fc->execute();

    fc->clear_context();
    REQUIRE(n0->m_is_outer);
    REQUIRE(n1->m_is_outer);
    REQUIRE(n2->m_is_outer);
    REQUIRE(n3->m_is_outer);

  }

  SECTION ("sflow |0|(0) -> (0)|1|  , error for multiple inputs for socket 0"
           "                   ^(0)                                         "
           "      |2|(0)--------                                           ") {

    auto fc = std::make_shared<sflow_context_t>();
    auto n0 = std::make_shared<sflow_soi_node_t>(); n0->m_node_name = "n0";
    auto n1 = std::make_shared<sflow_sgi_node_t>(); n1->m_node_name = "n1";
    auto n2 = std::make_shared<sflow_sgi_node_t>(); n2->m_node_name = "n2";

    fc->add_node(n0);
    fc->add_node(n1);
    fc->add_node(n2);

    REQUIRE(fc->connect_node(n0, n1, 0, 0) == sflow_error_e::no_error);
    REQUIRE(fc->connect_node(n2, n1, 0, 0) == sflow_error_e::too_many_inputs);

    REQUIRE(n0->m_is_outer);
    REQUIRE(!n1->m_is_outer);
    REQUIRE(n2->m_is_outer);

    fc->execute();

    fc->clear_context();
    REQUIRE(n0->m_is_outer);
    REQUIRE(n1->m_is_outer);
    REQUIRE(n2->m_is_outer);

  }

  SECTION ("sflow |0|(0) -> (0)|1|  ,  multiple outputs                      "
           "       |                                                         "
           "       -------> (0)|2|                                           "
           "       |                                                         "
           "       -------> (0)|3|                                           ") {
    auto fc = std::make_shared<sflow_context_t>();
    auto n0 = std::make_shared<sflow_soi_node_t>(); n0->m_node_name = "n0";
    auto n1 = std::make_shared<sflow_sgi_node_t>(); n1->m_node_name = "n1";
    auto n2 = std::make_shared<sflow_sgi_node_t>(); n2->m_node_name = "n2";
    auto n3 = std::make_shared<sflow_sgi_node_t>(); n3->m_node_name = "n3";

    fc->add_node(n0);
    fc->add_node(n1);
    fc->add_node(n2);
    fc->add_node(n3);

    REQUIRE(fc->connect_node(n0, n1, 0, 0) == sflow_error_e::no_error);
    REQUIRE(fc->connect_node(n0, n2, 0, 0) == sflow_error_e::no_error);
    REQUIRE(fc->connect_node(n0, n3, 0, 0) == sflow_error_e::no_error);

    REQUIRE(n0->m_is_outer);
    REQUIRE(!n1->m_is_outer);
    REQUIRE(!n2->m_is_outer);
    REQUIRE(!n3->m_is_outer);

    fc->execute();

    fc->clear_context();
    REQUIRE(n0->m_is_outer);
    REQUIRE(n1->m_is_outer);
    REQUIRE(n2->m_is_outer);
    REQUIRE(n3->m_is_outer);

  }

  SECTION ("sflow |0|(0) -> (0)|1|(0) -> (0)|2| , flow branching"
           "                   |-------> (0)|3|") {

    auto fc = std::make_shared<sflow_context_t>();

    auto n0 = std::make_shared<sflow_soi_node_t>(); n0->m_node_name = "nb0";
    auto n1 = std::make_shared<sflow_sgi_node_t>(); n1->m_node_name = "nb1";
    auto n2 = std::make_shared<sflow_sgi_node_t>(); n2->m_node_name = "nb2";
    auto n3 = std::make_shared<sflow_sii_node_t>(); n3->m_node_name = "nb3";

    fc->add_node(n0);
    fc->add_node(n1);
    fc->add_node(n2);
    fc->add_node(n3);

    REQUIRE(fc->connect_node(n0, n1, 0, 0) == sflow_error_e::no_error);
    REQUIRE(fc->connect_node(n1, n2, 0, 0) == sflow_error_e::no_error);
    REQUIRE(fc->connect_node(n1, n3, 0, 0) == sflow_error_e::no_error);

    REQUIRE(n0->m_is_outer);
    REQUIRE(!n1->m_is_outer);
    REQUIRE(!n2->m_is_outer);
    REQUIRE(!n3->m_is_outer);

    fc->execute();

    fc->clear_context();
    REQUIRE(n0->m_is_outer);
    REQUIRE(n1->m_is_outer);
    REQUIRE(n2->m_is_outer);
    REQUIRE(n3->m_is_outer);

  }

}
