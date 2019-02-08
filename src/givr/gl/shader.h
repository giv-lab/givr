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

        operator GLuint() const { return m_shaderID; }


    private:
        GLuint m_shaderID;

};
};// end namespace givr
