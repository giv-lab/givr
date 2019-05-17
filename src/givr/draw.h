#pragma once

#include "types.h"
#include "gl.h"
#include "renderer.h"
#include "instanced_renderer.h"

//------------------------------------------------------------------------------
// Example Code:
// FlatShading style{.colour=yellow};
// Sphere geom{.radius=1};
// auto spheres = createInstancedRenderable(geom, style);
// for (..) {
//     addInstance(spheres, at(x, y));
// }
// draw(spheres, view);
//------------------------------------------------------------------------------
namespace givr {
    template <typename GeometryT, typename StyleT>
    InstancedRenderContext<GeometryT, StyleT>
    createInstancedRenderable(GeometryT const &g, StyleT const &style) {
        auto ctx = getInstancedContext(g, style);
        allocateBuffers(ctx);
        uploadBuffers(ctx, fillBuffers(g, style));
        return ctx;
    }
    template <typename GeometryT, typename StyleT>
    RenderContext<GeometryT, StyleT>
    createRenderable(GeometryT const &g, StyleT const &style) {
        auto ctx = getContext(g, style);
        allocateBuffers(ctx);
        uploadBuffers(ctx, fillBuffers(g, style));
        return ctx;
    }
    template <typename GeometryT, typename StyleT>
    void updateRenderable(
        GeometryT const &g,
        StyleT const &style,
        InstancedRenderContext<GeometryT, StyleT> &ctx
    ) {
        updateStyle(ctx, style);
        uploadBuffers(ctx, fillBuffers(g, style));
    }
    template <typename GeometryT, typename StyleT>
    void updateRenderable(
        GeometryT const &g,
        StyleT const &style,
        RenderContext<GeometryT, StyleT> &ctx
    ) {
        updateStyle(ctx, style);
        uploadBuffers(ctx, fillBuffers(g, style));
    }
    template <typename GeometryT, typename StyleT>
    void addInstance(InstancedRenderContext<GeometryT, StyleT> &ctx, glm::mat4 const &f) {
        ctx.modelTransforms.push_back(f);
    }

}
