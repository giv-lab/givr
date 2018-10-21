#include "shader.h"

using shader = givr::shader;

shader::shader(
    const std::string &source,
    GLenum shader_type
) : m_shader_id{glCreateShader(shader_type)}
{
    const GLchar *source_c = source.c_str();
    glShaderSource(m_shader_id, 1, &source_c, NULL);
    glCompileShader(m_shader_id);
    GLint success;
    glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar info_log[512];
        glGetShaderInfoLog(m_shader_id, 512, NULL, info_log);
        std::ostringstream out;
        out << "Unable to compile shader " << info_log;
        // TODO(lw): Consider a better exception here
        throw std::runtime_error(out.str());
    }
}

shader::~shader() {
    glDeleteShader(m_shader_id);
}
