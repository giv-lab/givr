#pragma once

#include "../vertex_array_data.h"
#include "../buffer_data.h"
#include "../renderer.h"
#include "../instanced_renderer.h"
#include "../gl/program.h"
#include "static_assert.h"
#include "style.h"
#include "parameters.h"

#include <string>

namespace givr {
namespace style {

    struct LineParameters : public Style<Colour, Width> {
        LineParameters() {
            set(Width(1.0));
        }
    };

    struct LineRenderContext
        : public RenderContext,
          public LineParameters
    {

        void setUniforms(std::unique_ptr<Program> const &p) const;

        std::string getVertexShaderSource() const;
        std::string getFragmentShaderSource() const;
    };

    struct LineInstancedRenderContext
        : public InstancedRenderContext,
          public LineParameters
    {

        void setUniforms(std::unique_ptr<Program> const &p) const;

        std::string getVertexShaderSource() const;
        std::string getFragmentShaderSource() const;
    };

    struct LineStyle : public LineParameters {
        using RenderContext = LineRenderContext;
        using InstancedRenderContext = LineInstancedRenderContext;
    };

    template <typename... Args> LineStyle GL_Line(Args &&... args) {
        using required_args = std::tuple<Colour>;

        using namespace utility;
        static_assert(!has_duplicate_types<Args...>,
            "The arguments you passed in have duplicate parameters");

        static_assert(is_subset_of<required_args, std::tuple<Args...>>,
            "Colour is a required parameter for LineStyle. Please provide it.");
        static_assert(is_subset_of<std::tuple<Args...>, LineStyle::Args>,
            "You have provided incorrect parameters for LineStyle. "
            "Colour is required. Width is optional.");
        static_assert(sizeof...(args) <= std::tuple_size<LineStyle::Args>::value,
            "You have provided incorrect parameters for LineStyle. "
            "Colour is required. Width is optional.");
        LineStyle ns;
        ns.set(std::forward<Args>(args)...);
        return ns;
    }

    template <typename GeometryT>
    BufferData fillBuffers(GeometryT const &g, LineStyle const &) {
        static_assert(
            givr::isLineBased<GeometryT>(),
            R"error(
            The LineStyle style requires LINES, LINE_LOOP, LINE_STRIP,
            LINES_ADJACENCY, or LINE_STRIP_ADJACENCY for the primitive
            type. The geometry you use is not of this type"
            )error"
        );
        static_assert(
            hasVertices<GeometryT>::value,
            R"error(
            The LineStyle style requires vertices. The geometry you are using
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
    RenderContextT getContext(GeometryT &, LineStyle const &l) {
        RenderContextT ctx;
        ctx.shaderProgram = std::make_unique<Program>(
            Shader{ctx.getVertexShaderSource(), GL_VERTEX_SHADER},
            Shader{ctx.getFragmentShaderSource(), GL_FRAGMENT_SHADER}
        );
        ctx.primitive = getPrimitive<GeometryT>();
        updateStyle(ctx, l);
        return ctx;
    }

    template <typename GeometryT>
    LineStyle::RenderContext
    getContext(GeometryT &g, LineStyle const &l) {
        return getContext<LineStyle::RenderContext, GeometryT>(g, l);
    }

    template <typename GeometryT>
    LineStyle::InstancedRenderContext
    getInstancedContext(GeometryT &g, LineStyle const &l) {
        return getContext<LineStyle::InstancedRenderContext, GeometryT>(g, l);
    }

    template <typename RenderContextT>
    void updateStyle(RenderContextT &ctx, LineStyle const &l) {
        ctx.set(l.args);
    }

    template <typename ViewContextT>
    void draw(LineStyle::InstancedRenderContext &ctx, ViewContextT const &viewCtx) {
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(ctx.value<Width>());
        drawInstanced(ctx, viewCtx, [&ctx](std::unique_ptr<Program> const &program) {
            ctx.setUniforms(program);
        });
    }

    template <typename ViewContextT>
    void draw(LineStyle::RenderContext &ctx, ViewContextT const &viewCtx, mat4f model=mat4f(1.f)) {
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(ctx.value<Width>());
        drawArray(ctx, viewCtx, [&ctx, &model](std::unique_ptr<Program> const &program) {
            ctx.setUniforms(program);
            program->setMat4("model", model);
        });
    }

}// end style
}// end namespace
