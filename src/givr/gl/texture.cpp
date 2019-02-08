#include "texture.h"
#include <cassert>
#include "stb_image.h"

using Texture = givr::Texture;

Texture::Texture(
) : m_textureID{0}
{
    alloc();
}
// TODO(lw): make a version that just receives the source directly.
Texture::~Texture() {
    dealloc();
}

void Texture::alloc()
{
    dealloc();
    glGenTextures(1, &m_textureID);
}
void Texture::dealloc()
{
    if (m_textureID) {
        glDeleteBuffers(1, &m_textureID);
    }
}

void Texture::bind(GLenum target)
{
    glBindTexture(target, m_textureID);
}

void Texture::load(GLenum target, std::string filename, GLint level, GLenum format)
{
    // load and generate the texture
    int width, height, channels;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (data) {
        glTexImage2D(target, level, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(target);
    } else {
        throw std::runtime_error("Failed to load texture");
    }
    stbi_image_free(data);
}
