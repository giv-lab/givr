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

        template<typename ColorSrc>
        struct T_PhongParameters : public Style<
            ColorSrc,
            LightPosition,
            SpecularFactor,
            AmbientFactor,
            PhongExponent,
            PerVertexColour> {
            T_PhongParameters() {
                // Default values
                set(PerVertexColour(false));
                set(AmbientFactor(0.05f));
                set(SpecularFactor(0.3f));
                set(PhongExponent(8.0f));
            }
        };


        std::string phongVertexSource(std::string modelSource, bool usingTexture);
        std::string phongFragmentSource(bool usingTexture);

        template <typename RenderContextT>
        void setPhongUniforms(RenderContextT const &ctx, std::unique_ptr<givr::Program> const &p) {
            using namespace givr::style;
            p->setVec3("colour", ctx.template value<Colour>());
            p->setVec3("lightPosition", ctx.template value<LightPosition>());
            p->setFloat("ambientFactor", ctx.template value<AmbientFactor>());
            p->setFloat("specularFactor", ctx.template value<SpecularFactor>());
            p->setFloat("phongExponent", ctx.template value<PhongExponent>());
            p->setBool("perVertexColour", ctx.template value<PerVertexColour>());
        }

        template <typename RenderContextT>
        void setTexturedPhongUniforms(RenderContextT const & ctx, std::unique_ptr<givr::Program> const &p) {
            using namespace givr::style;
            givr::Texture texture = ctx.template value<ColorTexture>();
            if (GLuint(texture)) {
                glActiveTexture(GL_TEXTURE1);
                texture.bind(GL_TEXTURE_2D);
                p->setInt("colorTexture", 1);
                glActiveTexture(GL_TEXTURE0);
            }
            p->setVec3("lightPosition", ctx.template value<LightPosition>());
            p->setFloat("ambientFactor", ctx.template value<AmbientFactor>());
            p->setFloat("specularFactor", ctx.template value<SpecularFactor>());
            p->setFloat("phongExponent", ctx.template value<PhongExponent>());
            p->setBool("perVertexColour", ctx.template value<PerVertexColour>());
        }

        template<typename ColorSrc>
        struct T_PhongInstancedRenderContext
            :
            public T_PhongParameters<ColorSrc>,
            public InstancedRenderContext
        {
            void setUniforms(std::unique_ptr<Program> const &p) const {
                if constexpr (std::is_same<ColorSrc, ColorTexture>::value)
                    setTexturedPhongUniforms(*this, p);
                else
                    setPhongUniforms(*this, p);
            }

            std::string getVertexShaderSource() const { return phongVertexSource("in", std::is_same<ColorSrc, ColorTexture>::value); }
            std::string getFragmentShaderSource() const { return phongFragmentSource(std::is_same<ColorSrc, ColorTexture>::value); }
        };

        template<typename ColorSrc>
        struct T_PhongRenderContext
            :
            public T_PhongParameters<ColorSrc>,
            public RenderContext
        {
            void setUniforms(std::unique_ptr<Program> const &p) const {
                if constexpr (std::is_same<ColorSrc, ColorTexture>::value)
                    setTexturePhongUniforms(*this, p);
                else
                    setPhongUniforms(*this, p);

            }

            std::string getVertexShaderSource() const { return phongVertexSource("uniform", std::is_same<ColorSrc, ColorTexture>::value); }
            std::string getFragmentShaderSource() const { return phongFragmentSource(std::is_same<ColorSrc, ColorTexture>::value); }
        };

        template<typename ColorSrc>
        struct T_PhongStyle : T_PhongParameters<ColorSrc> {
            using InstancedRenderContext = T_PhongInstancedRenderContext<ColorSrc>;
            using RenderContext = T_PhongRenderContext<ColorSrc>;
        };

        template <typename ColorSrc, typename... Args>
        T_PhongStyle<ColorSrc> T_Phong(Args &&... args) {
            using required_args =
                std::tuple<LightPosition, ColorSrc>;

            using namespace style;
            using namespace utility;

            static_assert(
                is_subset_of<
                std::tuple<LightPosition>,
                std::tuple<Args...>>,
                "testing is_subset_of");
            static_assert(!has_duplicate_types<Args...>,
                "The arguments you passed in have duplicate parameters");

            static_assert(is_subset_of<required_args, std::tuple<Args...>>,
                "LightPosition and Colour/ColorTexture are required parameters "
                "for T_Phong."
                "provide them.");
            static_assert(is_subset_of<std::tuple<Args...>, T_PhongStyle<ColorSrc>::Args>,
                "You have provided incorrect parameters for phong. "
                "LightPosition and Colour/ColorTexture are required"
                "PhongExponent and PerVertexColor are optional.");
            static_assert(sizeof...(args) <= std::tuple_size<T_PhongStyle<ColorSrc>::Args>::value,
                "You have provided incorrect parameters for phong. "
                "LightPosition and Colour/ColorTexture are required"
                "PhongExponent and PerVertexColor are optional.");

            T_PhongStyle<ColorSrc> p;
            p.set(std::forward<Args>(args)...);
            return p;
        }

        template <typename GeometryT, typename ColorSrc>
        BufferData fillBuffers(GeometryT const &g, T_PhongStyle<ColorSrc> const &) {
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
            static_assert(
                hasNormals<GeometryT>::value,
                "The Phong style requires normals. The geometry you are using does "
                "not provide them."
                );
            static_assert(
                hasUvs<GeometryT>::value || std::is_same<ColorSrc, Colour>::value,
                "The Phong Texture style requires uvs. The geometry you are using does "
                "not provide them."
                );

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
            if constexpr (hasColours<GeometryT>::value && !std::is_same<ColorSrc, Colour>::value) {
                data.coloursType = d.coloursType;
                data.addColours(d.colours);
            }
            if constexpr (hasUvs<GeometryT>::value && std::is_same<ColorSrc, ColorTexture>::value) {
                data.uvsType = d.uvsType;
                data.addUvs(d.uvs);
            }

            return std::move(data);
        }

        template <typename RenderContextT, typename GeometryT, typename ColorSrc>
        RenderContextT getContext(GeometryT &, T_PhongStyle<ColorSrc> const &p) {
            RenderContextT ctx;
            // TODO: this probably belongs in the constructor
            ctx.shaderProgram = std::make_unique<Program>(
                Shader{ ctx.getVertexShaderSource(), GL_VERTEX_SHADER },
                Shader{ ctx.getFragmentShaderSource(), GL_FRAGMENT_SHADER }
            );
            ctx.primitive = getPrimitive<GeometryT>();
            updateStyle(ctx, p);
            return ctx;
        }

        template <typename GeometryT, typename ColorSrc>
        typename T_PhongStyle<ColorSrc>::InstancedRenderContext
            getInstancedContext(GeometryT &g, T_PhongStyle<ColorSrc> const &p) {
            return getContext<T_PhongStyle<ColorSrc>::InstancedRenderContext, GeometryT>(g, p);
        }

        template <typename GeometryT, typename ColorSrc>
        typename T_PhongStyle<ColorSrc>::RenderContext
            getContext(GeometryT &g, T_PhongStyle<ColorSrc> const &p) {
            return getContext<T_PhongStyle<ColorSrc>::RenderContext, GeometryT>(g, p);
        }

        template <typename RenderContextT, typename ColorSrc>
        void updateStyle(RenderContextT &ctx, T_PhongStyle<ColorSrc> const &p) {
            ctx.set(p.args);
        }

        // TODO: come up with a better way to not duplicate OpenGL state setup
        template <typename ViewContextT, typename ColorSrc>
        void draw(typename T_PhongInstancedRenderContext<ColorSrc> &ctx, ViewContextT const &viewCtx) {
            glEnable(GL_MULTISAMPLE);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            drawInstanced(ctx, viewCtx, [&ctx](std::unique_ptr<Program> const &program) {
                ctx.setUniforms(program);
            });
        }

        template <typename ViewContextT, typename ColorSrc>
        void draw(T_PhongRenderContext<ColorSrc>& ctx, ViewContextT const &viewCtx, mat4f const model = mat4f(1.f)) {
            glEnable(GL_MULTISAMPLE);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            drawArray(ctx, viewCtx, [&ctx, &model](std::unique_ptr<Program> const &program) {
                ctx.setUniforms(program);
                program->setMat4("model", model);
            });
        }


        using TexturedPhongStyle = T_PhongStyle<ColorTexture>;
        using TexturedPhongRenderContext = T_PhongRenderContext<ColorTexture>;
        using TexturedPhongInstancedRenderContext = T_PhongInstancedRenderContext<ColorTexture>;
        template<typename... Args>
        TexturedPhongStyle TexturedPhong(Args &&... args) { return T_Phong<ColorTexture>(std::forward<Args>(args)...); }
        using PhongStyle = T_PhongStyle<Colour>;
        using PhongRenderContext = T_PhongRenderContext<Colour>;
        using PhongInstancedRenderContext = T_PhongInstancedRenderContext<Colour>;
        template<typename... Args>
        PhongStyle Phong(Args &&... args) { return T_Phong<Colour>(std::forward<Args>(args)...); }

    }// end namespace style
}// end namespace givr

