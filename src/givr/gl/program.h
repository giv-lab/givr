#pragma once

#include "../types.h"
#include "shader.h"

#include <memory>

#include "../gl.h"

namespace givr {

    class Program
    {
    public:
        struct ShaderArgs {
            std::string path;
            GLenum type;
        };
        Program(GLuint vertex, GLuint shader);
        explicit Program(std::initializer_list<ShaderArgs> shaders);
        ~Program();

        operator GLuint() const { return m_programID; }
        void use();

        void setVec2(const std::string &name, vec2f const &value) const;
        void setVec3(const std::string &name, vec3f const &value) const;
        void setMat4(const std::string &name, mat4f const &mat) const;
        void setBool(const std::string &name, bool value) const;
        void setFloat(const std::string &name, float value) const;

        // TODO: make these work for our math library
        /*
        void setInt(const std::string &name, int value) const;
        void setVec2(const std::string &name, const glm::vec2 &value) const;
        void setVec2(const std::string &name, float x, float y) const;
        void setVec4(const std::string &name, const glm::vec4 &value) const;
        void setVec4(const std::string &name, float x, float y, float z, float w);
        void setMat2(const std::string &name, const glm::mat2 &mat) const;
        void setMat3(const std::string &name, const glm::mat3 &mat) const;*/


        private:
            GLuint m_programID;

    };
};// end namespace givr
