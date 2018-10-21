#include "vertex_array.h"
#include <cassert>

using vertex_array = givr::vertex_array;

vertex_array::vertex_array(
    GLuint num
) : m_number_arrays{num}
  , m_vertex_array_ids{nullptr}
{
    alloc(num);
}

void vertex_array::alloc(GLuint num) {
    dealloc();
    m_number_arrays =  num;
    m_vertex_array_ids = new GLuint[m_number_arrays];
    glGenVertexArrays(m_number_arrays, m_vertex_array_ids);
}
void vertex_array::dealloc() {
    if (m_vertex_array_ids) {
        glDeleteVertexArrays(m_number_arrays, m_vertex_array_ids);
        delete[] m_vertex_array_ids;
        m_vertex_array_ids = nullptr;
        m_number_arrays = 0;
    }
}

void vertex_array::bind() {
    bind(0);
}
void vertex_array::bind(GLuint i) {
    assert( i < m_number_arrays );
    glBindVertexArray(m_vertex_array_ids[i]);
}
void vertex_array::unbind() {
    glBindVertexArray(0);
}

vertex_array::~vertex_array() {
    dealloc();
}
