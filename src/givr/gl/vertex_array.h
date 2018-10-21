#pragma once

#include <glad/glad.h>

namespace givr {
    class vertex_array {
        public:
            vertex_array() = default;
            vertex_array(GLuint num);
            // TODO(lw): make a version that just receives the source directly.
            ~vertex_array();

            operator GLuint() const { return m_vertex_array_ids[0]; }
            void alloc(GLuint num);
            void dealloc();
            void bind();
            void bind(GLuint i);
            void unbind();

        private:
            GLuint m_number_arrays = 0;
            GLuint *m_vertex_array_ids = nullptr;

    };
};// end namespace givr
