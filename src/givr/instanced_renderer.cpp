#include "instanced_renderer.h"

namespace givr {

    void upload_buffers(
        instanced_render_context &ctx,
        buffer_data const &data
    ) {
        std::uint16_t index = 0;
        ctx.vao = std::make_unique<vertex_array>();
        ctx.vao->alloc(1);
        ctx.vao->bind();


        // Map - but don't upload framing data.
        ctx.model_transforms_buffer = std::make_unique<buffer>();
        ctx.model_transforms_buffer->alloc(1);
        ctx.model_transforms_buffer->bind(GL_ARRAY_BUFFER);
        ctx.model_transforms_buffer->debug();
        auto vec4_size = sizeof(mat4f)/4;

        for (std::uint16_t i = 0; i < 4; ++i) {
            glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, sizeof(mat4f), (GLvoid*)(i*vec4_size));
            glEnableVertexAttribArray(index);
            glVertexAttribDivisor(index, 1);
            ++index;
        }


        std::unique_ptr<buffer> indices = std::make_unique<buffer>();
        indices->alloc(1);
        indices->bind(GL_ELEMENT_ARRAY_BUFFER);
        indices->data(GL_ELEMENT_ARRAY_BUFFER, data.indices, GL_STATIC_DRAW);
        ctx.number_of_indices = data.indices.size();
        ctx.array_buffers.push_back(std::move(indices));

        auto apply_buffer = [&ctx, &index](
            GLenum type,
            GLuint size,
            std::vector<float> const &data
        ) {
            if (data.size() == 0) {
                return;
            }
            std::unique_ptr<buffer> vbo = std::make_unique<buffer>();
            vbo->alloc(1);
            vbo->bind(type);
            vbo->data(type, data, GL_STATIC_DRAW);
            glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
            glEnableVertexAttribArray(index);
            ctx.array_buffers.push_back(std::move(vbo));
            ++index;
        };

        // Upload / bind / map model data
        apply_buffer(GL_ARRAY_BUFFER, 3, data.vertices);
        apply_buffer(GL_ARRAY_BUFFER, 3, data.normals);
        apply_buffer(GL_ARRAY_BUFFER, 2, data.uvs);
        apply_buffer(GL_ARRAY_BUFFER, 3, data.colours);

        ctx.vao->unbind();
    }
};// end namespace givr
