#include "noshading.h"

using nsrc = givr::style::NoShadingRenderContext;
using nsirc = givr::style::NoShadingInstancedRenderContext;

template <typename RenderContextT>
void setNoShadingUniforms(RenderContextT const &ctx, std::unique_ptr<givr::Program> const &p) {
    p->setVec3("colour", ctx.template value<givr::style::Colour>());
}

void nsrc::setUniforms(std::unique_ptr<givr::Program> const &p) const {
    setNoShadingUniforms(*this, p);
}
void nsirc::setUniforms(std::unique_ptr<givr::Program> const &p) const {
    setNoShadingUniforms(*this, p);
}

std::string noShadingVertexSource(std::string modelSource) {
    return modelSource + std::string(R"shader( mat4 model;
        attribute vec3 position;

        uniform mat4 view;
        uniform mat4 projection;
        uniform vec3 colour;

        void main()
        {
            mat4 mvp = projection * view * model;
            gl_Position = mvp * vec4(position, 1.0);
        }
        )shader"
    );
}

std::string noShadingFragmentSource() {
    return std::string(R"shader(
        uniform vec3 colour;

        void main()
        {
            gl_FragColor = vec4(colour, 1.0);
        }

        )shader"
    );
}

std::string nsrc::getVertexShaderSource() const {
    return noShadingVertexSource("uniform");
}
std::string nsrc::getFragmentShaderSource() const {
    return noShadingFragmentSource();
}
std::string nsirc::getVertexShaderSource() const {
    return noShadingVertexSource("attribute");
}
std::string nsirc::getFragmentShaderSource() const {
    return noShadingFragmentSource();
}
