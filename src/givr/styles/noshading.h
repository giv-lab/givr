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

    struct NoShadingParameters : public Style<Colour> {
    };

    struct NoShadingInstancedRenderContext
        : public InstancedRenderContext,
          public NoShadingParameters
    {
        void setUniforms(std::unique_ptr<Program> const &p) const;

        std::string getVertexShaderSource() const;
        std::string getFragmentShaderSource() const;
    };

    struct NoShadingRenderContext
        : public RenderContext,
          public NoShadingParameters
    {
        void setUniforms(std::unique_ptr<Program> const &p) const;

        std::string getVertexShaderSource() const;
        std::string getFragmentShaderSource() const;
    };

    struct NoShadingStyle : public NoShadingParameters {
        using InstancedRenderContext = NoShadingInstancedRenderContext;
        using RenderContext = NoShadingRenderContext;
    };

    template <typename... Args> NoShadingStyle NoShading(Args &&... args) {
        using required_args = std::tuple<Colour>;

        using namespace utility;
        static_assert(!has_duplicate_types<Args...>,
            "The arguments you passed in have duplicate parameters");

        static_assert(is_subset_of<required_args, std::tuple<Args...>>,
            "Colour is a required parameter for NoShadingStyle. Please provide it.");
        static_assert(is_subset_of<std::tuple<Args...>, NoShadingStyle::Args>,
            "You have provided incorrect parameters for NoShadingStyle. "
            "Colour is required.");
        static_assert(sizeof...(args) <= std::tuple_size<NoShadingStyle::Args>::value,
            "You have provided incorrect parameters for NoShadingStyle. "
            "Colour is required.");
        NoShadingStyle ns;
        ns.set(std::forward<Args>(args)...);
        return ns;
    }

    template <typename GeometryT>
    BufferData fillBuffers(GeometryT const &g, NoShadingStyle const &) {
        BufferData data;
        typename GeometryT::Data d = generateGeometry(g);
        data.dimensions = d.dimensions;
        static_assert(hasVertices<GeometryT>::value, "The NoShadingStyle style requires vertices. The geometry you are using does not provide them.");
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
    RenderContextT getContext(GeometryT &, NoShadingStyle const &f) {
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
    NoShadingStyle::InstancedRenderContext
    getInstancedContext(GeometryT &g, NoShadingStyle const &f) {
        return getContext<NoShadingStyle::InstancedRenderContext, GeometryT>(g, f);
    }

    template <typename GeometryT>
    NoShadingStyle::RenderContext
    getContext(GeometryT &g, NoShadingStyle const &f) {
        return getContext<NoShadingStyle::RenderContext, GeometryT>(g, f);
    }

    template <typename RenderContextT>
    void updateStyle(RenderContextT &ctx, NoShadingStyle const &f) {
        ctx.set(f.args);
    }

    template <typename ViewContextT>
    void draw(NoShadingStyle::InstancedRenderContext &ctx, ViewContextT const &viewCtx) {
        drawInstanced(ctx, viewCtx, [&ctx](std::unique_ptr<Program> const &program) {
            ctx.setUniforms(program);
        });
    }

    template <typename ViewContextT>
    void draw(NoShadingStyle::RenderContext &ctx, ViewContextT const &viewCtx, mat4f model=mat4f(1.f)) {
        drawArray(ctx, viewCtx, [&ctx, &model](std::unique_ptr<Program> const &program) {
            ctx.setUniforms(program);
            program->setMat4("model", model);
        });
    }
}// end namespace style
}// end namespace givr
