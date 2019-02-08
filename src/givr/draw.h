#pragma once

#include "types.h"
#include "gl.h"

//------------------------------------------------------------------------------
// Example Code:
// flatfill style{.colour=yellow};
// Sphere geom{.radius=1};
// auto spheres = createInstancedRenderable(geom, style);
// for (..) {
//     addInstance(spheres, at(x, y));
// }
// draw(spheres, view);
//------------------------------------------------------------------------------
namespace givr {
    template <typename GeometryT, typename StyleT>
    typename StyleT::InstancedRenderContext
    createInstancedRenderable(GeometryT const &g, StyleT const &style) {
        typename StyleT::InstancedRenderContext ctx =
            getInstancedContext(g, style);
        allocateBuffers(ctx);
        uploadBuffers(ctx, fillBuffers(g, style));
        return ctx;
    }
    template <typename GeometryT, typename StyleT>
    typename StyleT::RenderContext
    createRenderable(GeometryT const &g, StyleT const &style) {
        typename StyleT::RenderContext ctx =
            getContext(g, style);
        allocateBuffers(ctx);
        uploadBuffers(ctx, fillBuffers(g, style));
        return ctx;
    }
    template <typename GeometryT, typename StyleT>
    void updateRenderable(
        GeometryT const &g,
        StyleT const &style,
        typename StyleT::InstancedRenderContext &ctx
    ) {
        updateStyle(ctx, style);
        uploadBuffers(ctx, fillBuffers(g, style));
    }
    template <typename GeometryT, typename StyleT>
    void updateRenderable(
        GeometryT const &g,
        StyleT const &style,
        typename StyleT::RenderContext &ctx
    ) {
        updateStyle(ctx, style);
        uploadBuffers(ctx, fillBuffers(g, style));
    }
    template <typename ContextT>
    void addInstance(ContextT &ctx, glm::mat4 const &f) {
        ctx.modelTransforms.push_back(f);
    }

}
