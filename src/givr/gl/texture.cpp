#include "texture.h"
#include <cassert>
#include "stb_image.h"

using texture = givr::texture;

texture::texture(
) : m_number_textures{0}
  , m_texture_ids{nullptr}
{
}
texture::texture(
    GLuint num
) : m_number_textures{num}
  , m_texture_ids{nullptr}
{
    alloc(num);
}
// TODO(lw): make a version that just receives the source directly.
texture::~texture() {
}

void texture::alloc(GLuint num)
{
    dealloc();
    m_number_textures = num;
    m_texture_ids = new GLuint[m_number_textures];
    glGenTextures(m_number_textures, m_texture_ids);
}
void texture::dealloc()
{
    if (m_texture_ids) {
        glDeleteBuffers(m_number_textures, m_texture_ids);
        delete[] m_texture_ids;
        m_number_textures = 0;
    }
}

void texture::bind(GLenum target)
{
    bind(target, 0);
}

// TODO: target generates an unused paramater warning.
//       this is probably a bug or a bad design.
void texture::bind(GLenum target, GLuint i)
{
    assert(i < m_number_textures);
    glBindTexture(GL_TEXTURE_2D, m_texture_ids[i]);
}

// TODO: level generates an unused paramater warning.
//       this is probably a bug or a bad design.
void texture::load(GLenum target, std::string filename, GLint level, GLenum format)
{
    // load and generate the texture
    int width, height, channels;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (data) {
        glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(target);
    } else {
        throw std::runtime_error("Failed to load texture");
    }
    stbi_image_free(data);
}
