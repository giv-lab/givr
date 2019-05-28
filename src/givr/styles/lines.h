#pragma once

#include "../vertex_array_data.h"
#include "../renderer.h"
#include "../instanced_renderer.h"
#include "../gl/program.h"
#include "../static_assert.h"
#include "style.h"
#include "parameters.h"

#include <string>

namespace givr {
namespace style {
    struct GL_LineParameters : public Style<Colour, Width> { };

    struct GL_Line : public GL_LineParameters {
        using Parameters = GL_LineParameters;
        template <typename... Args>
        GL_Line(Args &&... args) {
            using required_args = std::tuple<Colour>;

            using namespace utility;
            static_assert(!has_duplicate_types<Args...>,
                "The arguments you passed in have duplicate parameters");

            static_assert(is_subset_of<required_args, std::tuple<Args...>>,
                "Colour is a required parameter for GL_Line. Please provide it.");
            static_assert(is_subset_of<std::tuple<Args...>, GL_Line::Args>,
                "You have provided incorrect parameters for GL_Line. "
                "Colour is required. Width is optional.");
            static_assert(sizeof...(args) <= std::tuple_size<GL_Line::Args>::value,
                "You have provided incorrect parameters for GL_Line. "
                "Colour is required. Width is optional.");
            set(Width(1.0));
            set(std::forward<Args>(args)...);
        }
    };

    using LineStyle = GL_Line; // Backwards compatibility

    template <typename GeometryT>
    typename GeometryT::Data fillBuffers(GeometryT const &g, GL_Line const &) {
        static_assert(
            givr::isLineBased<GeometryT>(),
            R"error(
            The GL_Line style requires LINES, LINE_LOOP, LINE_STRIP,
            LINES_ADJACENCY, or LINE_STRIP_ADJACENCY for the primitive
            type. The geometry you use is not of this type"
            )error"
        );
        static_assert(
            hasVertices<GeometryT>::value,
            R"error(
            The GL_Line style requires vertices. The geometry you are using
            does not provide them.
            )error"
        );
        return std::move(generateGeometry(g));
    }

    template <typename RenderContextT>
    void setLineUniforms(RenderContextT const &ctx, std::unique_ptr<givr::Program> const &p) {
        p->setVec3("colour", ctx.params.template value<Colour>());
    }
    std::string linesVertexSource(std::string modelSource);
    std::string linesFragmentSource();

    template <typename GeometryT>
    RenderContext<GeometryT, GL_Line> getContext(GeometryT const &, GL_Line const &l) {
        RenderContext<GeometryT, GL_Line> ctx;
        ctx.shaderProgram = std::make_unique<Program>(
            Shader{linesVertexSource(ctx.getModelSource()), GL_VERTEX_SHADER},
            Shader{linesFragmentSource(), GL_FRAGMENT_SHADER}
        );
        ctx.primitive = getPrimitive<GeometryT>();
        updateStyle(ctx, l);
        return ctx;
    }

    template <typename RenderContextT>
    void updateStyle(RenderContextT &ctx, GL_Line const &l) {
        ctx.params.set(l.args);
    }

    template <typename GeometryT, typename ViewContextT>
    void draw(InstancedRenderContext<GeometryT, GL_Line> &ctx, ViewContextT const &viewCtx) {
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(ctx.params.template value<Width>());
        drawInstanced(ctx, viewCtx, [&ctx](std::unique_ptr<Program> const &program) {
            setLineUniforms(ctx, program);
        });
    }

    template <typename GeometryT, typename ViewContextT>
    void draw(RenderContext<GeometryT, GL_Line> &ctx, ViewContextT const &viewCtx, mat4f model=mat4f(1.f)) {
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(ctx.params.template value<Width>());
        drawArray(ctx, viewCtx, [&ctx, &model](std::unique_ptr<Program> const &program) {
            setLineUniforms(ctx, program);
            program->setMat4("model", model);
        });
    }

}// end style
}// end namespace
