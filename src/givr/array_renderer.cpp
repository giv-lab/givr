#include "array_renderer.h"

namespace givr {
    void allocate_buffers(array_render_context &ctx) {
        ctx.vao = std::make_unique<vertex_array>();
        ctx.vao->alloc(1);

        auto allocate_buffer = [&ctx]() {
            std::unique_ptr<buffer> vbo = std::make_unique<buffer>();
            vbo->alloc(1);
            ctx.array_buffers.push_back(std::move(vbo));
        };

        // Upload / bind / map model data
        allocate_buffer();//data.vertices);
        allocate_buffer();//data.normals);
        allocate_buffer();//data.uvs);
        allocate_buffer();//data.colours);
    }

    void upload_buffers(
        array_render_context &ctx,
        buffer_data const &data
    ) {
        std::uint16_t va_index = 0;
        ctx.vao->bind();

        ctx.start_index = 0;
        ctx.end_index =  data.vertices.size() / 3;

        std::uint16_t buffer_index = 0;
        auto apply_buffer = [&ctx, &va_index, &buffer_index](
            GLenum type,
            GLuint size,
            GLenum buffer_type,
            std::vector<float> const &data
        ) {
            std::unique_ptr<buffer> &vbo = ctx.array_buffers[buffer_index];
            vbo->bind(type);
            vbo->data(type, data, buffer_type);
            glVertexAttribPointer(va_index, size, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
            glEnableVertexAttribArray(va_index);
            ++va_index;
            ++buffer_index;
        };

        // Upload / bind / map model data
        apply_buffer(GL_ARRAY_BUFFER, 3, get_buffer_usage_type(data.vertices_type), data.vertices);
        apply_buffer(GL_ARRAY_BUFFER, 3, get_buffer_usage_type(data.normals_type), data.normals);
        apply_buffer(GL_ARRAY_BUFFER, 2, get_buffer_usage_type(data.uvs_type), data.uvs);
        apply_buffer(GL_ARRAY_BUFFER, 3, get_buffer_usage_type(data.colours_type), data.colours);

        ctx.vao->unbind();
    }
};// end namespace givr