#include <qppcad/texture_buffer.hpp>


using namespace qpp;
using namespace qpp::cad;

template<typename REAL>
void texture_buffer_t<REAL>::init () {

  glGenBuffers(1, &m_tx_buffer_id);
  glBindBuffer(GL_TEXTURE_BUFFER, m_tx_buffer_id);
  glBufferData(GL_TEXTURE_BUFFER, sizeof(REAL) * 4 * m_data.rows(), m_data.data(), GL_STREAM_DRAW);

  glGenTextures(1, &m_tx_id);
  glBindTexture(GL_TEXTURE_BUFFER, m_tx_id);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);                         // set 1-byte alignment

  glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_tx_id);

  glBindBuffer(GL_TEXTURE_BUFFER, 0);
  glBindTexture(GL_TEXTURE_BUFFER, 0);
}

template<typename REAL>
void texture_buffer_t<REAL>::bind(const int slot) {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_BUFFER, m_tx_id);
}

template<typename REAL>
void cad::texture_buffer_t<REAL>::unbind (const int slot) {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_BUFFER, 0);
}

template<typename REAL>
void cad::texture_buffer_t<REAL>::finalize () {
  glDeleteTextures(1, &m_tx_id);
  glDeleteBuffers(1, &m_tx_buffer_id);
}

template class qpp::cad::texture_buffer_t<float>;

