#include <qppcad/texture_buffer.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;


//void texture_buffer_t::init () {

//  glGenBuffers(1, &m_tx_buffer_id);
//  glBindBuffer(GL_TEXTURE_BUFFER, m_tx_buffer_id);
//  glBufferData(GL_TEXTURE_BUFFER, sizeof(float) * 4 * m_data.rows(), m_data.data(), GL_STATIC_READ);

//  c_app::log(fmt::format("Texture buffer binded[size={}]", sizeof(float) * 4 * m_data.rows()));

//  glGenTextures(1, &m_tx_id);
//  glBindTexture(GL_TEXTURE_BUFFER, m_tx_id);
//  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//  glPixelStorei(GL_PACK_ALIGNMENT, 1);// set 1-byte alignment
//  glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_tx_id);

//  glBindBuffer(GL_TEXTURE_BUFFER, 0);
//  glBindTexture(GL_TEXTURE_BUFFER, 0);
//}


//void texture_buffer_t::bind(const int slot) {
//  glActiveTexture(GL_TEXTURE0 + slot);
//  glBindTexture(GL_TEXTURE_BUFFER, m_tx_id);
//}


//void cad::texture_buffer_t::unbind (const int slot) {
//  glActiveTexture(GL_TEXTURE0 + slot);
//  glBindTexture(GL_TEXTURE_BUFFER, 0);
//}

//void cad::texture_buffer_t::finalize () {
//  glDeleteTextures(1, &m_tx_id);
//  glDeleteBuffers(1, &m_tx_buffer_id);
//}


