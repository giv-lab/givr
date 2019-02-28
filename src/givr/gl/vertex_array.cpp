#include "vertex_array.h"
#include <cassert>

using VertexArray = givr::VertexArray;

VertexArray::VertexArray(
) : m_vertexArrayID{}
{
    alloc();
}

void VertexArray::alloc() {
    dealloc();
    glGenVertexArrays(1, &m_vertexArrayID);
}

void VertexArray::dealloc() {
    if (m_vertexArrayID) {
        glDeleteVertexArrays(1, &m_vertexArrayID);
        m_vertexArrayID = 0;
    }
}

void VertexArray::bind() {
    glBindVertexArray(m_vertexArrayID);
}
void VertexArray::unbind() {
    glBindVertexArray(0);
}

VertexArray::~VertexArray() {
    dealloc();
}
