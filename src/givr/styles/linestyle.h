#pragma once

#include "../buffer_data.h"
#include "../array_renderer.h"
#include "../gl/program.h"
#include "static_assert.h"

#include <string>

namespace givr {

    struct linestyle_render_context : public array_render_context {
        vec3f colour;
        float line_width = 1.f;

        void set_uniforms(std::unique_ptr<program> const &p) const;
    };

    struct linestyle {
        vec3f colour;
        float line_width = 1.f;

        using render_context = linestyle_render_context;

        std::string get_vertex_shader_source() const;
        std::string get_fragment_shader_source() const;
    };

    template <typename GeometryT>
    buffer_data fill_buffers(GeometryT const &g, linestyle const &) {
        buffer_data data;
        typename GeometryT::data d = generate_geometry(g);
        static_assert(has_vertices<GeometryT>::value, "The linestyle style requires vertices. The geometry you are using does not provide them.");
        data.add_vertices(d.vertices);

        // TODO: better way to optionall add data like colours.
        // if (d.colours.size() > 0) data.add_colours(d.colours);
        return std::move(data);
    }

    template <typename GeometryT>
    linestyle::render_context get_context(GeometryT &, linestyle const &l) {
        auto ctx = linestyle::render_context{};
        ctx.shader_program = std::make_unique<program>(
            shader{l.get_vertex_shader_source(), GL_VERTEX_SHADER},
            shader{l.get_fragment_shader_source(), GL_FRAGMENT_SHADER}
        );
        ctx.primitive = primitive_type::LINE_STRIP;
        ctx.colour = l.colour;
        ctx.line_width = l.line_width;
        return ctx;
    }

    template <typename ViewContextT>
    void draw(linestyle::render_context &ctx, ViewContextT const &view_ctx) {
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(ctx.line_width);
        draw_array(ctx, view_ctx, [&ctx](std::unique_ptr<program> const &program) {
            ctx.set_uniforms(program);
        });
    }

};// end namespace
