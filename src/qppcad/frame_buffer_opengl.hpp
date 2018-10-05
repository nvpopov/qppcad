#ifndef QPPCAD_FRAMEBUFFER_OPENGL
#define QPPCAD_FRAMEBUFFER_OPENGL
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <qppcad/qppcad.hpp>


namespace qpp {

  struct frame_buffer_opengl_provider{

      typedef GLuint handle_t;

      handle_t m_fbo_first;
      handle_t m_fbo_first_color_buffer;
      handle_t m_fbo_first_depth_buffer;

      handle_t m_fbo_second;
      handle_t m_fbo_second_color_buffer;
      handle_t m_fbo_second_tex;

      uint16_t m_width;
      uint16_t m_height;
      uint16_t m_num_samples;

      bool m_multisampling{true};

      ///
      /// Start gen fbo section
      ///
      void gen_fbo (uint16_t width,
                    uint16_t height,
                    bool enable_multisampling = true,
                    uint8_t num_samples = 8) {

        m_multisampling = enable_multisampling;

        if (m_multisampling){
            m_num_samples = num_samples;
            gen_fbo_multisample(width, height);
          } else {
            m_num_samples = 0;
            gen_fbo_legacy(width, height);
          }

      }

      void gen_fbo_legacy(uint16_t width,
                          uint16_t height){

      }

      void gen_fbo_multisample (uint16_t width,
                                uint16_t height) {

        m_width = width;
        m_height = height;

        glGenTextures(1, &m_fbo_second_tex);
        glBindTexture(GL_TEXTURE_2D, m_fbo_second_tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenFramebuffers(1, &m_fbo_first);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_first);

        glGenRenderbuffers(1, &m_fbo_first_color_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_first_color_buffer);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_num_samples, GL_RGB8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glGenRenderbuffers(1, &m_fbo_first_depth_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_first_depth_buffer);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_num_samples, GL_DEPTH_COMPONENT,
                                         width, height);

        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER,
                                  m_fbo_first_color_buffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
                                  m_fbo_first_depth_buffer);

        glGenFramebuffers(1, &m_fbo_second);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_second);

        glGenRenderbuffers(1, &m_fbo_second_color_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_second_color_buffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                               m_fbo_second_tex, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
                                  m_fbo_second_color_buffer);

      }
      ///
      /// End gen fbo section
      ///

      void resize_fbo (uint16_t new_width, uint16_t new_height) {

        destroy_fbo();
        gen_fbo(new_width, new_height, m_multisampling, m_num_samples);

      }

      ///
      /// Start bind fbo section
      ///
      void bind_fbo () {
        if (m_multisampling) bind_fbo_multisample(); else bind_fbo_legacy();
      }

      void bind_fbo_legacy () {

      }

      void bind_fbo_multisample () {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_first);
      }
      ///
      /// End bind fbo section
      ///

      ///
      /// Start unbind fbo section
      ///
      void unbind_fbo () {
        if (m_multisampling) unbind_fbo_multisample(); else unbind_fbo_legacy();
      }

      void unbind_fbo_legacy () {

      }

      void unbind_fbo_multisample () {

        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo_first);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo_second);
        glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height,
                          GL_COLOR_BUFFER_BIT, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

      }
      ///
      /// End unbind fbo section
      ///

      ///
      /// Start destroy fbo section
      ///
      void destroy_fbo () {
        if (m_multisampling) destroy_fbo_multisample(); else destroy_fbo_legacy();
      }

      void destroy_fbo_legacy () {

      }


      void destroy_fbo_multisample () {

        glDeleteFramebuffers(1, &m_fbo_first);
        glDeleteFramebuffers(1, &m_fbo_second);
        glDeleteRenderbuffers(1, &m_fbo_first_color_buffer);
        glDeleteRenderbuffers(1, &m_fbo_first_depth_buffer);
        glDeleteRenderbuffers(1, &m_fbo_second_color_buffer);
        glDeleteTextures(1, &m_fbo_second_tex);

      }
      ///
      /// End destroy fbo section
      ///

  };
}

#endif
