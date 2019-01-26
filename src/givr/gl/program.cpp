#include "program.h"
#include <glm/gtc/type_ptr.hpp>

using program = givr::program;

program::program(
    GLuint vertex,
    GLuint fragment
) : m_program_id{glCreateProgram()}
{
    glAttachShader(m_program_id, vertex);
    glAttachShader(m_program_id, fragment);
    glLinkProgram(m_program_id);

    GLint success;
    glGetProgramiv(m_program_id, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar info_log[512];
        glGetProgramInfoLog(m_program_id, 512, NULL, info_log);
        std::ostringstream out;
        out << "Unable to link program: " << info_log;
        // TODO(lw): Consider a better exception here
        throw std::runtime_error(out.str());
    }
}
program::program(
    std::initializer_list<shader_args> shaders
) : m_program_id{glCreateProgram()}
{
    for (const auto &args : shaders) {
        shader shader{args.path, args.type};
        glAttachShader(m_program_id, shader);
    }

    glLinkProgram(m_program_id);
    GLint success;
    glGetProgramiv(m_program_id, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar info_log[512];
        glGetProgramInfoLog(m_program_id, 512, NULL, info_log);
        std::ostringstream out;
        out << "Unable to link program: " << info_log;
        // TODO(lw): Consider a better exception here
        throw std::runtime_error(out.str());
    }
}

program::~program() {
    glDeleteProgram(m_program_id);
}

void program::use() {
    glUseProgram(m_program_id);
}

void program::set_vec2(const std::string &name, vec2f const &value) const
{
    glUniform2fv(glGetUniformLocation(m_program_id, name.c_str()), 1, value_ptr(value));
}
void program::set_vec3(const std::string &name, vec3f const &value) const
{
    glUniform3fv(glGetUniformLocation(m_program_id, name.c_str()), 1, value_ptr(value));
}
void program::set_mat4(const std::string &name, mat4f const &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_program_id, name.c_str()), 1, GL_FALSE, value_ptr(mat));
}
void program::set_bool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_program_id, name.c_str()), static_cast<int>(value));
}
void program::set_float(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(m_program_id, name.c_str()), value);
}

/*
void program::set_int(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(m_program_id, name.c_str()), value);
}
void program::set_vec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(m_program_id, name.c_str()), 1, glm::value_ptr(value));
}
void program::set_vec2(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(m_program_id, name.c_str()), x, y);
}
void program::set_vec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(m_program_id, name.c_str()), x, y, z);
}
void program::set_vec4(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(m_program_id, name.c_str()), 1, glm::value_ptr(value));
}
void program::set_vec4(const std::string &name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(m_program_id, name.c_str()), x, y, z, w);
}
void program::set_mat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(m_program_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
void program::set_mat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(m_program_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}*/

