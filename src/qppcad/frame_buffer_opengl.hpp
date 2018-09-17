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


      void gen_fbo(uint16_t width, uint16_t height, uint8_t num_samples = 6){
        m_width = width;
        m_height = height;
        std::cout << fmt::format(" gen fbo {} {}\n", width, height);
        //generate output texture
        glGenTextures(1, &m_fbo_second_tex);
        glBindTexture(GL_TEXTURE_2D, m_fbo_second_tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenFramebuffers(1, &m_fbo_first);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_first);

        glGenRenderbuffers(1, &m_fbo_first_color_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_first_color_buffer);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 6, GL_RGB8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glGenRenderbuffers(1, &m_fbo_first_depth_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_first_depth_buffer);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 6, GL_DEPTH_COMPONENT,
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

      void resize_fbo(uint16_t new_width, uint16_t new_height){
        destroy_fbo();
        //std::cout<<"???????????????????????????????????\n"<<std::flush;
        gen_fbo(new_width, new_height);
      }

      void bind_fbo(){
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_first);
      }

      void unbind_fbo(){
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo_first);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo_second);
        glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height,
                          GL_COLOR_BUFFER_BIT, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
      }

      void destroy_fbo(){
        glDeleteFramebuffers(1, &m_fbo_first);
        glDeleteFramebuffers(1, &m_fbo_second);
        glDeleteRenderbuffers(1, &m_fbo_first_color_buffer);
        glDeleteRenderbuffers(1, &m_fbo_first_depth_buffer);
        glDeleteRenderbuffers(1, &m_fbo_second_color_buffer);
        glDeleteTextures(1, &m_fbo_second_tex);
      }

  };
}

#endif
