#include "lines.h"

using lrc = givr::style::LineRenderContext;
using lirc = givr::style::LineInstancedRenderContext;

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
    return modelSource + std::string(R"shader( mat4 model;
        attribute vec3 position;

        uniform mat4 view;
        uniform mat4 projection;

        varying vec3 fragPosition;

        void main(){
            mat4 modelview = model * view;
            mat4 mvp = projection * modelview;
            gl_Position = mvp * vec4(position, 1.0);
            vec4 modelVert = modelview * vec4(position, 1.0);
            fragPosition = vec3(modelVert);
        }

        )shader"
    );
}

std::string linesFragmentSource() {
    return std::string(R"shader(
        uniform vec3 colour;
        uniform vec3 lightPosition;
        uniform vec3 viewPosition;

        varying vec3 fragPosition;

        void main()
        {
            gl_FragColor = vec4(colour, 1.);
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
    return linesVertexSource("attribute");
}

std::string lirc::getFragmentShaderSource() const {
    return linesFragmentSource();
}

