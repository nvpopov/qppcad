#ifndef QPPCAD_FRAMEBUFFER
#define QPPCAD_FRAMEBUFFER
#include <qppcad/qppcad.hpp>
#include <qppcad/frame_buffer_opengl.hpp>

namespace qpp {

  namespace cad {

  template<typename PROVIDER = frame_buffer_opengl_provider>
  class frame_buffer_t {

    public:

      static constexpr uint16_t default_width{800};
      static constexpr uint16_t default_height{600};

      PROVIDER m_provider;

      uint16_t get_width(){
        return m_provider.m_width;
      }

      uint16_t get_height(){
        return m_provider.m_height;
      }

      void bind(){
        m_provider.bind_fbo();
      }

      void unbind(){
        m_provider.unbind_fbo();
      }

      void resize(const uint16_t new_width, const uint16_t new_height){
        m_provider.resize_fbo(new_width, new_height);
      }

      frame_buffer_t(const uint16_t width,
                     const uint16_t height,
                     bool enable_multisampling,
                     uint8_t num_multisampling) {
        m_provider.gen_fbo(width, height, enable_multisampling, num_multisampling);
      }

      typename PROVIDER::handle_t get_color_texture(){
        return  m_provider.m_fbo_second_tex;
      }

      typename PROVIDER::handle_t get_depth_texture(){
        return m_provider.m_fbo_second_tex;
      }

      ~frame_buffer_t(){
        m_provider.destroy_fbo();
      }

  };

  } // namespace qpp::cad

} // namespace qpp

#endif
