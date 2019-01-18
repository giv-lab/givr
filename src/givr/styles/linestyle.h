#pragma once

#include "../buffer_data.h"
#include "../renderer.h"
#include "../instanced_renderer.h"
#include "../gl/program.h"
#include "static_assert.h"

#include <string>

namespace givr {

    struct linestyle_params {
        vec3f colour;
        float line_width = 1.f;
    };

    struct linestyle_render_context
        : public render_context,
          public linestyle_params
    {

        void set_uniforms(std::unique_ptr<program> const &p) const;

        std::string get_vertex_shader_source() const;
        std::string get_fragment_shader_source() const;
    };

    struct linestyle_instanced_render_context
        : public instanced_render_context,
          public linestyle_params
    {

        void set_uniforms(std::unique_ptr<program> const &p) const;

        std::string get_vertex_shader_source() const;
        std::string get_fragment_shader_source() const;
    };

    struct linestyle : public linestyle_params {
        using render_context = linestyle_render_context;
        using instanced_render_context = linestyle_instanced_render_context;
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

    template <typename RenderContextT, typename GeometryT>
    RenderContextT get_context(GeometryT &, linestyle const &l) {
        auto ctx = RenderContextT{};
        ctx.shader_program = std::make_unique<program>(
            shader{ctx.get_vertex_shader_source(), GL_VERTEX_SHADER},
            shader{ctx.get_fragment_shader_source(), GL_FRAGMENT_SHADER}
        );
        update_style(ctx, l);
        return ctx;
    }

    template <typename GeometryT>
    linestyle::render_context
    get_context(GeometryT &g, linestyle const &l) {
        return get_context<linestyle::render_context, GeometryT>(g, l);
    }

    template <typename GeometryT>
    linestyle::instanced_render_context
    get_instanced_context(GeometryT &g, linestyle const &l) {
        return get_context<linestyle::instanced_render_context, GeometryT>(g, l);
    }

    template <typename RenderContextT>
    void update_style(RenderContextT &ctx, linestyle const &l) {
        // TODO: Want a compile time guard to ensure geometry and style are compatible.
        ctx.primitive = primitive_type::LINE_STRIP;
        ctx.colour = l.colour;
        ctx.line_width = l.line_width;
    }

    template <typename ViewContextT>
    void draw(linestyle::instanced_render_context &ctx, ViewContextT const &view_ctx) {
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(ctx.line_width);
        draw_instanced(ctx, view_ctx, [&ctx](std::unique_ptr<program> const &program) {
            ctx.set_uniforms(program);
        });
    }

    template <typename ViewContextT>
    void draw(linestyle::render_context &ctx, ViewContextT const &view_ctx, mat4f model=mat4f(1.f)) {
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(ctx.line_width);
        draw_array(ctx, view_ctx, [&ctx, &model](std::unique_ptr<program> const &program) {
            ctx.set_uniforms(program);
            program->set_mat4("model", model);
        });
    }

};// end namespace
