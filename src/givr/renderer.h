#pragma once

#include "buffer_data.h"
#include "types.h"
#include "gl/shader.h"
#include "gl/program.h"
#include "gl/buffer.h"
#include "gl/vertex_array.h"

#include "gl.h"
#include "static_assert.h"

#include <functional>
#include <memory>
#include <vector>

namespace givr {

    template <typename GeometryT, typename StyleT>
    struct RenderContext {
        std::unique_ptr<Program> shaderProgram;
        std::unique_ptr<VertexArray> vao;

        // Keep references to the GL_ARRAY_BUFFERS so that
        // the stay in scope for this context.
        std::vector<std::unique_ptr<Buffer>> arrayBuffers;

        GLuint numberOfIndices;
        GLuint startIndex;
        GLuint vertexCount;

        PrimitiveType primitive;

        bool hasIndices = false;

        typename StyleT::Parameters params;

        // Default ctor/dtor & move operations
        RenderContext() = default;
        ~RenderContext() = default;
        RenderContext(RenderContext &&) = default;
        RenderContext &operator=(RenderContext &&) = default;

        // But no copy or assignment. Bad.
        RenderContext(const RenderContext & ) = delete;
        RenderContext &operator=(const RenderContext &) = delete;

        std::string getModelSource() const { return "uniform"; }
    };

    template <typename GeometryT, typename StyleT, typename ViewContextT>
    void drawArray(
        RenderContext<GeometryT, StyleT> &ctx,
        ViewContextT const &viewCtx,
        std::function<void(std::unique_ptr<Program> const&)> setUniforms
    ) {
        ctx.shaderProgram->use();

        mat4f view = viewCtx.camera.viewMatrix();
        mat4f projection = viewCtx.projection.projectionMatrix();
        vec3f viewPosition = viewCtx.camera.viewPosition();

        ctx.shaderProgram->setVec3("viewPosition", viewPosition);
        ctx.shaderProgram->setMat4("view", view);
        ctx.shaderProgram->setMat4("projection", projection);
        setUniforms(ctx.shaderProgram);
        ctx.vao->bind();
        glPolygonMode(GL_FRONT, GL_FILL);
        GLenum mode = givr::getMode(ctx.primitive);
        if constexpr (hasIndices<GeometryT>::value) {
            if (ctx.numberOfIndices > 0) {
                glDrawElements(mode, ctx.numberOfIndices, GL_UNSIGNED_INT, 0);
            } else {
                glDrawArrays(mode, ctx.startIndex, ctx.vertexCount);
            }
        } else {
            glDrawArrays(mode, ctx.startIndex, ctx.vertexCount);
        }

        ctx.vao->unbind();
    }
    template <typename GeometryT, typename StyleT>
    void allocateBuffers(RenderContext<GeometryT, StyleT> &ctx) {
        ctx.vao = std::make_unique<VertexArray>();
        ctx.vao->alloc();

        if constexpr (hasIndices<GeometryT>::value) {
            // Map - but don't upload indices data
            std::unique_ptr<Buffer> indices = std::make_unique<Buffer>();
            indices->alloc();
            ctx.arrayBuffers.push_back(std::move(indices));
        }

        auto allocateBuffer = [&ctx]() {
            std::unique_ptr<Buffer> vbo = std::make_unique<Buffer>();
            vbo->alloc();
            ctx.arrayBuffers.push_back(std::move(vbo));
        };

        // Upload / bind / map model data
        if constexpr (hasVertices<GeometryT>::value) {
            allocateBuffer();//data.vertices);
        }
        if constexpr (hasNormals<GeometryT>::value) {
            allocateBuffer();//data.normals);
        }
        if constexpr (hasUvs<GeometryT>::value) {
            allocateBuffer();//data.uvs);
        }
        if constexpr (hasColours<GeometryT>::value) {
            allocateBuffer();//data.colours);
        }
    }
    template <typename GeometryT, typename StyleT>
    void uploadBuffers(
        RenderContext<GeometryT, StyleT> &ctx,
        typename GeometryT::Data const &data
    ) {
        // Start by setting the appropriate context variables for rendering.
        if constexpr (hasIndices<GeometryT>::value) {
            ctx.numberOfIndices = data.indices.size();
        } else {
            ctx.numberOfIndices = 0;
        }
        ctx.startIndex = 0;
        ctx.vertexCount =  data.vertices.size() / data.dimensions;

        std::uint16_t vaIndex = 4;
        ctx.vao->bind();

        std::uint16_t bufferIndex = 0;
        if constexpr (hasIndices<GeometryT>::value) {
            std::unique_ptr<Buffer> &indices = ctx.arrayBuffers[0];
            indices->bind(GL_ELEMENT_ARRAY_BUFFER);
            indices->data(
                    GL_ELEMENT_ARRAY_BUFFER,
                    data.indices,
                    getBufferUsageType(data.indicesType));
            ++bufferIndex;
        }

        auto applyBuffer = [&ctx, &vaIndex, &bufferIndex](
            GLenum type,
            GLuint size,
            GLenum bufferType,
            std::string name,
            gsl::span<const float> const &data
        ) {
            // if this data piece is empty disable this one.
            std::unique_ptr<Buffer> &vbo = ctx.arrayBuffers[bufferIndex];
            vbo->bind(type);
            if (data.size() == 0) {
                glDisableVertexAttribArray(vaIndex);
            } else {
                glBindAttribLocation(*ctx.shaderProgram.get(), vaIndex, name.c_str());
                vbo->data(type, data, bufferType);
                glVertexAttribPointer(vaIndex, size, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
                glEnableVertexAttribArray(vaIndex);
            }
            ++vaIndex;
            ++bufferIndex;
        };

        // Upload / bind / map model data
        if constexpr (hasVertices<GeometryT>::value) {
            applyBuffer(GL_ARRAY_BUFFER, data.dimensions, getBufferUsageType(data.verticesType), "position", data.vertices);
        }
        if constexpr (hasNormals<GeometryT>::value) {
            applyBuffer(GL_ARRAY_BUFFER, data.dimensions, getBufferUsageType(data.normalsType), "normals", data.normals);
        }
        if constexpr (hasUvs<GeometryT>::value) {
            applyBuffer(GL_ARRAY_BUFFER, 2, getBufferUsageType(data.uvsType), "uvs", data.uvs);
        }
        if constexpr (hasColours<GeometryT>::value) {
            applyBuffer(GL_ARRAY_BUFFER, 3, getBufferUsageType(data.coloursType), "colour", data.colours);
        }

        ctx.vao->unbind();

        if constexpr (hasIndices<GeometryT>::value) {
            ctx.arrayBuffers[0]->unbind(GL_ELEMENT_ARRAY_BUFFER);
            if (ctx.arrayBuffers.size() > 1) {
                ctx.arrayBuffers[1]->unbind(GL_ARRAY_BUFFER);
            }
        }
    }
};// end namespace givr
