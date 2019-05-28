#pragma once

#include "../vertex_array_data.h"
#include "../buffer_data.h"
#include "../renderer.h"
#include "../instanced_renderer.h"
#include "../gl/program.h"
#include "../static_assert.h"
#include "style.h"
#include "parameters.h"

#include <string>

namespace givr {
namespace style {
    struct NoShadingParameters : public Style<Colour> {};

    struct NoShading : public NoShadingParameters {
        using Parameters = NoShadingParameters;
        template <typename... Args>
        NoShading(Args &&... args) {
            using required_args = std::tuple<Colour>;

            using namespace utility;
            static_assert(!has_duplicate_types<Args...>,
                "The arguments you passed in have duplicate parameters");

            static_assert(is_subset_of<required_args, std::tuple<Args...>>,
                "Colour is a required parameter for NoShading. Please provide it.");
            static_assert(is_subset_of<std::tuple<Args...>, NoShading::Args>,
                "You have provided incorrect parameters for NoShading. "
                "Colour is required.");
            static_assert(sizeof...(args) <= std::tuple_size<NoShading::Args>::value,
                "You have provided incorrect parameters for NoShading. "
                "Colour is required.");

            set(std::forward<Args>(args)...);
        }

    };
    using NoShadingStyle = NoShading;

    template <typename RenderContextT>
    void setNoShadingUniforms(RenderContextT const &ctx, std::unique_ptr<givr::Program> const &p) {
        p->setVec3("colour", ctx.params.template value<givr::style::Colour>());
    }

    std::string noShadingVertexSource(std::string modelSource);
    std::string noShadingFragmentSource();

    template <typename GeometryT>
    typename GeometryT::Data fillBuffers(GeometryT const &g, NoShading const &) {
        static_assert(hasVertices<GeometryT>::value, "The NoShading style requires vertices. The geometry you are using does not provide them.");
        return std::move(generateGeometry(g));
    }

    template <typename GeometryT>
    RenderContext<GeometryT, NoShading> getContext(GeometryT const &, NoShading const &f) {
        std::cout << "NoShading" << std::endl;
        RenderContext<GeometryT, NoShading> ctx;
        ctx.shaderProgram = std::make_unique<Program>(
            Shader{noShadingVertexSource(ctx.getModelSource()), GL_VERTEX_SHADER},
            Shader{noShadingFragmentSource(), GL_FRAGMENT_SHADER}
        );
        ctx.primitive = getPrimitive<GeometryT>();
        updateStyle(ctx, f);
        return ctx;
    }

    template <typename RenderContextT>
    void updateStyle(RenderContextT &ctx, NoShading const &f) {
        ctx.params.set(f.args);
    }

    template <typename GeometryT, typename ViewContextT>
    void draw(InstancedRenderContext<GeometryT, NoShading> &ctx, ViewContextT const &viewCtx) {
        drawInstanced(ctx, viewCtx, [&ctx](std::unique_ptr<Program> const &program) {
            setNoShadingUniforms(ctx, program);
        });
    }

    template <typename GeometryT, typename ViewContextT>
    void draw(RenderContext<GeometryT, NoShading> &ctx, ViewContextT const &viewCtx, mat4f model=mat4f(1.f)) {
        drawArray(ctx, viewCtx, [&ctx, &model](std::unique_ptr<Program> const &program) {
            setNoShadingUniforms(ctx, program);
            program->setMat4("model", model);
        });
    }
}// end namespace style
}// end namespace givr
