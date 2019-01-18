#pragma once

#include "types.h"
#include "gl.h"

//------------------------------------------------------------------------------
// Example Code:
// flatfill style{.colour=yellow};
// sphere geom{.radius=1};
// auto spheres = create_instanced_renderable(geom, style);
// for (..) {
//     add_instance(spheres, at(x, y));
// }
// draw(spheres, view);
//------------------------------------------------------------------------------
namespace givr {
    template <typename GeometryT, typename StyleT>
    typename StyleT::instanced_render_context
    create_instanced_renderable(GeometryT const &g, StyleT const &style) {
        typename StyleT::instanced_render_context ctx =
            get_instanced_context(g, style);
        allocate_buffers(ctx);
        upload_buffers(ctx, fill_buffers(g, style));
        return ctx;
    }
    template <typename GeometryT, typename StyleT>
    typename StyleT::render_context
    create_renderable(GeometryT const &g, StyleT const &style) {
        typename StyleT::render_context ctx =
            get_context(g, style);
        allocate_buffers(ctx);
        upload_buffers(ctx, fill_buffers(g, style));
        return ctx;
    }
    template <typename GeometryT, typename StyleT>
    void update_renderable(
        GeometryT const &g,
        StyleT const &style,
        typename StyleT::instanced_render_context &ctx
    ) {
        update_style(ctx, style);
        upload_buffers(ctx, fill_buffers(g, style));
    }
    template <typename GeometryT, typename StyleT>
    void update_renderable(
        GeometryT const &g,
        StyleT const &style,
        typename StyleT::render_context &ctx
    ) {
        update_style(ctx, style);
        upload_buffers(ctx, fill_buffers(g, style));
    }
    template <typename ContextT>
    void add_instance(ContextT &ctx, glm::mat4 const &f) {
        ctx.model_transforms.push_back(f);
    }

}
