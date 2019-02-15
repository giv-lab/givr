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

    struct RenderContext {
        std::unique_ptr<Program> shaderProgram;
        std::unique_ptr<VertexArray> vao;

        // Keep references to the GL_ARRAY_BUFFERS so that
        // the stay in scope for this context.
        std::vector<std::unique_ptr<Buffer>> arrayBuffers;

        GLuint numberOfIndices;
        GLuint startIndex;
        GLuint endIndex;

        PrimitiveType primitive;

        bool hasIndices = false;
    };

    template <typename ViewContextT>
    void drawArray(
        RenderContext &ctx,
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
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        GLenum mode = givr::getMode(ctx.primitive);
        if (ctx.numberOfIndices > 0) {
            glDrawElements(mode, ctx.numberOfIndices, GL_UNSIGNED_INT, 0);
        } else {
            glDrawArrays(mode, ctx.startIndex, ctx.endIndex);
        }

        ctx.vao->unbind();
    }
    void allocateBuffers(
        RenderContext &ctx
    );
    void uploadBuffers(
        RenderContext &ctx,
        BufferData const &data
    );
};// end namespace givr
