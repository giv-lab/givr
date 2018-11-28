#pragma once

#include "buffer_data.h"
#include "types.h"
#include "gl/shader.h"
#include "gl/program.h"
#include "gl/buffer.h"
#include "gl/vertex_array.h"

#include <glad/glad.h>

#include <functional>
#include <memory>
#include <vector>

namespace givr {

    struct instanced_render_context {
        std::unique_ptr<program> shader_program;
        std::unique_ptr<vertex_array> vao;

        std::vector<mat4f> model_transforms;
        std::unique_ptr<buffer> model_transforms_buffer;

        // Keep references to the GL_ARRAY_BUFFERS so that
        // the stay in scope for this context.
        std::vector<std::unique_ptr<buffer>> array_buffers;
        GLuint number_of_indices;

        primitive_type primitive;
    };

    template <typename ViewContextT>
    void draw_instanced(
        instanced_render_context &ctx,
        ViewContextT const &view_ctx,
        std::function<void(std::unique_ptr<program> const&)> set_uniforms
    ) {
        ctx.shader_program->use();

        mat4f view = get_view_matrix(view_ctx.camera);
        mat4f projection = get_projection_matrix(view_ctx.projection);
        vec3f view_pos = get_view_position(view_ctx.camera);

        ctx.shader_program->set_vec3("view_pos", view_pos);
        ctx.shader_program->set_mat4("view", view);
        ctx.shader_program->set_mat4("projection", projection);
        set_uniforms(ctx.shader_program);

        ctx.vao->bind();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        GLenum mode = givr::get_mode(ctx.primitive);
        ctx.model_transforms_buffer->bind(GL_ARRAY_BUFFER);
        ctx.model_transforms_buffer->data(GL_ARRAY_BUFFER, ctx.model_transforms, GL_DYNAMIC_DRAW);

        glDrawElementsInstanced(
            mode, ctx.number_of_indices, GL_UNSIGNED_INT, 0, ctx.model_transforms.size()
        );

        ctx.vao->unbind();

        ctx.model_transforms.clear();
    }
    void allocate_buffers(
        instanced_render_context &ctx
    );
    void upload_buffers(
        instanced_render_context &ctx,
        buffer_data const &data
    );
};// end namespace givr
