#include "flatfill.h"

using frc = givr::flatfill_render_context;
using firc = givr::flatfill_instanced_render_context;

template <typename RenderContextT>
void set_flatfill_uniforms(RenderContextT const &ctx, std::unique_ptr<givr::program> const &p) {
    p->set_vec3("colour", ctx.colour);
}

void frc::set_uniforms(std::unique_ptr<givr::program> const &p) const {
    set_flatfill_uniforms(*this, p);
}
void firc::set_uniforms(std::unique_ptr<givr::program> const &p) const {
    set_flatfill_uniforms(*this, p);
}

std::string flatfill_vertex_source(std::string model_source) {
    return model_source + std::string(R"shader( mat4 model;
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

std::string flatfill_fragment_source() {
    return std::string(R"shader(
        uniform vec3 colour;

        void main()
        {
            gl_FragColor = vec4(colour, 1.0);
        }

        )shader"
    );
}

std::string frc::get_vertex_shader_source() const {
    return flatfill_vertex_source("uniform");
}
std::string frc::get_fragment_shader_source() const {
    return flatfill_fragment_source();
}
std::string firc::get_vertex_shader_source() const {
    return flatfill_vertex_source("attribute");
}
std::string firc::get_fragment_shader_source() const {
    return flatfill_fragment_source();
}
