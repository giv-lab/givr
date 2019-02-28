#include "buffer.h"
#include <cassert>

using Buffer = givr::Buffer;

Buffer::Buffer(
) : m_bufferID{0}
{
    alloc();
}

void Buffer::alloc() {
    dealloc();
    glGenBuffers(1, &m_bufferID);
}
void Buffer::dealloc() {
    if (m_bufferID) {
        glDeleteBuffers(1, &m_bufferID);
    }
}

void Buffer::bind(GLenum target) {
    glBindBuffer(target, m_bufferID);
}
void Buffer::unbind(GLenum target) {
    glBindBuffer(target, 0);
}

Buffer::~Buffer() {
    dealloc();
}
