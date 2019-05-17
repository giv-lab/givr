#pragma once

#include "../vertex_array_data.h"
#include "../buffer_data.h"
#include "../instanced_renderer.h"
#include "../renderer.h"
#include "../gl/program.h"
#include "../static_assert.h"
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
            PerVertexColour,
            ShowWireFrame,
            WireFrameColour,
            WireFrameWidth,
            GenerateNormals
        > {
        };

        template<typename ColorSrc>
        struct T_Phong : T_PhongParameters<ColorSrc> {
            using Parameters = T_PhongParameters<ColorSrc>;
            template <typename... T_PhongArgs>
            T_Phong(T_PhongArgs &&... args) {
                using required_args =
                    std::tuple<LightPosition, ColorSrc>;

                using namespace style;
                using namespace utility;

                static_assert(!has_duplicate_types<T_PhongArgs...>,
                    "The arguments you passed in have duplicate parameters");

                static_assert(
                    is_subset_of<required_args, std::tuple<T_PhongArgs...>> &&
                    is_subset_of<std::tuple<T_PhongArgs...>, typename T_Phong<ColorSrc>::Args> &&
                    sizeof...(args) <= std::tuple_size<typename T_Phong<ColorSrc>::Args>::value,
                    "You have provided incorrect parameters for phong. "
                    "LightPosition and (Colour or ColorTexture) are required "
                    "AmbientFactor, SpecularFactor PhongExponent and PerVertexColor "
                    "are optional.");

                this->set(PerVertexColour(false));
                this->set(AmbientFactor(0.05f));
                this->set(SpecularFactor(0.3f));
                this->set(PhongExponent(8.0f));
                this->set(ShowWireFrame(false));
                this->set(WireFrameColour(0.f, 0.f, 0.f));
                this->set(WireFrameWidth(1.5f));
                this->set(GenerateNormals(false));
                this->set(std::forward<T_PhongArgs>(args)...);
            }
        };

        std::string phongVertexSource(std::string modelSource, bool usingTexture, bool hasNormals, bool hasColours);
        std::string phongGeometrySource(bool usingTexture, bool hasNormals, bool hasColours);
        std::string phongFragmentSource(bool usingTexture, bool hasColours);

        template <typename RenderContextT>
        void setPhongUniforms(RenderContextT const & ctx, std::unique_ptr<givr::Program> const &p) {
            using namespace givr::style;
            if constexpr (std::is_same<RenderContextT, T_Phong<ColorTexture>>::value) {
                givr::Texture texture = ctx.template value<ColorTexture>();
                if (GLuint(texture)) {
                    glActiveTexture(GL_TEXTURE1);
                    texture.bind(GL_TEXTURE_2D);
                    p->setInt("colorTexture", 1);
                    glActiveTexture(GL_TEXTURE0);
                }
            } else {
                p->setVec3("colour", ctx.params.template value<Colour>());
            }
            p->setVec3("lightPosition", ctx.params.template value<LightPosition>());
            p->setFloat("ambientFactor", ctx.params.template value<AmbientFactor>());
            p->setFloat("specularFactor", ctx.params.template value<SpecularFactor>());
            p->setFloat("phongExponent", ctx.params.template value<PhongExponent>());
            p->setBool("perVertexColour", ctx.params.template value<PerVertexColour>());
            p->setBool("showWireFrame", ctx.params.template value<ShowWireFrame>());
            p->setVec3("wireFrameColour", ctx.params.template value<WireFrameColour>());
            p->setFloat("wireFrameWidth", ctx.params.template value<WireFrameWidth>());
            p->setBool("generateNormals", ctx.params.template value<GenerateNormals>());
        }

        template <typename GeometryT, typename ColorSrc>
        typename GeometryT::Data fillBuffers(GeometryT const &g, T_Phong<ColorSrc> const &) {
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
            static_assert(
                hasUvs<GeometryT>::value || std::is_same<ColorSrc, Colour>::value,
                "The Phong Texture style requires uvs. The geometry you are using does "
                "not provide them."
                );

            return std::move(generateGeometry(g));
        }

        template <typename GeometryT, typename StyleT>
        std::unique_ptr<Program> getPhongShaderProgram(std::string modelSource) {
            constexpr bool _hasNormals = hasNormals<GeometryT>::value;
            constexpr bool _hasColours = hasColours<GeometryT>::value;
            constexpr bool _useTex = std::is_same<StyleT, T_Phong<ColorTexture>>::value;
            return std::make_unique<Program>(
                Shader{
                    phongVertexSource(modelSource, _useTex, _hasNormals, _hasColours),
                    GL_VERTEX_SHADER
                },
                Shader{
                    phongGeometrySource(_useTex, _hasNormals, _hasColours),
                    GL_GEOMETRY_SHADER
                },
                Shader{
                    phongFragmentSource(_useTex, _hasColours),
                    GL_FRAGMENT_SHADER
                }
            );
        }

        template <typename GeometryT, typename StyleT>
        RenderContext<GeometryT, StyleT>
        getContext(GeometryT const &, StyleT const &p) {
            RenderContext<GeometryT, StyleT> ctx;
            ctx.shaderProgram = getPhongShaderProgram<GeometryT, StyleT>(ctx.getModelSource());
            ctx.primitive = getPrimitive<GeometryT>();
            updateStyle(ctx, p);
            return std::move(ctx);
        }

        template <typename GeometryT, typename StyleT>
        InstancedRenderContext<GeometryT, StyleT>
        getInstancedContext(GeometryT const &, StyleT const &p) {
            InstancedRenderContext<GeometryT, StyleT> ctx;
            ctx.shaderProgram = getPhongShaderProgram<GeometryT, StyleT>(ctx.getModelSource());
            ctx.primitive = getPrimitive<GeometryT>();
            updateStyle(ctx, p);
            return std::move(ctx);
        }


        template <typename RenderContextT, typename ColorSrc>
        void updateStyle(RenderContextT &ctx, T_Phong<ColorSrc> const &p) {
            ctx.params.set(p.args);
        }

        // TODO: come up with a better way to not duplicate OpenGL state setup
        template <typename GeometryT, typename ViewContextT, typename ColorSrc>
        void draw(InstancedRenderContext<GeometryT, T_Phong<ColorSrc>> &ctx, ViewContextT const &viewCtx) {
            glEnable(GL_MULTISAMPLE);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            drawInstanced(ctx, viewCtx, [&ctx](std::unique_ptr<Program> const &program) {
                setPhongUniforms(ctx, program);
            });
        }

        template <typename GeometryT, typename ViewContextT, typename ColorSrc>
        void draw(RenderContext<GeometryT, T_Phong<ColorSrc>>& ctx, ViewContextT const &viewCtx, mat4f const model = mat4f(1.f)) {
            glEnable(GL_MULTISAMPLE);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            drawArray(ctx, viewCtx, [&ctx, &model](std::unique_ptr<Program> const &program) {
                setPhongUniforms(ctx, program);
                program->setMat4("model", model);
            });
        }


        using TexturedPhongStyle = T_Phong<ColorTexture>;
        using PhongStyle = T_Phong<Colour>;
        using TexturedPhong = T_Phong<ColorTexture>;
        using Phong = T_Phong<Colour>;

    }// end namespace style
}// end namespace givr

