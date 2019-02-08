#pragma once

#include "../vertex_array_data.h"
#include "../buffer_data.h"
#include "../instanced_renderer.h"
#include "../renderer.h"
#include "../gl/program.h"
#include "static_assert.h"

#include <string>

namespace givr {
    struct PhongParameters {
        vec3f colour;
        vec3f lightPosition;
        bool perVertexColour = false;
        float ambientFactor = 0.05f;
        float specularFactor = 0.3f;
        float phongExponent = 8.0f;
    };

    struct PhongInstancedRenderContext
        :
            public PhongParameters,
            public InstancedRenderContext
    {
        void setUniforms(std::unique_ptr<Program> const &p) const;

        std::string getVertexShaderSource() const;
        std::string getFragmentShaderSource() const;
    };

    struct PhongRenderContext
        :
            public PhongParameters,
            public RenderContext
    {
        void setUniforms(std::unique_ptr<Program> const &p) const;

        std::string getVertexShaderSource() const;
        std::string getFragmentShaderSource() const;
    };

    struct Phong : PhongParameters {
        using InstancedRenderContext = PhongInstancedRenderContext;
        using RenderContext = PhongRenderContext;
    };

    template <typename GeometryT>
    BufferData fillBuffers(GeometryT const &g, Phong const &) {
        static_assert(
            givr::isTriangleBased<GeometryT>(),
            "The Phong style requires TRIANGLES, TRIANGLE_STRIP, TRIANGLE_FAN, TRIANGLES_ADJACENCY, or TRIANGLE_STRIP_ADJACENCY for the primitive type. The geometry you use is not of this type"
        );
        static_assert(hasVertices<GeometryT>::value, "The Phong style requires vertices. The geometry you are using does not provide them.");
        static_assert(hasNormals<GeometryT>::value, "The Phong style requires normals. The geometry you are using does not provide them.");

        BufferData data;
        typename GeometryT::Data d = generateGeometry(g);
        data.verticesType = d.verticesType;
        data.addVertices(d.vertices);
        data.normalsType = d.normalsType;
        data.addNormals(d.normals);

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
    RenderContextT getContext(GeometryT &, Phong const &p) {
        auto ctx = RenderContextT{};
        // TODO: this probably belongs in the constructor
        ctx.shaderProgram = std::make_unique<Program>(
            Shader{ctx.getVertexShaderSource(), GL_VERTEX_SHADER},
            Shader{ctx.getFragmentShaderSource(), GL_FRAGMENT_SHADER}
        );
        ctx.primitive = getPrimitive<GeometryT>();
        updateStyle(ctx, p);
        return ctx;
    }

    template <typename GeometryT>
    Phong::InstancedRenderContext
    getInstancedContext(GeometryT &g, Phong const &p) {
        return getContext<Phong::InstancedRenderContext, GeometryT>(g, p);
    }

    template <typename GeometryT>
    Phong::RenderContext
    getContext(GeometryT &g, Phong const &p) {
        return getContext<Phong::RenderContext, GeometryT>(g, p);
    }

    template <typename RenderContextT>
    void updateStyle(RenderContextT &ctx, Phong const &p) {
        ctx.colour = p.colour;
        ctx.lightPosition = p.lightPosition;
        ctx.perVertexColour = p.perVertexColour;
        ctx.ambientFactor = p.ambientFactor;
        ctx.specularFactor = p.specularFactor;
        ctx.phongExponent = p.phongExponent;
    }

    // TODO: come up with a better way to not duplicate OpenGL state setup
    template <typename ViewContextT>
    void draw(Phong::InstancedRenderContext &ctx, ViewContextT const &viewCtx) {
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
    void draw(Phong::RenderContext &ctx, ViewContextT const &viewCtx, mat4f const model=mat4f(1.f)) {
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

};// end namespace

