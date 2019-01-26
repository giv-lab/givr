#pragma once

#include "../vertex_array_data.h"
#include "../buffer_data.h"
#include "../instanced_renderer.h"
#include "../renderer.h"
#include "../gl/program.h"
#include "static_assert.h"

#include <string>

namespace givr {
    struct phong_parameters {
        vec3f colour;
        vec3f light_position;
        bool per_vertex_colour = false;
        float ambient_factor = 0.05f;
        float specular_factor = 0.3f;
        float phong_exponent = 8.0f;
    };

    struct phong_instanced_render_context
        :
            public phong_parameters,
            public instanced_render_context
    {
        void set_uniforms(std::unique_ptr<program> const &p) const;

        std::string get_vertex_shader_source() const;
        std::string get_fragment_shader_source() const;
    };

    struct phong_render_context
        :
            public phong_parameters,
            public render_context
    {
        void set_uniforms(std::unique_ptr<program> const &p) const;

        std::string get_vertex_shader_source() const;
        std::string get_fragment_shader_source() const;
    };

    struct phong : phong_parameters {
        using instanced_render_context = phong_instanced_render_context;
        using render_context = phong_render_context;
    };

    template <typename GeometryT>
    buffer_data fill_buffers(GeometryT const &g, phong const &) {
        buffer_data data;
        typename GeometryT::data d = generate_geometry(g);
        static_assert(
            givr::is_triangle_based<GeometryT>(),
            "The phong style requires TRIANGLES, TRIANGLE_STRIP, TRIANGLE_FAN, TRIANGLES_ADJACENCY, or TRIANGLE_STRIP_ADJACENCY for the primitive type. The geometry you use is not of this type"
        );
        static_assert(has_vertices<GeometryT>::value, "The phong style requires vertices. The geometry you are using does not provide them.");
        static_assert(has_normals<GeometryT>::value, "The phong style requires normals. The geometry you are using does not provide them.");
        data.vertices_type = d.vertices_type;
        data.add_vertices(d.vertices);
        data.normals_type = d.normals_type;
        data.add_normals(d.normals);

        if constexpr (has_indices<GeometryT>::value) {
            data.indices_type = d.indices_type;
            data.add_indices(d.indices);
        }
        if constexpr (has_colours<GeometryT>::value) {
            data.colours_type = d.colours_type;
            data.add_colours(d.colours);
        }
        return std::move(data);
    }

    template <typename RenderContextT, typename GeometryT>
    RenderContextT get_context(GeometryT &, phong const &p) {
        auto ctx = RenderContextT{};
        // TODO: this probably belongs in the constructor
        ctx.shader_program = std::make_unique<program>(
            shader{ctx.get_vertex_shader_source(), GL_VERTEX_SHADER},
            shader{ctx.get_fragment_shader_source(), GL_FRAGMENT_SHADER}
        );
        ctx.primitive = get_primitive<GeometryT>();
        update_style(ctx, p);
        return ctx;
    }

    template <typename GeometryT>
    phong::instanced_render_context
    get_instanced_context(GeometryT &g, phong const &p) {
        return get_context<phong::instanced_render_context, GeometryT>(g, p);
    }

    template <typename GeometryT>
    phong::render_context
    get_context(GeometryT &g, phong const &p) {
        return get_context<phong::render_context, GeometryT>(g, p);
    }

    template <typename RenderContextT>
    void update_style(RenderContextT &ctx, phong const &p) {
        ctx.colour = p.colour;
        ctx.light_position = p.light_position;
        ctx.per_vertex_colour = p.per_vertex_colour;
        ctx.ambient_factor = p.ambient_factor;
        ctx.specular_factor = p.specular_factor;
        ctx.phong_exponent = p.phong_exponent;
    }

    // TODO: come up with a better way to not duplicate OpenGL state setup
    template <typename ViewContextT>
    void draw(phong::instanced_render_context &ctx, ViewContextT const &view_ctx) {
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        draw_instanced(ctx, view_ctx, [&ctx](std::unique_ptr<program> const &program) {
            ctx.set_uniforms(program);
        });
    }

    template <typename ViewContextT>
    void draw(phong::render_context &ctx, ViewContextT const &view_ctx, mat4f const model=mat4f(1.f)) {
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        draw_array(ctx, view_ctx, [&ctx, &model](std::unique_ptr<program> const &program) {
            ctx.set_uniforms(program);
            program->set_mat4("model", model);
        });
    }

};// end namespace

