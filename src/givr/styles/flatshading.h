#pragma once

#include "../vertex_array_data.h"
#include "../buffer_data.h"
#include "../renderer.h"
#include "../instanced_renderer.h"
#include "../gl/program.h"
#include "static_assert.h"
#include "style.h"

#include <string>

namespace givr {
namespace style {

    struct FlatShadingParameters : public Style<Colour> {
    };

    struct FlatShadingInstancedRenderContext
        : public InstancedRenderContext,
          public FlatShadingParameters
    {
        void setUniforms(std::unique_ptr<Program> const &p) const;

        std::string getVertexShaderSource() const;
        std::string getFragmentShaderSource() const;
    };

    struct FlatShadingRenderContext
        : public RenderContext,
          public FlatShadingParameters
    {
        void setUniforms(std::unique_ptr<Program> const &p) const;

        std::string getVertexShaderSource() const;
        std::string getFragmentShaderSource() const;
    };

    struct FlatShading : public FlatShadingParameters {
        using InstancedRenderContext = FlatShadingInstancedRenderContext;
        using RenderContext = FlatShadingRenderContext;
    };


    template <typename GeometryT>
    BufferData fillBuffers(GeometryT const &g, FlatShading const &) {
        BufferData data;
        typename GeometryT::Data d = generateGeometry(g);
        static_assert(hasVertices<GeometryT>::value, "The FlatShading style requires vertices. The geometry you are using does not provide them.");
        data.verticesType = d.verticesType;
        data.addVertices(d.vertices);
        if constexpr (hasIndices<GeometryT>::value) {
            data.indicesType = d.indicesType;
            data.addIndices(d.indices);
        }
        // TODO: this could probably have per vertex colouring too.
        return std::move(data);
    }

    template <typename RenderContextT, typename GeometryT>
    RenderContextT getContext(GeometryT &, FlatShading const &f) {
        RenderContextT ctx;
        ctx.shaderProgram = std::make_unique<Program>(
            Shader{ctx.getVertexShaderSource(), GL_VERTEX_SHADER},
            Shader{ctx.getFragmentShaderSource(), GL_FRAGMENT_SHADER}
        );
        ctx.primitive = getPrimitive<GeometryT>();
        updateStyle(ctx, f);
        return ctx;
    }

    template <typename GeometryT>
    FlatShading::InstancedRenderContext
    getInstancedContext(GeometryT &g, FlatShading const &f) {
        return getContext<FlatShading::InstancedRenderContext, GeometryT>(g, f);
    }

    template <typename GeometryT>
    FlatShading::RenderContext
    getContext(GeometryT &g, FlatShading const &f) {
        return getContext<FlatShading::RenderContext, GeometryT>(g, f);
    }

    template <typename RenderContextT>
    void updateStyle(RenderContextT &ctx, FlatShading const &f) {
        ctx.set(f.args);
    }

    template <typename ViewContextT>
    void draw(FlatShading::InstancedRenderContext &ctx, ViewContextT const &viewCtx) {
        drawInstanced(ctx, viewCtx, [&ctx](std::unique_ptr<Program> const &program) {
            ctx.setUniforms(program);
        });
    }

    template <typename ViewContextT>
    void draw(FlatShading::RenderContext &ctx, ViewContextT const &viewCtx, mat4f model=mat4f(1.f)) {
        drawArray(ctx, viewCtx, [&ctx, &model](std::unique_ptr<Program> const &program) {
            ctx.setUniforms(program);
            program->setMat4("model", model);
        });
    }
}// end namespace style
}// end namespace givr
