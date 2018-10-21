#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace givr {

class shader
{
    public:
        shader(const std::string &source, GLenum shader_type);
        // TODO(lw): make a version that just receives the source directly.
        ~shader();

        operator GLuint() const { return m_shader_id; }


    private:
        GLuint m_shader_id;

};
};// end namespace givr
