#pragma once

#include "frame.h"
#include <glad/glad.h>

//------------------------------------------------------------------------------
// Example Code:
// flatfill style{.colour=yellow};
// sphere geom;
// geom.r = 1;
// renderable spheres = create_renderable(geom, style);
// for (..) {
//     add_instance(spheres, i, at(x, y));
// }
// draw(spheres, style, camera);
//------------------------------------------------------------------------------
namespace givr {
    template <typename GeometryT, typename StyleT>
    typename StyleT::render_context create_renderable(GeometryT const &g, StyleT const &style) {
        typename StyleT::render_context ctx = get_context(g, style);
        upload_buffers(ctx, fill_buffers(g, style));
        return ctx;
    }
    template <typename GeometryT, typename StyleT>
    typename StyleT::render_context update_renderable(GeometryT const &g, StyleT const &style, typename StyleT::render_context &ctx) {
        upload_buffers(ctx, update_buffers(g, style));
        return ctx;
    }
    template <typename ContextT>
    void add_instance(ContextT &ctx, frame const &f) {
        ctx.model_transforms.push_back(f);
    }

}
