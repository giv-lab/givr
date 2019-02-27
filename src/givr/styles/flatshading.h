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

    struct FlatShadingParameters : public Style<
        Colour,
        LightPosition,
        SpecularFactor,
        AmbientFactor,
        PhongExponent,
        PerVertexColour
    > {
        FlatShadingParameters() {
            // Default values
            set(PerVertexColour(false));
            set(AmbientFactor(0.05f));
            set(SpecularFactor(0.3f));
            set(PhongExponent(8.0f));
        }
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

    struct FlatShadingStyle : public FlatShadingParameters {
        using InstancedRenderContext = FlatShadingInstancedRenderContext;
        using RenderContext = FlatShadingRenderContext;
    };

    template <typename... Args> FlatShadingStyle FlatShading(Args &&... args) {
        using required_args =
                std::tuple<LightPosition, Colour>;

        using namespace utility;
        static_assert(!has_duplicate_types<Args...>,
            "The arguments you passed in have duplicate parameters");

        static_assert(
            is_subset_of<required_args, std::tuple<Args...>> &&
            is_subset_of<std::tuple<Args...>, FlatShadingStyle::Args> &&
            sizeof...(args) <= std::tuple_size<FlatShadingStyle::Args>::value,
            "You have provided incorrect parameters for FlatShadingStyle. "
            "LightPosition, Colour are required. SpecularFactor, AmbientFactor, "
            "PhongExponent and PerVertexColor are optional.");
        FlatShadingStyle fs;
        fs.set(std::forward<Args>(args)...);
        return fs;
    }


    template <typename GeometryT>
    BufferData fillBuffers(GeometryT const &g, FlatShadingStyle const &) {
        BufferData data;
        typename GeometryT::Data d = generateGeometry(g);
        data.dimensions = d.dimensions;
        static_assert(hasVertices<GeometryT>::value, "The FlatShadingStyle requires vertices. The geometry you are using does not provide them.");
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
    RenderContextT getContext(GeometryT &, FlatShadingStyle const &f) {
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
    FlatShadingStyle::InstancedRenderContext
    getInstancedContext(GeometryT &g, FlatShadingStyle const &f) {
        return getContext<FlatShadingStyle::InstancedRenderContext, GeometryT>(g, f);
    }

    template <typename GeometryT>
    FlatShadingStyle::RenderContext
    getContext(GeometryT &g, FlatShadingStyle const &f) {
        return getContext<FlatShadingStyle::RenderContext, GeometryT>(g, f);
    }

    template <typename RenderContextT>
    void updateStyle(RenderContextT &ctx, FlatShadingStyle const &f) {
        ctx.set(f.args);
    }

    template <typename ViewContextT>
    void draw(FlatShadingStyle::InstancedRenderContext &ctx, ViewContextT const &viewCtx) {
        drawInstanced(ctx, viewCtx, [&ctx](std::unique_ptr<Program> const &program) {
            ctx.setUniforms(program);
        });
    }

    template <typename ViewContextT>
    void draw(FlatShadingStyle::RenderContext &ctx, ViewContextT const &viewCtx, mat4f model=mat4f(1.f)) {
        drawArray(ctx, viewCtx, [&ctx, &model](std::unique_ptr<Program> const &program) {
            ctx.setUniforms(program);
            program->setMat4("model", model);
        });
    }
}// end namespace style
}// end namespace givr
