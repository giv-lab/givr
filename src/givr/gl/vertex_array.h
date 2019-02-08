#pragma once

#include "../gl.h"

namespace givr {
    class VertexArray {
        public:
            VertexArray();
            // TODO(lw): make a version that just receives the source directly.
            ~VertexArray();

            operator GLuint() const { return m_vertexArrayID; }
            void alloc();
            void dealloc();
            void bind();
            void unbind();

        private:
            GLuint m_vertexArrayID = 0;

    };
};// end namespace givr
