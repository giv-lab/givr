#pragma once

#include "../vertex_array_data.h"
#include "../buffer_data.h"
#include "../renderer.h"
#include "../instanced_renderer.h"
#include "../gl/program.h"
#include "static_assert.h"

#include <string>

namespace givr {

    struct NoShadingParams {
        vec3f colour;
    };

    struct NoShadingInstancedRenderContext
        : public InstancedRenderContext,
          public NoShadingParams
    {
        void setUniforms(std::unique_ptr<Program> const &p) const;

        std::string getVertexShaderSource() const;
        std::string getFragmentShaderSource() const;
    };

    struct NoShadingRenderContext
        : public RenderContext,
          public NoShadingParams
    {
        void setUniforms(std::unique_ptr<Program> const &p) const;

        std::string getVertexShaderSource() const;
        std::string getFragmentShaderSource() const;
    };

    struct NoShading : public NoShadingParams {
        using InstancedRenderContext = NoShadingInstancedRenderContext;
        using RenderContext = NoShadingRenderContext;
    };


    template <typename GeometryT>
    BufferData fillBuffers(GeometryT const &g, NoShading const &) {
        BufferData data;
        typename GeometryT::Data d = generateGeometry(g);
        static_assert(hasVertices<GeometryT>::value, "The NoShading style requires vertices. The geometry you are using does not provide them.");
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
    RenderContextT getContext(GeometryT &, NoShading const &f) {
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
    NoShading::InstancedRenderContext
    getInstancedContext(GeometryT &g, NoShading const &f) {
        return getContext<NoShading::InstancedRenderContext, GeometryT>(g, f);
    }

    template <typename GeometryT>
    NoShading::RenderContext
    getContext(GeometryT &g, NoShading const &f) {
        return getContext<NoShading::RenderContext, GeometryT>(g, f);
    }

    template <typename RenderContextT>
    void updateStyle(RenderContextT &ctx, NoShading const &f) {
        ctx.colour = f.colour;
    }

    template <typename ViewContextT>
    void draw(NoShading::InstancedRenderContext &ctx, ViewContextT const &viewCtx) {
        drawInstanced(ctx, viewCtx, [&ctx](std::unique_ptr<Program> const &program) {
            ctx.setUniforms(program);
        });
    }

    template <typename ViewContextT>
    void draw(NoShading::RenderContext &ctx, ViewContextT const &viewCtx, mat4f model=mat4f(1.f)) {
        drawArray(ctx, viewCtx, [&ctx, &model](std::unique_ptr<Program> const &program) {
            ctx.setUniforms(program);
            program->setMat4("model", model);
        });
    }
};// end namespace givr
