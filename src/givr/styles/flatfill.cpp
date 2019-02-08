#include "flatfill.h"

using frc = givr::FlatfillRenderContext;
using firc = givr::FlatfillInstancedRenderContext;

template <typename RenderContextT>
void setFlatfillUniforms(RenderContextT const &ctx, std::unique_ptr<givr::Program> const &p) {
    p->setVec3("colour", ctx.colour);
}

void frc::setUniforms(std::unique_ptr<givr::Program> const &p) const {
    setFlatfillUniforms(*this, p);
}
void firc::setUniforms(std::unique_ptr<givr::Program> const &p) const {
    setFlatfillUniforms(*this, p);
}

std::string flatfillVertexSource(std::string modelSource) {
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

std::string flatfillFragmentSource() {
    return std::string(R"shader(
        uniform vec3 colour;

        void main()
        {
            gl_FragColor = vec4(colour, 1.0);
        }

        )shader"
    );
}

std::string frc::getVertexShaderSource() const {
    return flatfillVertexSource("uniform");
}
std::string frc::getFragmentShaderSource() const {
    return flatfillFragmentSource();
}
std::string firc::getVertexShaderSource() const {
    return flatfillVertexSource("attribute");
}
std::string firc::getFragmentShaderSource() const {
    return flatfillFragmentSource();
}
