#ifndef QPPCAD_FRAMEBUFFER_OPENGL
#define QPPCAD_FRAMEBUFFER_OPENGL
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <qppcad/core/qppcad.hpp>


namespace qpp {

  struct frame_buffer_opengl_provider{

      typedef GLuint handle_t;

      handle_t m_fbo_first;
      handle_t m_fbo_first_color_buffer;
      handle_t m_fbo_first_depth_buffer;

      handle_t m_fbo_second;
      handle_t m_fbo_second_color_buffer;
      handle_t m_fbo_second_tex;
      handle_t m_fbo_second_tex_depth;

      uint16_t m_width;
      uint16_t m_height;
      uint16_t m_num_samples;

      bool m_multisampling{true};

      ///
      /// Start gen fbo section
      ///
      void gen_fbo (uint16_t width,
                    uint16_t height,
                    bool enable_multisampling,
                    uint8_t num_samples) {

        m_multisampling = enable_multisampling;
        m_width = width;
        m_height = height;

        if (m_multisampling){
            m_num_samples = num_samples;
            gen_fbo_multisample();
          } else {
            m_num_samples = 0;
            gen_fbo_legacy();
          }

      }

      void gen_fbo_legacy () {

        //color texture
        glGenTextures(1, &m_fbo_second_tex);
        glBindTexture(GL_TEXTURE_2D, m_fbo_second_tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenTextures(1, &m_fbo_second_tex_depth);
        glBindTexture(GL_TEXTURE_2D, m_fbo_second_tex_depth);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0,
                     GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

        glGenFramebuffers(1, &m_fbo_first);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_first);

        // attach a texture to FBO depth attachement point
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                               m_fbo_second_tex, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER,  GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                               m_fbo_second_tex_depth, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

      }

      void gen_fbo_multisample () {

        glGenTextures(1, &m_fbo_second_tex);
        glBindTexture(GL_TEXTURE_2D, m_fbo_second_tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenFramebuffers(1, &m_fbo_first);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_first);

        glGenRenderbuffers(1, &m_fbo_first_color_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_first_color_buffer);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_num_samples, GL_RGB8,
                                         m_width, m_height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glGenRenderbuffers(1, &m_fbo_first_depth_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_first_depth_buffer);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_num_samples, GL_DEPTH_COMPONENT,
                                         m_width, m_height);

        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER,
                                  m_fbo_first_color_buffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
                                  m_fbo_first_depth_buffer);

        glGenFramebuffers(1, &m_fbo_second);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_second);

        glGenRenderbuffers(1, &m_fbo_second_color_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_second_color_buffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                               m_fbo_second_tex, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
                                  m_fbo_second_color_buffer);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_first);
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
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

        glDeleteFramebuffers(1, &m_fbo_first);
        glDeleteRenderbuffers(1, &m_fbo_first_depth_buffer);
        glDeleteTextures(1, &m_fbo_second_tex);

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

  } // namespace qpp::cad

} // namespace qpp

#endif
