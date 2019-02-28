#pragma once

#include "../gl.h"

#include <vector>
#include <array>
#include <iostream>

namespace givr {

    class Buffer
    {
        public:
            Buffer();
            // TODO(lw): make a version that just receives the source directly.

            // Default ctor/dtor & move operations
            Buffer(Buffer &&other) = default;
            Buffer &operator=(Buffer &&rhs) = default;

            // But no copy or assignment. Bad.
            Buffer(const Buffer & ) = delete;
            Buffer &operator=(const Buffer &) = delete;

            ~Buffer();

            operator GLuint() const { return m_bufferID; }
            void alloc();
            void dealloc();
            void bind(GLenum target);
            void unbind(GLenum target);
            template <typename T>
            void data(GLenum target, const std::vector<T> &data, GLenum usage) {
                glBufferData(target, sizeof(T) * data.size(), data.data(), usage);
            }
            template <typename T, long unsigned int Size>
            void data(GLenum target, const std::array<T, Size> &data, GLenum usage) {
                glBufferData(target, sizeof(T) * Size, data.data(), usage);
            }

        private:
            GLuint m_bufferID = 0;
    };
};// end namespace givr
