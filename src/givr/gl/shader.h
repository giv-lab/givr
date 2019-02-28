#pragma once

#include "../gl.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace givr {

class Shader
{
    public:
        Shader(const std::string &source, GLenum shaderType);
        // TODO(lw): make a version that just receives the source directly.
        ~Shader();

        // Default ctor/dtor & move operations
        Shader(Shader &&other) = default;
        Shader &operator=(Shader &&rhs) = default;

        // But no copy or assignment. Bad.
        Shader(const Shader & ) = delete;
        Shader &operator=(const Shader &) = delete;

        operator GLuint() const { return m_shaderID; }

    private:
        GLuint m_shaderID = 0;

};
};// end namespace givr
