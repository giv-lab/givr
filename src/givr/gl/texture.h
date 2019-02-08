#pragma once

#include "../gl.h"

#include <vector>
#include <array>

namespace givr {
    class Texture
    {
        public:
            Texture();
            // TODO(lw): make a version that just receives the source directly.
            ~Texture();

            operator GLuint() const { return m_textureID; }
            void alloc();
            void dealloc();
            void bind(GLenum target);
            void load(GLenum target, std::string filename, GLint level=0, GLenum format=GL_RGB);

        private:
            GLuint m_textureID = 0;

    };
};// end namespace givr
