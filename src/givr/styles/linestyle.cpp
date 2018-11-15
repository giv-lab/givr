#include "linestyle.h"

using linestyle = givr::linestyle;

void linestyle::set_uniforms(std::unique_ptr<program> const &p) const {
    p->set_vec3("colour", colour);
    p->set_vec3("light_position", light_position);
}

std::string linestyle::get_vertex_shader_source() const {
    return std::string(R"shader(
        attribute mat4 model;
        attribute vec3 position;

        uniform mat4 view;
        uniform mat4 projection;

        varying vec3 frag_pos;

        void main(){
            mat4 modelview = view * model;
            mat4 mvp = projection * modelview;
            gl_Position = mvp * vec4(position, 1.0);
            vec4 model_vert = modelview * vec4(position, 1.0);
            frag_pos = vec3(model_vert);
        }

        )shader"
    );
}

std::string linestyle::get_fragment_shader_source() const {
    return std::string(R"shader(
        uniform vec3 colour;
        uniform vec3 light_position;
        uniform vec3 view_pos;

        varying vec3 frag_pos;

        void main()
        {
            gl_FragColor = colour;
        }


        )shader"
    );
}

