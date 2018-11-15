#include "array_renderer.h"

namespace givr {

    void upload_buffers(
        array_render_context &ctx,
        buffer_data const &data
    ) {
        std::uint16_t index = 0;
        ctx.vao = std::make_unique<vertex_array>();
        ctx.vao->alloc(1);
        ctx.vao->bind();

        ctx.start_index = 0;
        ctx.end_index =  data.vertices.size() / 3;

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
