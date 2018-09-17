#ifndef QPPCAD_FRAMEBUFFER_OPENGL
#define QPPCAD_FRAMEBUFFER_OPENGL
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <qppcad/qppcad.hpp>

namespace qpp {
    struct frame_buffer_opengl_provider{
        typedef GLuint tex_handle;
        tex_handle m_fbo;
        tex_handle m_tex_color;
        tex_handle m_tex_depth;
        uint16_t m_width;
        uint16_t m_height;
        vector<GLenum> draw_buffer;

        void gen_textures(uint16_t width, uint16_t height){
          glGenTextures(1, &m_tex_color);
          glBindTexture(GL_TEXTURE_2D, m_tex_color);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
          glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

          glGenTextures(1, &m_tex_depth);
          glBindTexture(GL_TEXTURE_2D, m_tex_depth);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
          glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0,
                       GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        }

        void gen_fbo(uint16_t width, uint16_t height){
          glGenFramebuffers(1, &m_fbo);
          glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
          gen_textures(width, height);
          uint attachment_index_color_texture = 0;
          glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 +
                               attachment_index_color_texture, m_tex_color, 0);
          glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_tex_depth, 0);//optional
          draw_buffer.push_back(GL_COLOR_ATTACHMENT0 + attachment_index_color_texture);
          glDrawBuffers(draw_buffer.size(), &draw_buffer[0]);
          glBindFramebuffer(GL_FRAMEBUFFER, 0);
          m_width = width;
          m_height = height;
        }

        void resize_fbo(uint16_t new_width, uint16_t new_height){
          destroy_fbo();
          gen_fbo(new_width, new_height);
        }

        void bind_fbo(){
          glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        }

        void unbind_fbo(){
          glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void destroy_fbo(){
          glDeleteFramebuffers(1, &m_fbo);
          glDeleteTextures(1, &m_tex_color);
          glDeleteTextures(1, &m_tex_depth);
        }

    };
}

#endif
