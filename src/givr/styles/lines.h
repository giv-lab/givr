#pragma once

#include "../vertex_array_data.h"
#include "../buffer_data.h"
#include "../renderer.h"
#include "../instanced_renderer.h"
#include "../gl/program.h"
#include "static_assert.h"

#include <string>

namespace givr {

    struct LinesParams {
        vec3f colour;
        float lineWidth = 1.f;
    };

    struct LinesRenderContext
        : public RenderContext,
          public LinesParams
    {

        void setUniforms(std::unique_ptr<Program> const &p) const;

        std::string getVertexShaderSource() const;
        std::string getFragmentShaderSource() const;
    };

    struct LinesInstancedRenderContext
        : public InstancedRenderContext,
          public LinesParams
    {

        void setUniforms(std::unique_ptr<Program> const &p) const;

        std::string getVertexShaderSource() const;
        std::string getFragmentShaderSource() const;
    };

    struct Lines : public LinesParams {
        using RenderContext = LinesRenderContext;
        using InstancedRenderContext = LinesInstancedRenderContext;
    };

    template <typename GeometryT>
    BufferData fillBuffers(GeometryT const &g, Lines const &) {
        static_assert(
            givr::isLineBased<GeometryT>(),
            R"error(
            The Lines style requires LINES, LINE_LOOP, LINE_STRIP,
            LINES_ADJACENCY, or LINE_STRIP_ADJACENCY for the primitive
            type. The geometry you use is not of this type"
            )error"
        );
        static_assert(
            hasVertices<GeometryT>::value,
            R"error(
            The Lines style requires vertices. The geometry you are using
            does not provide them.
            )error"
        );
        BufferData data;
        typename GeometryT::Data d = generateGeometry(g);
        data.addVertices(d.vertices);

        if constexpr (hasIndices<GeometryT>::value) {
            data.indicesType = d.indicesType;
            data.addIndices(d.indices);
        }

        // TODO: This could optionally support per vertex colouring too.
        return std::move(data);
    }

    template <typename RenderContextT, typename GeometryT>
    RenderContextT getContext(GeometryT &, Lines const &l) {
        RenderContextT ctx;
        ctx.shaderProgram = std::make_unique<Program>(
            Shader{ctx.getVertexShaderSource(), GL_VERTEX_SHADER},
            Shader{ctx.getFragmentShaderSource(), GL_FRAGMENT_SHADER}
        );
        // TODO: Want a compile time guard to ensure geometry and style are compatible.
        ctx.primitive = getPrimitive<GeometryT>();
        updateStyle(ctx, l);
        return ctx;
    }

    template <typename GeometryT>
    Lines::RenderContext
    getContext(GeometryT &g, Lines const &l) {
        return getContext<Lines::RenderContext, GeometryT>(g, l);
    }

    template <typename GeometryT>
    Lines::InstancedRenderContext
    getInstancedContext(GeometryT &g, Lines const &l) {
        return getContext<Lines::InstancedRenderContext, GeometryT>(g, l);
    }

    template <typename RenderContextT>
    void updateStyle(RenderContextT &ctx, Lines const &l) {
        ctx.colour = l.colour;
        ctx.lineWidth = l.lineWidth;
    }

    template <typename ViewContextT>
    void draw(Lines::InstancedRenderContext &ctx, ViewContextT const &viewCtx) {
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(ctx.lineWidth);
        drawInstanced(ctx, viewCtx, [&ctx](std::unique_ptr<Program> const &program) {
            ctx.setUniforms(program);
        });
    }

    template <typename ViewContextT>
    void draw(Lines::RenderContext &ctx, ViewContextT const &viewCtx, mat4f model=mat4f(1.f)) {
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(ctx.lineWidth);
        drawArray(ctx, viewCtx, [&ctx, &model](std::unique_ptr<Program> const &program) {
            ctx.setUniforms(program);
            program->setMat4("model", model);
        });
    }

};// end namespace
