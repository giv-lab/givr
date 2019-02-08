#include "renderer.h"

namespace givr {
    void allocateBuffers(RenderContext &ctx) {
        ctx.vao = std::make_unique<VertexArray>();
        ctx.vao->alloc();

        // Map - but don't upload indices data
        std::unique_ptr<Buffer> indices = std::make_unique<Buffer>();
        indices->alloc();
        ctx.arrayBuffers.push_back(std::move(indices));

        auto allocateBuffer = [&ctx]() {
            std::unique_ptr<Buffer> vbo = std::make_unique<Buffer>();
            vbo->alloc();
            ctx.arrayBuffers.push_back(std::move(vbo));
        };

        // Upload / bind / map model data
        allocateBuffer();//data.vertices);
        allocateBuffer();//data.normals);
        allocateBuffer();//data.uvs);
        allocateBuffer();//data.colours);
    }

    void uploadBuffers(
        RenderContext &ctx,
        BufferData const &data
    ) {
        // Start by setting the appropriate context variables for rendering.
        ctx.numberOfIndices = data.indices.size();
        ctx.startIndex = 0;
        ctx.endIndex =  data.vertices.size() / 3;

        std::uint16_t vaIndex = 0;
        ctx.vao->bind();

        std::unique_ptr<Buffer> &indices = ctx.arrayBuffers[0];
        indices->bind(GL_ELEMENT_ARRAY_BUFFER);
        indices->data(
                GL_ELEMENT_ARRAY_BUFFER,
                data.indices,
                getBufferUsageType(data.indicesType));

        std::uint16_t bufferIndex = 1;
        auto applyBuffer = [&ctx, &vaIndex, &bufferIndex](
            GLenum type,
            GLuint size,
            GLenum bufferType,
            std::vector<float> const &data
        ) {
            // if this data piece is empty
            if (data.size() == 0) {
                ++bufferIndex;
                return;
            }
            std::unique_ptr<Buffer> &vbo = ctx.arrayBuffers[bufferIndex];
            vbo->bind(type);
            vbo->data(type, data, bufferType);
            glVertexAttribPointer(vaIndex, size, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
            glEnableVertexAttribArray(vaIndex);
            ++vaIndex;
            ++bufferIndex;
        };

        // Upload / bind / map model data
        applyBuffer(GL_ARRAY_BUFFER, 3, getBufferUsageType(data.verticesType), data.vertices);
        applyBuffer(GL_ARRAY_BUFFER, 3, getBufferUsageType(data.normalsType), data.normals);
        applyBuffer(GL_ARRAY_BUFFER, 2, getBufferUsageType(data.uvsType), data.uvs);
        applyBuffer(GL_ARRAY_BUFFER, 3, getBufferUsageType(data.coloursType), data.colours);

        ctx.vao->unbind();

        ctx.arrayBuffers[0]->unbind(GL_ELEMENT_ARRAY_BUFFER);
        if (ctx.arrayBuffers.size() > 1) {
            ctx.arrayBuffers[1]->unbind(GL_ARRAY_BUFFER);
        }
    }
};// end namespace givr
