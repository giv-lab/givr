#include "instanced_renderer.h"

namespace givr {
    void allocate_buffers(instanced_render_context &ctx) {
        ctx.vao = std::make_unique<vertex_array>();
        ctx.vao->alloc(1);

        // Map - but don't upload framing data.
        ctx.model_transforms_buffer = std::make_unique<buffer>();
        ctx.model_transforms_buffer->alloc(1);

        // Map - but don't upload indices data
        std::unique_ptr<buffer> indices = std::make_unique<buffer>();
        indices->alloc(1);
        ctx.array_buffers.push_back(std::move(indices));

        auto allocate_buffer = [&ctx]() {
            std::unique_ptr<buffer> vbo = std::make_unique<buffer>();
            vbo->alloc(1);
            ctx.array_buffers.push_back(std::move(vbo));
        };

        // bind / map model data
        allocate_buffer();//data.vertices);
        allocate_buffer();//data.normals);
        allocate_buffer();//data.uvs);
        allocate_buffer();//data.colours);
    }

    void upload_buffers(
        instanced_render_context &ctx,
        buffer_data const &data
    ) {
        // Start by setting the appropriate context variables for rendering.
        ctx.number_of_indices = data.indices.size();
        ctx.start_index = 0;
        ctx.end_index =  data.vertices.size() / 3;

        std::uint16_t va_index = 0;
        ctx.vao->bind();

        // Upload framing data.
        ctx.model_transforms_buffer->bind(GL_ARRAY_BUFFER);
        auto vec4_size = sizeof(mat4f)/4;
        for (std::uint16_t i = 0; i < 4; ++i) {
            glVertexAttribPointer(va_index, 4, GL_FLOAT, GL_FALSE, sizeof(mat4f), (GLvoid*)(i*vec4_size));
            glEnableVertexAttribArray(va_index);
            glVertexAttribDivisor(va_index, 1);
            ++va_index;
        }

        std::unique_ptr<buffer> &indices = ctx.array_buffers[0];
        indices->bind(GL_ELEMENT_ARRAY_BUFFER);
        indices->data(
                GL_ELEMENT_ARRAY_BUFFER,
                data.indices,
                get_buffer_usage_type(data.indices_type));

        std::uint16_t buffer_index = 1;
        auto apply_buffer = [&ctx, &va_index, &buffer_index](
            GLenum type,
            GLuint size,
            GLenum buffer_type,
            std::vector<float> const &data
        ) {
            if (data.size() == 0) {
                ++buffer_index;
                return;
            }
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

        ctx.array_buffers[0]->unbind(GL_ELEMENT_ARRAY_BUFFER);
        if (ctx.array_buffers.size() > 1) {
            ctx.array_buffers[1]->unbind(GL_ARRAY_BUFFER);
        }

    }
};// end namespace givr
