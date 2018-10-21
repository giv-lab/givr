#include "buffer.h"
#include <cassert>

using buffer = givr::buffer;

buffer::buffer(
    GLuint num
) : m_number_buffers{num}
  , m_buffer_ids{nullptr}
{
    alloc(num);
}

buffer::buffer(
) : m_number_buffers{0}
  , m_buffer_ids{nullptr}
{
}

void buffer::alloc(GLuint num) {
    dealloc();
    m_number_buffers = num;
    m_buffer_ids = new GLuint[m_number_buffers];
    glGenBuffers(m_number_buffers, m_buffer_ids);
}
void buffer::dealloc() {
    if (m_buffer_ids) {
        glDeleteBuffers(m_number_buffers, m_buffer_ids);
        delete[] m_buffer_ids;
        m_number_buffers = 0;
    }
}

void buffer::bind(GLenum target) {
    bind(target, 0);
}
void buffer::bind(GLenum target, GLuint i) {
    assert(i < m_number_buffers);
    glBindBuffer(target, m_buffer_ids[i]);
}

buffer::~buffer() {
    dealloc();
}
