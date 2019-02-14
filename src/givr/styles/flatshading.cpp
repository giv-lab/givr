#include "flatshading.h"

using fsrc = givr::FlatShadingRenderContext;
using fsirc = givr::FlatShadingInstancedRenderContext;

template <typename RenderContextT>
void setFlatShadingUniforms(RenderContextT const &ctx, std::unique_ptr<givr::Program> const &p) {
    p->setVec3("colour", ctx.colour);
}

void fsrc::setUniforms(std::unique_ptr<givr::Program> const &p) const {
    setFlatShadingUniforms(*this, p);
}
void fsirc::setUniforms(std::unique_ptr<givr::Program> const &p) const {
    setFlatShadingUniforms(*this, p);
}

std::string flatShadingVertexSource(std::string modelSource) {
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

std::string flatShadingFragmentSource() {
    return std::string(R"shader(
        uniform vec3 colour;

        void main()
        {
            gl_FragColor = vec4(colour, 1.0);
        }

        )shader"
    );
}

std::string fsrc::getVertexShaderSource() const {
    return flatShadingVertexSource("uniform");
}
std::string fsrc::getFragmentShaderSource() const {
    return flatShadingFragmentSource();
}
std::string fsirc::getVertexShaderSource() const {
    return flatShadingVertexSource("attribute");
}
std::string fsirc::getFragmentShaderSource() const {
    return flatShadingFragmentSource();
}
