#pragma once

#include <glad/glad.h>

#include <vector>
#include <array>
#include <iostream>

namespace givr {

    class buffer
    {
        public:
            buffer();
            explicit buffer(GLuint num);
            // TODO(lw): make a version that just receives the source directly.
            buffer(const buffer&) = delete;
            buffer(buffer&&) = delete;
            buffer& operator=(const buffer&) = delete;
            buffer& operator=(buffer&&) = delete;
            ~buffer();

            operator GLuint() const { return m_buffer_ids[0]; }
            void alloc(GLuint num);
            void dealloc();
            void bind(GLenum target);
            void bind(GLenum target, GLuint i);
            template <typename T>
            void data(GLenum target, const std::vector<T> &data, GLenum usage) {
                glBufferData(target, sizeof(T) * data.size(), data.data(), usage);
            }
            template <typename T, long unsigned int Size>
            void data(GLenum target, const std::array<T, Size> &data, GLenum usage) {
                glBufferData(target, sizeof(T) * Size, data.data(), usage);
            }

            void debug() {
                for (GLuint i = 0; i < m_number_buffers; ++i) {
                    std::cout << "#" << i << " -> " << m_buffer_ids[i] << std::endl;
                }
            }

        private:
            GLuint m_number_buffers = 0;
            GLuint *m_buffer_ids = nullptr;
    };
};// end namespace givr
