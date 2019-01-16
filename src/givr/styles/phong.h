#pragma once

#include "../buffer_data.h"
#include "../instanced_renderer.h"
#include "../gl/program.h"
#include "static_assert.h"

#include <string>

namespace givr {

    struct phong_render_context : public instanced_render_context {
        vec3f colour;
        vec3f light_position;
        bool per_vertex_colour = false;
        float ambient_factor = 0.05f;
        float specular_factor = 0.3f;
        float phong_exponent = 8.0f;

        void set_uniforms(std::unique_ptr<program> const &p) const;
    };

    struct phong {
        vec3f colour;
        vec3f light_position;
        bool per_vertex_colour = false;
        float ambient_factor = 0.05f;
        float specular_factor = 0.3f;
        float phong_exponent = 8.0f;

        using render_context = phong_render_context;

        std::string get_vertex_shader_source() const;
        std::string get_fragment_shader_source() const;

    };

    template <typename GeometryT>
    buffer_data fill_buffers(GeometryT const &g, phong const &) {
        buffer_data data;
        typename GeometryT::data d = generate_geometry(g);
        static_assert(has_vertices<GeometryT>::value, "The phong style requires vertices. The geometry you are using does not provide them.");
        static_assert(has_indices<GeometryT>::value, "The phong style requires indices. The geometry you are using does not provide them.");
        static_assert(has_normals<GeometryT>::value, "The phong style requires normals. The geometry you are using does not provide them.");
        data.vertices_type = d.vertices_type;
        data.add_vertices(d.vertices);
        data.indices_type = d.indices_type;
        data.add_indices(d.indices);
        data.normals_type = d.normals_type;
        data.add_normals(d.normals);
        data.colours_type = d.colours_type;
        data.add_colours(d.colours);
        return std::move(data);
    }

    template <typename GeometryT>
    phong::render_context get_context(GeometryT &, phong const &p) {
        auto ctx = phong::render_context{};
        ctx.shader_program = std::make_unique<program>(
            shader{p.get_vertex_shader_source(), GL_VERTEX_SHADER},
            shader{p.get_fragment_shader_source(), GL_FRAGMENT_SHADER}
        );
        ctx.primitive = primitive_type::TRIANGLES;
        ctx.colour = p.colour;
        ctx.light_position = p.light_position;
        ctx.per_vertex_colour = p.per_vertex_colour;
        ctx.ambient_factor = p.ambient_factor;
        ctx.specular_factor = p.specular_factor;
        ctx.phong_exponent = p.phong_exponent;
        return ctx;
    }

    template <typename ViewContextT>
    void draw(phong::render_context &ctx, ViewContextT const &view_ctx) {
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        draw_instanced(ctx, view_ctx, [&ctx](std::unique_ptr<program> const &program) {
            ctx.set_uniforms(program);
        });
    }

};// end namespace
