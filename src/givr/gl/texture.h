#pragma once

#include "../gl.h"

#include <vector>
#include <array>

namespace givr {
    class texture
    {
        public:
            texture();
            texture(GLuint num);
            // TODO(lw): make a version that just receives the source directly.
            ~texture();

            operator GLuint() const { return m_texture_ids[0]; }
            void alloc(GLuint num);
            void dealloc();
            void bind(GLenum target);
            void bind(GLenum target, GLuint i);
            void load(GLenum target, std::string filename, GLint level=0, GLenum format=GL_RGB);

        private:
            GLuint m_number_textures = 0;
            GLuint *m_texture_ids = nullptr;

    };
};// end namespace givr
