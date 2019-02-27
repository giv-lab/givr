#pragma once

#include "../vertex_array_data.h"
#include "../buffer_data.h"
#include "../instanced_renderer.h"
#include "../renderer.h"
#include "../gl/program.h"
#include "static_assert.h"
#include "style.h"
#include "parameters.h"

#include <string>

namespace givr {
namespace style {
    struct PhongBase : public Style<
        Colour,
        LightPosition,
        SpecularFactor,
        AmbientFactor,
        PhongExponent,
        PerVertexColour,
        ShowWireFrame,
        WireFrameColour,
        WireFramePercent,
        GenerateNormals
    > {
        PhongBase() {
            // Default values
            set(PerVertexColour(false));
            set(AmbientFactor(0.05f));
            set(SpecularFactor(0.3f));
            set(PhongExponent(8.0f));
            set(ShowWireFrame(false));
            set(WireFrameColour(0.f, 0.f, 0.f));
            set(WireFramePercent(0.02f));
            set(GenerateNormals(false));
        }
        void setUniforms(std::unique_ptr<Program> const &p) const;
    };


    struct PhongInstancedRenderContext
        :
            public PhongBase,
            public InstancedRenderContext
    {

        std::string getVertexShaderSource() const;
        std::string getGeometryShaderSource() const;
        std::string getFragmentShaderSource() const;
    };

    struct PhongRenderContext
        :
            public PhongBase,
            public RenderContext
    {
        std::string getVertexShaderSource() const;
        std::string getGeometryShaderSource() const;
        std::string getFragmentShaderSource() const;
    };

    struct PhongStyle : PhongBase {
        using InstancedRenderContext = PhongInstancedRenderContext;
        using RenderContext = PhongRenderContext;
    };

    template <typename... Args> PhongStyle Phong(Args &&... args) {
        using required_args =
                std::tuple<LightPosition, Colour>;

        using namespace utility;
        static_assert(!has_duplicate_types<Args...>,
            "The arguments you passed in have duplicate parameters");

        static_assert(
            is_subset_of<required_args, std::tuple<Args...>> &&
            is_subset_of<std::tuple<Args...>, PhongStyle::Args> &&
            sizeof...(args) <= std::tuple_size<PhongStyle::Args>::value,
            "You have provided incorrect parameters for phong. "
            "LightPosition, Colour are required. SpecularFactor, AmbientFactor, "
            "PhongExponent and PerVertexColor are optional.");
        PhongStyle p;
        p.set(std::forward<Args>(args)...);
        return p;
    }

    template <typename GeometryT>
    BufferData fillBuffers(GeometryT const &g, PhongStyle const &) {
        static_assert(
            givr::isTriangleBased<GeometryT>(),
            "The PhongStyle requires TRIANGLES, TRIANGLE_STRIP, TRIANGLE_FAN, "
            "TRIANGLES_ADJACENCY, or TRIANGLE_STRIP_ADJACENCY for the primitive "
            "type. The geometry you use is not of this type"
        );
        static_assert(
            hasVertices<GeometryT>::value,
            "The Phong style requires vertices. The geometry you are using does "
            "not provide them."
        );
        //static_assert(
            //hasNormals<GeometryT>::value,
            //"The Phong style requires normals. The geometry you are using does "
            //"not provide them."
        //);

        BufferData data;
        typename GeometryT::Data d = generateGeometry(g);
        data.dimensions = d.dimensions;
        data.verticesType = d.verticesType;
        data.addVertices(d.vertices);

        if constexpr (hasNormals<GeometryT>::value) {
            data.normalsType = d.normalsType;
            data.addNormals(d.normals);
        }

        if constexpr (hasIndices<GeometryT>::value) {
            data.indicesType = d.indicesType;
            data.addIndices(d.indices);
        }
        if constexpr (hasColours<GeometryT>::value) {
            data.coloursType = d.coloursType;
            data.addColours(d.colours);
        }
        return std::move(data);
    }

    template <typename RenderContextT, typename GeometryT>
    RenderContextT getContext(GeometryT &, PhongStyle const &p) {
        RenderContextT ctx;
        // TODO: this probably belongs in the constructor
        ctx.shaderProgram = std::make_unique<Program>(
            Shader{ctx.getVertexShaderSource(), GL_VERTEX_SHADER},
            Shader{ctx.getGeometryShaderSource(), GL_GEOMETRY_SHADER},
            Shader{ctx.getFragmentShaderSource(), GL_FRAGMENT_SHADER}
        );
        ctx.primitive = getPrimitive<GeometryT>();
        updateStyle(ctx, p);
        return ctx;
    }

    template <typename GeometryT>
    PhongStyle::InstancedRenderContext
    getInstancedContext(GeometryT &g, PhongStyle const &p) {
        return getContext<PhongStyle::InstancedRenderContext, GeometryT>(g, p);
    }

    template <typename GeometryT>
    PhongStyle::RenderContext
    getContext(GeometryT &g, PhongStyle const &p) {
        return getContext<PhongStyle::RenderContext, GeometryT>(g, p);
    }

    template <typename RenderContextT>
    void updateStyle(RenderContextT &ctx, PhongStyle const &p) {
        ctx.set(p.args);
    }

    // TODO: come up with a better way to not duplicate OpenGL state setup
    template <typename ViewContextT>
    void draw(PhongStyle::InstancedRenderContext &ctx, ViewContextT const &viewCtx) {
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        drawInstanced(ctx, viewCtx, [&ctx](std::unique_ptr<Program> const &program) {
            ctx.setUniforms(program);
        });
    }

    template <typename ViewContextT>
    void draw(PhongStyle::RenderContext &ctx, ViewContextT const &viewCtx, mat4f const model=mat4f(1.f)) {
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        drawArray(ctx, viewCtx, [&ctx, &model](std::unique_ptr<Program> const &program) {
            ctx.setUniforms(program);
            program->setMat4("model", model);
        });
    }

}// end namespace style
}// end namespace givr

