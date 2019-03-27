#include "lines.h"

using lrc = givr::style::LineRenderContext;
using lirc = givr::style::LineInstancedRenderContext;
using Colour = givr::style::Colour;

template <typename RenderContextT>
void setLineUniforms(RenderContextT const &ctx, std::unique_ptr<givr::Program> const &p) {
    p->setVec3("colour", ctx.template value<Colour>());
}

void lrc::setUniforms(std::unique_ptr<givr::Program> const &p) const {
    setLineUniforms(*this, p);
}
void lirc::setUniforms(std::unique_ptr<givr::Program> const &p) const {
    setLineUniforms(*this, p);
}

std::string linesVertexSource(std::string modelSource) {
    return "#version 330 core\n" + modelSource + std::string(R"shader( mat4 model;
        layout(location=4) in vec3 position;

        uniform mat4 view;
        uniform mat4 projection;

        void main(){
            mat4 mvp = projection * view * model;
            gl_Position = mvp * vec4(position, 1.0);
        }

        )shader"
    );
}

std::string linesFragmentSource() {
    return std::string(R"shader(#version 330 core
        uniform vec3 colour;
        uniform vec3 lightPosition;
        uniform vec3 viewPosition;

        out vec4 outColour;

        void main()
        {
            outColour = vec4(colour, 1.);
        }


        )shader"
    );
}

std::string lrc::getVertexShaderSource() const {
    return linesVertexSource("uniform");
}

std::string lrc::getFragmentShaderSource() const {
    return linesFragmentSource();
}

std::string lirc::getVertexShaderSource() const {
    return linesVertexSource("layout(location=0) in");
}

std::string lirc::getFragmentShaderSource() const {
    return linesFragmentSource();
}

