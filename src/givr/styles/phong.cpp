#include "phong.h"

using prc = givr::style::PhongRenderContext;
using pirc = givr::style::PhongInstancedRenderContext;
using namespace givr::style;

template <typename RenderContextT>
void setPhongUniforms(RenderContextT const &ctx, std::unique_ptr<givr::Program> const &p) {
    using namespace givr::style;
    p->setVec3("colour", ctx.template value<Colour>());
    p->setVec3("lightPosition", ctx.template value<LightPosition>());
    p->setFloat("ambientFactor", ctx.template value<AmbientFactor>());
    p->setFloat("specularFactor", ctx.template value<SpecularFactor>());
    p->setFloat("phongExponent", ctx.template value<PhongExponent>());
    p->setBool("perVertexColour", ctx.template value<PerVertexColour>());
}

void pirc::setUniforms(std::unique_ptr<givr::Program> const &p) const {
    setPhongUniforms(*this, p);
}
void prc::setUniforms(std::unique_ptr<givr::Program> const &p) const {
    setPhongUniforms(*this, p);
}

std::string phongVertexSource(std::string modelSource) {
    return
        "#version 330 core\n" + modelSource + std::string(R"shader( mat4 model;
        in vec3 position;
        in vec3 normal;
        in vec2 uvs;
        in vec3 colour;

        uniform mat4 view;
        uniform mat4 projection;

        out vec3 fragNormal;
        out vec3 originalPosition;
        out vec2 fragUv;
        out vec3 fragColour;

        void main(){
            mat4 mv = view * model;
            mat4 mvp = projection * mv;
            gl_Position = mvp * vec4(position, 1.0);
            originalPosition = vec3(model * vec4(position, 1.0));
            fragNormal = vec3(model*vec4(normal, 0));
            fragColour = colour;
        }

        )shader"
    );
}

std::string phongFragmentSource() {
    return std::string(R"shader(#version 330 core
        uniform vec3 colour;
        uniform bool perVertexColour;
        uniform vec3 lightPosition;
        uniform float ambientFactor;
        uniform float specularFactor;
        uniform float phongExponent;
        uniform vec3 viewPosition;

        in vec3 fragNormal;
        in vec3 originalPosition;
        in vec2 fragUv;
        in vec3 fragColour;

        out vec4 outColour;

        void main()
        {
            vec3 finalColour = colour;
            if (perVertexColour) {
                finalColour = fragColour;
            }
            // ambient
            vec3 ambient = ambientFactor * finalColour;

            // diffuse
            vec3 lightDirection = normalize(lightPosition - originalPosition);
            vec3 normal = normalize(fragNormal);
            float diff = max(dot(lightDirection, normal), 0.0);
            vec3 diffuse = diff * finalColour;

            // specular
            vec3 viewDirection = normalize(viewPosition - originalPosition);
            vec3 reflectDirection = max(2.0*dot(lightDirection, normal), 0.0)*normal - lightDirection;
            float spec = 0.0;
            spec = pow(max(dot(viewDirection, reflectDirection), 0.0), phongExponent);
            vec3 specular = vec3(specularFactor) * spec; // assuming bright white light colour

            outColour = vec4(ambient + diffuse + specular, 1.0);
        }


        )shader"
    );
}

// TODO: these shaders are near duplicates of each other, we need to deal
//       with this.
std::string pirc::getVertexShaderSource() const {
    return phongVertexSource("in");
}

std::string pirc::getFragmentShaderSource() const {
    return phongFragmentSource();
}

std::string prc::getVertexShaderSource() const {
    return phongVertexSource("uniform");
}

std::string prc::getFragmentShaderSource() const {
    return phongFragmentSource();
}
