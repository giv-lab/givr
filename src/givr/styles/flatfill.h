#pragma once

#include "../buffer_data.h"
#include "../instanced_renderer.h"
#include "../gl/program.h"
#include "static_assert.h"

#include <string>

namespace givr {

    struct flatfill_render_context : public instanced_render_context {
        vec3f colour;
        void set_uniforms(std::unique_ptr<program> &p) const;
    };

    struct flatfill {
        vec3f colour;

        using render_context = flatfill_render_context;

        std::string get_vertex_shader_source() const;
        std::string get_fragment_shader_source() const;

    };


    template <typename GeometryT>
    buffer_data fill_buffers(GeometryT const &g, flatfill const &f) {
        buffer_data data;
        typename GeometryT::data d = generate_geometry(g);
        static_assert(has_vertices<GeometryT>::value, "The flatfill style requires vertices. The geometry you are using does not provide them.");
        static_assert(has_indices<GeometryT>::value, "The flatfill style requires indices. The geometry you are using does not provide them.");
        data.add_vertices(d.vertices);
        data.add_indices(d.indices);
        return std::move(data);
    }

    template <typename GeometryT>
    flatfill::render_context get_context(GeometryT &geometry, flatfill const &f) {
        auto ctx = flatfill::render_context{};
        ctx.shader_program = std::make_unique<program>(
            shader{f.get_vertex_shader_source(), GL_VERTEX_SHADER},
            shader{f.get_fragment_shader_source(), GL_FRAGMENT_SHADER}
        );
        ctx.colour = f.colour;
        return ctx;
    }

    template <typename ViewContextT>
    void draw(flatfill::render_context &ctx, ViewContextT const &view_ctx) {
        draw_instanced(ctx, view_ctx, [&ctx](std::unique_ptr<program> program) {
            ctx.set_uniforms(program);
        });
    }
};// end namespace givr
