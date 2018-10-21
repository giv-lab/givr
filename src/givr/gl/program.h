#pragma once

#include "shader.h"
#include "som.h"

#include <memory>

#include <glad/glad.h>

namespace givr {

    class program
    {
    public:
        struct shader_args {
            std::string path;
            GLenum type;
        };
        program(GLuint vertex, GLuint shader);
        explicit program(std::initializer_list<shader_args> shaders);
        ~program();

        operator GLuint() const { return m_program_id; }
        void use();

        void set_vec3(const std::string &name, vec3f const &value) const;
        void set_mat4(const std::string &name, mat4f const &mat) const;

        // TODO: make these work for our math library
        /*void set_bool(const std::string &name, bool value) const;
        void set_int(const std::string &name, int value) const;
        void set_float(const std::string &name, float value) const;
        void set_vec2(const std::string &name, const glm::vec2 &value) const;
        void set_vec2(const std::string &name, float x, float y) const;
        void set_vec3(const std::string &name, float x, float y, float z) const;
        void set_vec4(const std::string &name, const glm::vec4 &value) const;
        void set_vec4(const std::string &name, float x, float y, float z, float w);
        void set_mat2(const std::string &name, const glm::mat2 &mat) const;
        void set_mat3(const std::string &name, const glm::mat3 &mat) const;
        void set_mat4(const std::string &name, const glm::mat4 &mat) const;*/


        private:
            GLuint m_program_id;

    };
};// end namespace givr
