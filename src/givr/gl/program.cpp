#include "program.h"
#include <glm/gtc/type_ptr.hpp>

using Program = givr::Program;

Program::Program(
    GLuint vertex,
    GLuint fragment
) : m_programID{glCreateProgram()}
{
    glAttachShader(m_programID, vertex);
    glAttachShader(m_programID, fragment);
    glLinkProgram(m_programID);

    GLint success;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(m_programID, 512, NULL, infoLog);
        std::ostringstream out;
        out << "Unable to link Program: " << infoLog;
        // TODO(lw): Consider a better exception here
        throw std::runtime_error(out.str());
    }
}
Program::Program(
    std::initializer_list<ShaderArgs> shaders
) : m_programID{glCreateProgram()}
{
    for (const auto &args : shaders) {
        Shader shader{args.path, args.type};
        glAttachShader(m_programID, shader);
    }

    glLinkProgram(m_programID);
    GLint success;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(m_programID, 512, NULL, infoLog);
        std::ostringstream out;
        out << "Unable to link program: " << infoLog;
        // TODO(lw): Consider a better exception here
        throw std::runtime_error(out.str());
    }
}

Program::~Program() {
    glDeleteProgram(m_programID);
}

void Program::use() {
    glUseProgram(m_programID);
}

void Program::setVec2(const std::string &name, vec2f const &value) const
{
    glUniform2fv(glGetUniformLocation(m_programID, name.c_str()), 1, value_ptr(value));
}
void Program::setVec3(const std::string &name, vec3f const &value) const
{
    glUniform3fv(glGetUniformLocation(m_programID, name.c_str()), 1, value_ptr(value));
}
void Program::setMat4(const std::string &name, mat4f const &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_programID, name.c_str()), 1, GL_FALSE, value_ptr(mat));
}
void Program::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_programID, name.c_str()), static_cast<int>(value));
}
void Program::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(m_programID, name.c_str()), value);
}

/*
void Program::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(m_programID, name.c_str()), value);
}
void Program::setVec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(m_programID, name.c_str()), 1, glm::value_ptr(value));
}
void Program::setVec2(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(m_programID, name.c_str()), x, y);
}
void Program::setVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(m_programID, name.c_str()), x, y, z);
}
void Program::setVec4(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(m_programID, name.c_str()), 1, glm::value_ptr(value));
}
void Program::setVec4(const std::string &name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(m_programID, name.c_str()), x, y, z, w);
}
void Program::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(m_programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
void Program::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(m_programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}*/

