#include "linestrip.h"

using lrc = givr::linestrip_render_context;
using lirc = givr::linestrip_instanced_render_context;

template <typename RenderContextT>
void set_linestrip_uniforms(RenderContextT const &ctx, std::unique_ptr<givr::program> const &p) {
    p->set_vec3("colour", ctx.colour);
}

void lrc::set_uniforms(std::unique_ptr<givr::program> const &p) const {
    set_linestrip_uniforms(*this, p);
}
void lirc::set_uniforms(std::unique_ptr<givr::program> const &p) const {
    set_linestrip_uniforms(*this, p);
}

std::string linestrip_vertex_source(std::string model_source) {
    return model_source + std::string(R"shader( mat4 model;
        attribute vec3 position;

        uniform mat4 view;
        uniform mat4 projection;

        varying vec3 frag_pos;

        void main(){
            mat4 modelview = model * view;
            mat4 mvp = projection * modelview;
            gl_Position = mvp * vec4(position, 1.0);
            vec4 model_vert = modelview * vec4(position, 1.0);
            frag_pos = vec3(model_vert);
        }

        )shader"
    );
}

std::string linestrip_fragment_source() {
    return std::string(R"shader(
        uniform vec3 colour;
        uniform vec3 light_position;
        uniform vec3 view_pos;

        varying vec3 frag_pos;

        void main()
        {
            gl_FragColor = vec4(colour, 1.);
        }


        )shader"
    );
}

std::string lrc::get_vertex_shader_source() const {
    return linestrip_vertex_source("uniform");
}

std::string lrc::get_fragment_shader_source() const {
    return linestrip_fragment_source();
}

std::string lirc::get_vertex_shader_source() const {
    return linestrip_vertex_source("attribute");
}

std::string lirc::get_fragment_shader_source() const {
    return linestrip_fragment_source();
}

