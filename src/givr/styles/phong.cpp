#include "phong.h"

using phong = givr::phong;

void phong::set_uniforms(std::unique_ptr<givr::program> const &p) const {
    p->set_vec3("colour", colour);
    p->set_bool("per_vertex_colour", per_vertex_colour);
    p->set_vec3("light_position", light_position);
    p->set_float("ambient_factor", ambient_factor);
    p->set_float("specular_factor", specular_factor);
    p->set_float("phong_exponent", phong_exponent);
}

std::string phong::get_vertex_shader_source() const {
    return std::string(R"shader(
        attribute mat4 model;
        attribute vec3 position;
        attribute vec3 normal;
        attribute vec2 uvs;
        attribute vec3 colour;

        uniform mat4 view;
        uniform mat4 projection;

        varying vec3 frag_normal;
        varying vec3 original_pos;
        varying vec3 frag_colour;

        void main(){
            mat4 mv = view * model;
            mat4 mvp = projection * mv;
            gl_Position = mvp * vec4(position, 1.0);
            vec4 model_vert = mv * vec4(position, 1.0);
            original_pos = vec3(position);
            frag_normal = normal;
            frag_colour = colour;
        }

        )shader"
    );
}

std::string phong::get_fragment_shader_source() const {
    return std::string(R"shader(
        uniform vec3 colour;
        uniform bool per_vertex_colour;
        uniform vec3 light_position;
        uniform float ambient_factor;
        uniform float specular_factor;
        uniform float phong_exponent;
        uniform vec3 view_pos;

        varying vec3 frag_normal;
        varying vec3 original_pos;
        varying vec2 frag_uv;
        varying vec3 frag_colour;

        void main()
        {
            vec3 final_colour = colour;
            if (per_vertex_colour) {
                final_colour = frag_colour;
            }
            // ambient
            vec3 ambient = ambient_factor * final_colour;

            // diffuse
            vec3 light_dir = normalize(light_position - original_pos);
            vec3 normal = normalize(frag_normal);
            float diff = max(dot(light_dir, normal), 0.0);
            vec3 diffuse = diff * final_colour;

            // specular
            vec3 view_dir = normalize(view_pos - original_pos);
            vec3 reflect_dir = reflect(-light_dir, normal);
            float spec = 0.0;
            spec = pow(max(dot(view_dir, reflect_dir), 0.0), phong_exponent);
            vec3 specular = vec3(specular_factor) * spec; // assuming bright white light colour

            gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
        }


        )shader"
    );
}

