#include "shader.h"

using Shader = givr::Shader;

Shader::Shader(
    const std::string &source,
    GLenum shaderType
) : m_shaderID{glCreateShader(shaderType)}
{
    const GLchar *source_char = source.c_str();
    glShaderSource(m_shaderID, 1, &source_char, NULL);
    glCompileShader(m_shaderID);
    GLint success;
    glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(m_shaderID, 512, NULL, infoLog);
        std::ostringstream out;
        out << "Unable to compile Shader " << infoLog;
        // TODO(lw): Consider a better exception here
        throw std::runtime_error(out.str());
    }
}

Shader::~Shader() {
    glDeleteShader(m_shaderID);
}
