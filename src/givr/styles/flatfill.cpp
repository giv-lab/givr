#include "flatfill.h"

using flatfill = givr::flatfill;
using flatfill_render_context = givr::flatfill_render_context;

void flatfill_render_context::set_uniforms(std::unique_ptr<givr::program> &p) const {
    p->set_vec3("colour", colour);
}

std::string flatfill::get_vertex_shader_source() const {
    return std::string(R"shader(
        attribute mat4 model;
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

std::string flatfill::get_fragment_shader_source() const {
    return std::string(R"shader(
        uniform vec3 colour;

        void main()
        {
            gl_FragColor = vec4(colour, 1.0);
        }

        )shader"
    );
}

