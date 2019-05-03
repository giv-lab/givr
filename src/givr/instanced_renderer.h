#pragma once

#include "buffer_data.h"
#include "types.h"
#include "gl/shader.h"
#include "gl/program.h"
#include "gl/buffer.h"
#include "gl/vertex_array.h"

#include "gl.h"

#include <functional>
#include <memory>
#include <vector>

namespace givr {

    struct InstancedRenderContext {
        std::unique_ptr<Program> shaderProgram;
        std::unique_ptr<VertexArray> vao;

        std::vector<mat4f> modelTransforms;
        std::unique_ptr<Buffer> modelTransformsBuffer;

        // Keep references to the GL_ARRAY_BUFFERS so that
        // the stay in scope for this context.
        std::vector<std::unique_ptr<Buffer>> arrayBuffers;

        GLuint numberOfIndices;
        GLuint startIndex;
        GLuint endIndex;

        PrimitiveType primitive;

        // Default ctor/dtor & move operations
        InstancedRenderContext() = default;
        ~InstancedRenderContext() = default;
        InstancedRenderContext(InstancedRenderContext &&) = default;
        InstancedRenderContext &operator=(InstancedRenderContext &&) = default;

        // But no copy or assignment. Bad.
        InstancedRenderContext(const InstancedRenderContext & ) = delete;
        InstancedRenderContext &operator=(const InstancedRenderContext &) = delete;
    };

    template <typename ViewContextT>
    void drawInstanced(
        InstancedRenderContext &ctx,
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
        ctx.modelTransformsBuffer->bind(GL_ARRAY_BUFFER);
        ctx.modelTransformsBuffer->data(GL_ARRAY_BUFFER, ctx.modelTransforms, GL_DYNAMIC_DRAW);

        if (ctx.numberOfIndices > 0) {
            glDrawElementsInstanced(
                mode, ctx.numberOfIndices, GL_UNSIGNED_INT, 0, ctx.modelTransforms.size()
            );
        } else {
            glDrawArraysInstanced(
                mode, ctx.startIndex, ctx.endIndex, ctx.modelTransforms.size()
            );
        }

        ctx.vao->unbind();

        ctx.modelTransforms.clear();
    }
    void allocateBuffers(
        InstancedRenderContext &ctx
    );
    void uploadBuffers(
        InstancedRenderContext &ctx,
        BufferData const &data
    );
};// end namespace givr
